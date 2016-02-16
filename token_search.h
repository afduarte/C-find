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

        printf("Pointer to buffer: %p\n", buffer);

        //If output_lines, print the line number before the line
        if(options.output_lines){
            fprintf(ofp,"Line %d:", line_num);
        }

        while(word=strtok(buffer, " ") != NULL){
            if(!strcmp(word,string)){
                if(options.mode==1){
                    char *match = word;
                    match[strlen(string)] ='\0';
                    fprintf(ofp,"%s ",word);
                    fprintf(ofp, "\n");

                }else if(options.mode==2){
                    fprintf(ofp,"%s ",buffer);
                }

            }
        }

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