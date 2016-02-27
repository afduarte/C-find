// Include stdio.h(Standard Input/Output)
#include <stdio.h>

// Libraries that include helper functions
#include <string.h>
#include <stdlib.h>

// Using tolwr from ctype
#include <ctype.h>

// Include the header file
#include "search.h"

#define BUFF_SIZE  512*sizeof(char)

int find_match(const char *haystack, const char *needle, int mode){

    // Prevents error on freeing the memory space at the end of the function when the last line of the line was passed in for the search
    // Due to the pointer being NULL. Even if it is not for this case, a NULL check can prevent other problems
    if(haystack != NULL && needle != NULL){

        // j keeps track of the index of the character being checked
        int j = 0;

        // If we're not using mode one, use strstr()
        if(mode>=2){
            if((strstr(haystack,needle)) != NULL){
                return 1;
            }else{
                return 0;
            }
        // Else do the simpler whole word check
        }else if(mode == 1){

            for(int i =0;i<strlen(haystack);i++){
                if(haystack[i]==needle[j]){
                    j++;
                }else{
                    return 0;
                }
            }

            return 1;

        }
    }
}

int find_match_nocase(const char *haystack, const char *needle, int mode){
    // Prevents error on freeing the memory space at the end of the function when the last line of the line was passed in for the search
    // Due to the pointer being NULL. Even if it is not for this case, a NULL check can prevent other problems
    if(haystack != NULL && needle != NULL){
        char *temp_haystack;
        char *temp_needle;
        int r;

        temp_haystack=(char*)malloc((strlen(haystack)+1)*sizeof(char));
        strcpy(temp_haystack,haystack);

        temp_needle=(char*)malloc((strlen(needle)+1)*sizeof(char));
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

    }else{
        return 0;
    }
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
        // Print the result separator if we're writing to stdout
        printf("\n--RESULTS--\n\n");
    //Else, open file output, if file fails to open, output message and return 0 for unsuccessful
    }else if((ofp=fopen(output, "w"))==NULL){
        printf("Can't open output file: %s\n",output);
        return (-1);
    }else{
        printf("Opened file: %s to write to\n",output);
    }

    char c;

    buffer = (char*)malloc(BUFF_SIZE);

    // Flag for continuous input
    int cont_input = 1;

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
            if(strlen(buffer)>=strlen(string)){

                result = options.match_case?find_match(buffer,string,options.mode):find_match_nocase(buffer,string,options.mode);


                // If a match is found
                if(result){
                    // If we're reading from stdin
                    if(!strcmp(input,"stdin") && !strcmp(output,"stdout")){
                        // This makes it easier to see the match on the command line when input is stdin and output is stdout
                        printf("Match:");
                    }

                    fprintf(ofp,"%s\n",buffer);

                    // Increase the number of matches
                    matches++;
                }
            }

        }while (c != EOF && cont_input == 1);

    }else if(options.mode == 2){

        while(fgets(buffer, 512, ifp) != NULL){

            if(strlen(buffer)>=strlen(string)){

                result = options.match_case?find_match(buffer,string,options.mode):find_match_nocase(buffer,string,options.mode);

                if(result){
                    // If we're reading from stdin
                    if(!strcmp(input,"stdin") && !strcmp(output,"stdout")){
                        // This makes it easier to see the match on the command line when input is stdin and output is stdout
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


    }else{
        printf("Error parsing mode\n");
        return 0;
    }

    free(buffer);


    if(matches == 0){
        fprintf(ofp,"\nNo matches found for %s in %s.\n",string,input);
    }else{
        fprintf(ofp,"\nMatches found: %d\n", matches);
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