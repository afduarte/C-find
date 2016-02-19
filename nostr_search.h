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

struct result{
    // Variable to store the result, either 0 or 1
    int result;
    // Flag to indicate where the first match happened
    int first_match;
};

//STATUS: search working for whole word, going to implement modes now
//TODO: Implement inclusive search with strstr

struct result find_match(const char *haystack, const char *needle, int mode){

    // Returns a struct that holds the result (1 or 0) and an int that represents the offset from the first character where the first match happened
    // The char where the first match happened should only be accounted for if the result is 1

    struct result r = {0,0};
    // j keeps track of how many characters have been checked
    int j = 0;
    // matches keeps track of how many matches were found
    int matches = 0;

    if(mode==3){
        if((strstr(haystack,needle)) != NULL){
            r.result = 1;
            r.first_match = (int)(haystack-needle);
            return r;
        }
    }


    for(int i =0;i<strlen(haystack);i++){
        // Guard to not let the function look past the size of "needle"
        if(j>strlen(needle)-1){
            r.result=0;
            return r;
        }else if(haystack[i]==needle[j]){
            j++;

            //Checks if first_match has been changed from 0
            //If it hasn't, assign the value of i to it, so the function returns the offset to where the match occurred
            if(!r.first_match){
                r.first_match = i;
                // Increment the number of matches
            }

            matches++;

        }else{
            //Just increment j to keep looking
            j++;
        }
    }

    r.result=matches==j?1:0;

    // Effectively, if we're meant to find "hell" in "hello", this function will return {1,0}, meaning if found hell starting at char 0 of hello
    // On the other hand, if we're looking for "abc" in "abfghc", it will return {0,0}, meaning it did find the a for "abc" on char 0 of "abfghc", but


    return r;

    
}

struct result find_match_nocase(const char *haystack, const char *needle, int mode){
    char *temp_haystack;
    char *temp_needle;
    struct result r = {0,0};

    temp_haystack=malloc((strlen(haystack))*sizeof(char));
    strcpy(temp_haystack,haystack);

    temp_needle=malloc((strlen(needle))*sizeof(char));
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
        printf("Can't open output file: %s\n",output);
        return 0;
    }

    // Print the result separator
    printf("\n--RESULTS--\n\n");



    // FILE *fp1;
    char *oneword;
    char c;
    struct result r;

    oneword = malloc(BUFF_SIZE);

    // fp1 = fopen("TENLINES.TXT","r");

    do {
        c = fscanf(ifp,"%s",oneword);

        struct result r;



        r = options.match_case?find_match(oneword,string,options.mode):find_match_nocase(oneword,string,options.mode);

        if(r.result){
            fprintf(ofp,"%s\n",oneword);
        }

    } while (c != EOF);             




    // free(buffer);

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