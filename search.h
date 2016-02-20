#pragma once

#include <ctype.h>

#define BUFF_SIZE  512*sizeof(char)

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

//STATUS: modes 1&3 working, lacking mode 2 and multi word search
//TODO: Implement inclusive search with strstr

int find_match(const char *haystack, const char *needle, int mode){

    // Returns a struct that holds the result (1 or 0) and an int that represents the offset from the first character where the first match happened
    // The char where the first match happened should only be accounted for if the result is 1

    // j keeps track of how many characters have been checked
    int j = 0;
    // matches keeps track of how many matches were found

    if(mode==3){
        if((strstr(haystack,needle)) != NULL){
            return 1;
        }else{
            return 0;
        }
    }

    for(int i =0;i<strlen(haystack);i++){
        if(haystack[i]==needle[j]){
            j++;
        }else{
            return 0;
        }
    }

    return 1;

    
}

int find_match_nocase(const char *haystack, const char *needle, int mode){
    char *temp_haystack;
    char *temp_needle;
    int r;

    temp_haystack=(char*)malloc((strlen(haystack))*sizeof(char));
    strcpy(temp_haystack,haystack);

    temp_needle=(char*)malloc((strlen(needle))*sizeof(char));
    strcpy(temp_needle,needle);

    for(int i = 0; i<(strlen(temp_needle)-1); i++){
        temp_needle[i] = (char)tolower(temp_needle[i]);
    }

    for(int i = 0; i<(strlen(temp_haystack)-1); i++){
        temp_haystack[i] = (char)tolower(temp_haystack[i]);
    }

    r = find_match(temp_haystack,temp_needle,mode);
    free(temp_haystack);
    free(temp_needle);

    return r;
}

// Search function
int search(char *string, char *input, char *output, struct opts options){
    //Input file pointer
    FILE *ifp;
    //Output file pointer
    FILE *ofp;
    int line_num = 1;
    int matches = 0;
    
    char *buffer;

    char *word;

    //Open input file
    //If file fails to open, output message and return 0 for unsuccessful
    //strcmp() returns 0 if the strings are equal.
    if(!strcmp(input,"stdin")){
        ifp=stdin;
    //Else, open file output, if file fails to open, output message and return 0 for unsuccessful
    }else if((ifp = fopen(input, "r")) == NULL){
        printf("Can't open input file: %s\n",input);
        return (-1);
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
        return (-1);
    }else{
        printf("Opened file: %s to write to\n",output);
    }

    // Print the result separator
    printf("\n--RESULTS--\n\n");

    char c;

    buffer = (char*)malloc(BUFF_SIZE);

    // Flag for continuous input
    int cont_input = 1;

    int newline;

    // Declare the result variable outside the loop so it isn't reallocated for every iteration
    int result;

    if(options.mode !=2){
        do{
            c = fscanf(ifp,"%s",buffer);

            if(!strcmp(input,"stdin")){
            // Check if text should be asked for in continuous mode, strcmp returns 0 if the strings match
                if(!strcmp(buffer,"--quit--")){
                    cont_input=0;
                }
            }

            // There's no point in searching if the needle is bigger than the haystack
            if(strlen(buffer)>strlen(string)){
                result = options.match_case?find_match(buffer,string,options.mode):find_match_nocase(buffer,string,options.mode);
            }

            // If a match is found
            if(result){
                // If we're reading from stdin
                if(!strcmp(input,"stdin")){
                // This makes it easier to see tha match on the command line
                    printf("Match:");
                }

                fprintf(ofp,"%s\n",buffer);

                // Increase the number of matches
                matches++;
            }

        }while (c != EOF && cont_input == 1);

    }else if(options.mode == 2){

        word = (char*)malloc(BUFF_SIZE);

        while(fgets(buffer, 512, ifp) != NULL){

            while(sscanf(buffer,"%s%n",word,&newline) == EOF){
                result = options.match_case?find_match(word,string,options.mode):find_match_nocase(word,string,options.mode);

                if(result){
                    // If we're reading from stdin
                    if(!strcmp(input,"stdin")){
                    // This makes it easier to see tha match on the command line
                        printf("Match:");
                    }

                    if(options.output_lines){
                        fprintf(ofp, "Line %d:",line_num);
                    }

                    fprintf(ofp,"%s\n",buffer);

                    // Increase the number of matches
                    matches++;
                }
            }

            line_num++;
        }

        free(word);

    }else{
        printf("Error parsing mode\n");
        return 0;
    }

    free(buffer);


    if(matches == 0){
        fprintf(ofp,"\nNo matches found for %s in %s.\n",string,input);
    }else{
        fprintf(ofp,"\n%d Matches found.\n", matches);
    }

    //Close the files if still open.
    if(ifp){
        fclose(ifp);
    }

    if(ofp && strcmp(output,"stdout")){
        fclose(ofp);
    }

    return matches;
}