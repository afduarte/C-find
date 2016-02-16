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
//STATUS: loop until segmentation fault because word is never != NULL

// Checks if a passed in(in the form of a pointer) char is a white space character that matters for this use case
// Returns 0 if the char passed in is not a '\t', or a '\n' or a ' '
int is_white(char *ch){
    if(*ch==' ')
        return 1;
    else if(*ch=='\n')
        return 2;
    else if(*ch=='\t')
        return 3;
    else
        return 0;
}
// Search function
int search(char *string, char *input, char *output, struct opts options){
    //Input file pointer
    FILE *ifp;
    //Output file pointer
    FILE *ofp;
    int line_num = 1;
    int find_result = 0;
    
    char *buffer;

    char *word;

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
        //if(DEBUG)printf("Can't open output file: %s\n",output);
        return 0;
    //Else, print success message
    }else{
        //if(DEBUG)printf("Opened file: %s to write to\n",output);
    }

    // Print the result separator
    printf("\n--RESULTS--\n\n");

    buffer = malloc(512*sizeof(char));

    while(fgets(buffer, 512, ifp) != NULL){

        // If case sensitive, do strstr, else do strcasestr
        // word = options.match_case?strstr(buffer, string):strcasestr(buffer, string)

        while( (word=strstr(buffer, string)) != NULL){

            // word = strstr(buffer, string);

            if(options.output_lines)
                fprintf(ofp,"Line %d:", line_num);

            if(word==buffer && is_white(word+strlen(string)) || is_white((word-1)) && is_white(word+strlen(string)) ){
                if(options.mode==1){
                    char *print_word = (char*)malloc(sizeof(char)*strlen(string)+1);
                    strcpy(print_word,word);
                    print_word[strlen(string)] ='\0';
                    fprintf(ofp,"%s ",print_word);
                    fprintf(ofp, "\n");
                    // free(print_word);
                }else if(options.mode==2){
                    fprintf(ofp,"%s ",buffer);
                }
            }

            if(options.mode>2){
                if(options.mode==3){
                    word[strlen(string)] ='\0';
                    fprintf(ofp,"%s ",word);
                }else if(options.mode==4){
                    fprintf(ofp,"%s ",buffer);
                }
            }

            fprintf(ofp, "%d\n",find_result++);

            // check if it is safe to move the buffer pointer to the end of the word pointer.
            // if it is, move it, else, change word to null to break out of the while loop
            if((buffer+strlen(buffer)-1) >= (word+strlen(string))){

                if(!word+strlen(string)=='\n'){
                    buffer = word+strlen(string);
                    getchar();
                }

                if(word[strlen(string)]=='\n'){
                    fprintf(ofp, "Word set to null\n");
                    word = NULL;
                }
                // fprintf(ofp, "Buffer after changing: %s Address: %p\n",buffer,buffer);
                
            }

            // if(buffer[strlen(buffer)-1]== '\n'){
            //     printf("\n");
            // }

        }



        // if(word!=NULL && word[strlen(string)] != '\n' || word[strlen(string)] != '\0'){
        //     printf("Gonna change the buffer pointer\n");
        //     buffer = word+strlen(string)-1;
        //     printf("New pointer to buffer: %p\n", buffer);
        //     printf("New buffer: %s\n", buffer);
        // }


            //If output_lines, print the line number before the line


            //if(DEBUG) fprintf(ofp, "\n\nThe line is:%s\nThe Last char is:%d\n", buffer,(int)buffer[strlen(buffer)-1]);

             // Check if:
                 // " word " - leading and trailing spaces, meaning its a single word and not a substring of a bigger word
                 // or "\nword " - is the first word in the line (address of word[0]==address of line[0])
                 // or " word\n" - is the last word in the buffer (leading space and trailing line end)
                 // or "\nword\n" - is the last word in the buffer
                     // Before string rules: space, or address of word[0]== address of buffer[0] -> word==line || *(word-1)==" "
                     // After string rules: space or '\n' -> *(word+strlen(word))==" " || *(word+strlen(word))=="\n"
                     // If one of each applies, word is not substring of larger string.



        


        if(buffer[strlen(buffer)-1]== '\n'){
            line_num++;
        }


    }



    free(buffer);

    //Close the files if still open.
        if(ifp) 
            fclose(ifp);

        if(ofp && !strcmp(output,"stdout"))
            fclose(ofp);

        if(find_result == 0) {
            printf("\nNo matches found for %s in %s.\n",string,input);
        }else{
            printf("\n%d Matches found.\n", find_result);
        }

        return find_result;
    }