#pragma once

struct opts{
    // Mode: 
    //  1 = literal search (e.g. if you search "hello", you get one occurence for each time "hello" appears in the output file)
    //  2 = contextual search (e.g. if you search "hello", you get context(the size of the buffer) for where "hello" appears in the output file)
    //  3 = inclusive search (e.g. if you search "hell", you get one occurence for each time "hell" appears even if the whole world is "hello" in the output file)
    int mode;
    // Match case if set to 1
    int match_case;
    // Output line numbers if set to 1
    int output_lines;
};

//TODO: htmlify if output to file 
//TODO: Whole word checking not working


// Implementation of getline() to make it portable: 
// https://sucs.org/Knowledge/Help/Program%20Advisory/Reading%20an%20arbitrarily%20long%20line%20in%20C#diy

char * mygetline(FILE * f){
    size_t size = 0;
    size_t len  = 0;
    size_t last = 0;
    char *buf = NULL;

    do {
        size += BUFSIZ; /* BUFSIZ is defined as "the optimal read size for this platform" */
        buf = realloc(buf, size); /* realloc(NULL,n) is the same as malloc(n) */            
        /* Actually do the read. Note that fgets puts a terminal '\0' on the
           end of the string, so we make sure we overwrite this */
        if (buf == NULL) return NULL;
        fgets(buf + last, size, f);
        len = strlen(buf);
        last = len - 1;
    } while (!feof(f) && buf[last] != '\n');
    return buf;
}

// REFS:
    //http://www.tutorialspoint.com/c_standard_library/c_function_sscanf.htm
    //http://stackoverflow.com/questions/11198604/c-split-string-into-an-array-of-strings
    //https://msdn.microsoft.com/en-us/library/2c8d19sb(v=vs.71).aspx
    //http://stackoverflow.com/questions/8097620/c-read-from-input-until-newline-is-found
    //http://stackoverflow.com/questions/7586990/strstr-faster-than-algorithms
    //http://qa.geeksforgeeks.org/3247/write-strstr-function-with-single-without-strcmp-function
    //http://stackoverflow.com/questions/9206091/going-through-a-text-file-line-by-line-in-c


// Search function
int search(char *string, char *input, char *output, struct opts options){
    //Input file pointer
    FILE *ifp;
    //Output file pointer
    FILE *ofp;
    int line_num = 1;
    int find_result = 0;
    // CAVEAT: if word gets split in half due to line buffer size, it won't be found...
    //char buffer[512];
    char *word;
    char line;
    char *buffer = &line;

    //Open input file
    //If file fails to open, output message and return 0 for unsuccessful
    if((ifp = fopen(input, "r")) == NULL){
        printf("Can't open input file: %s\n",input);
        return 0;
    }else{
        printf("Opened file: %s to read from\n",input);
    }

    //Open output file or set stdout as output
    //strcmp() returns 0 if the strings are equal.
    if(!strcmp(output,"stdout")){
        ofp=stdout;
    //Else, open file output, if file fails to open, output message and return 0 for unsuccessful
    }else if((ofp=fopen(output, "w"))==NULL){
        printf("Can't open output file: %s\n",output);
        return 0;
    //Else, print success message
    }else{
        printf("Opened file: %s to write to\n",output);
    }

    // Print the result separator
    printf("\n--RESULTS--\n\n");

    while(line=mygetline(ifp) != NULL){
        printf("%s\n",*line );
        // If case sensitive, do strstr, else do strcasestr
        word=options.match_case?strstr(buffer, string):strcasestr(buffer, string);

        if(word!=NULL){
            //If output_lines, print the line number before the line
            if(options.output_lines)
                fprintf(ofp,"Line %d:", line_num);
             // Check if:
                 // " word " - leading and trailing spaces, meaning its a single word and not a substring of a bigger word
                 // or "\nword " - is the first word in the line (address of word[0]==address of line[0])
                 // or " word\n" - is the last word in the buffer (leading space and trailing line end)
                 // or "\nword\n" - is the last word in the buffer
                     // Before string rules: space, or address of word[0]== address of buffer[0] -> word==line || *(word-1)==" "
                     // After string rules: space or '\n' -> *(word+strlen(word))==" " || *(word+strlen(word))=="\n"
                     // If one of each applies, word is not substring of larger string.

            if(word==buffer || strcmp((word-1)," ") && strcmp((word+strlen(string))," ") || word[strlen(string)]=='\n'){
                if(options.mode==1){
                    fprintf(ofp,"%s ",word);
                }else if(options.mode==2){
                    fprintf(ofp,"%s ",buffer);
                }
            }

            if(options.mode>2){
                if(options.mode==3){
                    fprintf(ofp,"%s ",word);
                }else if(options.mode==4){
                    fprintf(ofp,"%s ",buffer);
                }
            }
            find_result++;
            if(buffer[strlen(buffer)-1]== '\n'){
                printf("\n");
            }
        }
        if(buffer[strlen(buffer)-1]== '\n'){
            line_num++;
        }

    }



    /*if(options.match_case){
        while(fgets(line, 1030, ifp) != NULL) {
            word = strstr(line, string);
            if(word != NULL) {
                if(options.output_lines){
                    fprintf(ofp,"Line %d:", line_num);
                }
                fprintf(ofp,"%s",line);
                find_result++;
            }
            if(line[strlen(line)-1]== '\n'){
                line_num++;
                printf("\n");
            }
        }
    }else if(!options.match_case){
        while(fgets(line, 1030, ifp) != NULL) {
            if((strcasestr(line, string)) != NULL) {
                if(options.output_lines){
                    fprintf(ofp,"Line %d:",line_num);
                }
                fprintf(ofp,"%s\n",line);
                find_result++;
            }
            //get last char of the string in the buffer, if \n, increase line count and print \n for end of line
            if(line[strlen(line)-1]== '\n'){
                line_num++;
                printf("\n");
            }
        }
    }*/


    if(find_result == 0) {
        printf("\nNo matches found for %s in %s.\n",string,input);
    }else{
        printf("\n%d Matches found.\n", find_result);
    }

    //Close the files if still open.
    if(ifp) 
        fclose(ifp);

    if(ofp && !strcmp(output,"stdout"))
        fclose(ofp);
}