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

// Search function
int search(char *string, char *input, char *output, struct opts options){
    //Input file pointer
    FILE *ifp;
    //Output file pointer
    FILE *ofp;
    int line_num = 1;
    int find_result = 0;
    
    char *buffer;

    char *word = NULL;

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

        printf("Pointer to buffer: %p\n", buffer);

        do{

            if(strlen(buffer)<strlen(string)){
                printf(" remaining buffer is smaller than string\n");
                word = NULL;
            }else{
                
                printf("Gonna search for word now\n");

                // If case sensitive, do strstr, else do strcasestr
                word = options.match_case?strstr(buffer, string):strcasestr(buffer, string);


                if(word != NULL)
                    printf("Pointer to word: %p\n", word);

                if(word!=NULL && word[strlen(string)] != '\n' || word[strlen(string)] != '\0'){
                    printf("Gonna change the buffer pointer\n");
                    buffer = word+strlen(string)-1;
                    printf("New pointer to buffer: %p\n", buffer);
                    printf("New buffer: %s\n", buffer);
                }


            //If output_lines, print the line number before the line
                if(options.output_lines)
                    fprintf(ofp,"Line %d:", line_num);

            //if(DEBUG) fprintf(ofp, "\n\nThe line is:%s\nThe Last char is:%d\n", buffer,(int)buffer[strlen(buffer)-1]);

             // Check if:
                 // " word " - leading and trailing spaces, meaning its a single word and not a substring of a bigger word
                 // or "\nword " - is the first word in the line (address of word[0]==address of line[0])
                 // or " word\n" - is the last word in the buffer (leading space and trailing line end)
                 // or "\nword\n" - is the last word in the buffer
                     // Before string rules: space, or address of word[0]== address of buffer[0] -> word==line || *(word-1)==" "
                     // After string rules: space or '\n' -> *(word+strlen(word))==" " || *(word+strlen(word))=="\n"
                     // If one of each applies, word is not substring of larger string.

                if(word==buffer && strcmp((word+strlen(string))," ") || strcmp((word-1)," ") && strcmp((word+strlen(string))," ") || strcmp((word-1)," ") && word[strlen(string)]=='\n'){
                    if(options.mode==1){
                        word[strlen(string)] ='\0';
                        fprintf(ofp,"%s ",word);
                        fprintf(ofp, "\n");
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

                find_result++;

                if(buffer[strlen(buffer)-1]== '\n'){
                    printf("\n");
                }

            }

        }while(word!=NULL);

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