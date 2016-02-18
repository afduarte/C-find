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

//TODO: htmlify if output to file 
//TODO: Whole word checking not working
//STATUS: loop until segmentation fault because word is never != NULL
//TRY: reading file word by word
//http://cs.smith.edu/~thiebaut/classes/C_Tutor/node126.html

// do {
//       c = fscanf(fp1,"%s",oneword); /* got one word from the file */
//       printf("%s\n",oneword);       /* display it on the monitor  */
//    } while (c != EOF);              /* repeat until EOF           */

// Checks if a passed in(in the form of a pointer) char is a white space character that matters for this use case
// Returns 0 if the char passed in is not a '\t', or a '\n' or a ' '
int is_white(char *ch){
    if(*ch==' '){
        return 1;
    }else if(*ch=='\n'){
        return 2;
    }else if(*ch=='\t'){
        return 3;
    }else{
        return 0;
    }
}

char * find_match(const char *haystack, const char *needle){

    return strstr(haystack,needle);
}

char * find_match_nocase(const char *haystack, const char *needle){
    char * temp;
    char * temp_needle;

    temp=malloc((strlen(haystack)+1)*sizeof(char));
    strcpy(temp,haystack);

    temp_needle=malloc((strlen(needle)+1)*sizeof(char));
    strcpy(temp_needle,needle);
    // printf("TEMP:%s\n",temp );
    // printf("Haystack: %s\tTemp:%s\n",haystack,temp);

    for(int i = 0; i<(strlen(temp_needle)-1); i++){
        temp[i] = tolower(temp_needle[i]);
    }

    for(int i = 0; i<(strlen(temp)-1); i++){
        temp[i] = tolower(temp[i]);
    }

    // printf("After to lower: Haystack: %s\tTemp:%s\n",haystack,temp);

    if(find_match(temp, temp_needle) != NULL){
        char * p = (char*)haystack+(temp-find_match(temp, temp_needle));
        printf("on match: Haystack: %s\tTemp:%s\tPointer:%p\n",haystack,temp,p);
        free(temp);
        free(temp_needle);
        return p;
    }else{
        free(temp);
        return NULL;
    }
}

/*
    ALGORITHM:
        Search function opens input and output files
        Start reading from file, read while fgets != NULL
            Break line into words (strtok())
                strstr on the word, check for whitespace before and after
                print either line or word, with or without line number

 */

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


    buffer = (char*)malloc(BUFF_SIZE);

    int i=0 , l;

    while(fgets(buffer, 512, ifp) != NULL){

        word = options.match_case?find_match(buffer,string):find_match_nocase(buffer,string);

        if(word != NULL){

            if(options.output_lines)
                fprintf(ofp, "Line %d:",line_num);

            fprintf(ofp, "%s\n",word);        
        }


        if(buffer[strlen(buffer)-1]== '\n'){
            line_num++;
        }
    }



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