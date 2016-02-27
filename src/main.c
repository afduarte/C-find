// Include stdio.h(Standard Input/Output)
#include <stdio.h>
// Libraries that include helper functions
#include <string.h>
#include <stdlib.h>

//include newline and search functions
#include "rmnewline.h"

#if defined(RELEASE) || defined(DEBUG)
#include "search.h"
#endif



struct io{
    // Input file given?
    int input;
    // Output file given?
    int output;
    // Search string given?
    int string;
};

void print_usage(){
    printf("Usage: find [string or -s string] [-i input_file] [-o output_file] [-c] [-l] [-m 1-3]\n");
    printf("\t-v version: Output the version of the program and exit\n");
    printf("\t-s string: String to search for. If not passed in, user will be prompted for one.\n");
    printf("\t-i input_file: File containing the text to search in. If not passed in, user will be prompted for text until --quit-- is entered. Input can be piped in\n");
    printf("\t-o output_file: File to output to. If not passed in, output will be shown on the screen\n");
    printf("\t-c: Matching case. If passed, will only find matches where the case corresponds the one in the string to search for.\n");
    printf("\t-l: Line numbers. If passed, will output the number of the line where the match was found.\n");
    printf("\t-m: Mode. If passed, will force the search mode in one of the three listed below. If not, will be decided automatically:\n");
    printf("\t\t1 = literal search (e.g. if you search 'hello', you get one occurence for each time 'hello' appears in the input)\n");
    printf("\t\t2 = line search (e.g. if you search 'hello', you get every whole line where 'hello' appears in the input`)\n");
    printf("\t\t3 = inclusive search (e.g. if you search 'hell', you get one occurence for each time 'hell' appears even if the whole word is 'hello' in the input)\n");
    printf("\tNOTE: Forcing the mode might return unexpected results! \n");
    printf("\t-h: Show this help screen.\n");
}

int has_space(const char *haystack){
    for(int i = 0; i<(strlen(haystack)-1); i++){
        if(haystack[i] == ' ')
            return 1;
    }
    return 0;
}

int main(int argc, char **argv){

    // Declarations
    struct io state = {0,0,0};
    struct opts options = {0,1,0};

    // File to search in
    char *input_file;
    
    // File to output to
    char *output_file;

    // Parse first user argument as string to search for
    char *search_string;
    

    // Loop through arguments to get passed in options
    // Start on argv[1] because [0] is program name
    #ifdef DEBUG
    printf("Arguments Passed in:\n");
    #endif
    
    // This if check prevents a segmentation fault when the program is used like this: find test
    if(argc>2){
        for(int i=1;i<=argc-1;i++){
        // Parse arguments
        #ifdef DEBUG
            printf("\t--ARGUMENT %d: \"%s\"\n",i,argv[i]);
        #endif

            if(argv[i][0]=='-' && strlen(argv[i])==2){
                switch(argv[i][1]){

                    case 'v' :
                    printf("find tool -- v 1.0.0\n");
                    printf("Antero Duarte@2016 -- http://github.com/afduarte\n");
                    printf("Edinburgh Napier University, Programming Fundamentals\n");
                    exit(EXIT_SUCCESS);
                    break;

                    case 'h' :
                    print_usage();
                    exit(EXIT_SUCCESS);
                    break;

                    case 's' :
                    // if "-i" change input_file to filename passed in
                    search_string = (char*)malloc(strlen(argv[i+1]));
                    strcpy(search_string,argv[i+1]);
                    state.string = 1;

                    #ifdef DEBUG
                    printf("\t--SEARCH STRING (-s): \"%s\"\n",search_string);
                    #endif
                    break;

                    case 'i' :
                    // if "-i" change input_file to filename passed in
                    input_file = (char*)malloc(strlen(argv[i+1]));
                    strcpy(input_file,argv[i+1]);
                    state.input = 1;

                    #ifdef DEBUG
                    printf("\t--INPUT FILE: %s\n",input_file);
                    #endif
                    break;

                    case 'o' :
                    // if "-o" change output_file to filename passed in
                    output_file = (char*)malloc(strlen(argv[i+1]));
                    strcpy(output_file,argv[i+1]);
                    state.output = 1;

                    #ifdef DEBUG
                    printf("\t--OUTPUT FILE: %s\n",output_file);
                    #endif
                    break;

                    case 'c' :
                    // Case "-c" change match_case to 0
                    options.match_case=0;

                    #ifdef DEBUG
                    printf("\t--CASE INSENSITIVE\n");
                    #endif
                    break;

                    case 'l' :
                    // Case "-l" output line number with the occurrence to the output file
                    options.output_lines=1;

                    #ifdef DEBUG
                    printf("\t--OUTPUT LINE NUMBERS\n");
                    #endif
                    break;

                    case 'm' :
                    // Case "-m" record mode
                    options.mode=atoi(argv[i+1]);

                    #ifdef DEBUG
                    printf("\t--FORCED MODE: %d\n",options.mode);
                    #endif
                    break;

                    default:
                    printf("--Error parsing arguments.--\n");
                    print_usage();
                    exit(EXIT_FAILURE);
                    break;
                }
            }
        }
    }




    if(!state.string){

        #ifdef DEBUG
        printf("STRING WAS NOT PASSED IN WITH -s, CHECKING IF WAS PASSED IN WITHOUT IT\n");
        #endif

        // Check if string was passed in without '-s' and is more than 1 word
        if(argc>2 && argv[1][0] != '-'){
            int i = 1;
            search_string = (char*)malloc((strlen(argv[i])+1)*sizeof(char));
            while(i<=argc-1 && argv[i][0] != '-'){
                search_string = realloc(search_string,(sizeof(*search_string)+strlen(argv[i])+1)*sizeof(char));
                // If it's not the first word, prepend a space
                if(i != 1){
                    strcat(search_string, " ");
                }
                strcat(search_string,argv[i]);
                i++;
            }

        // Most likely the string is only one word
        }else if(argc==2){
            if(argv[1][0] != '-'){
                search_string = (char*)malloc((strlen(argv[1])+1)*sizeof(char));
                strcpy(search_string,argv[1]);
            }

        }else{
            char search_string_buffer[100];
            printf("No search string specified, enter the text you want to look for: \n");
            fgets(search_string_buffer, 100, stdin);
            remove_newline(search_string_buffer);
            search_string = (char*)malloc((strlen(search_string_buffer)+2)*sizeof(char));
            strcpy(search_string,search_string_buffer);
        }
    }


    if(!state.input){
        input_file = (char*)malloc(6);
        strcpy(input_file,"stdin\0");
    }

    if(!state.output){
        printf("No output file specified, printing to stdout: \n");
        output_file = (char*)malloc(7);
        strcpy(output_file,"stdout\0");

    }

    if(!options.mode){
        if(has_space(search_string) || options.output_lines){
            // If the search string is multi word or we want the line numbers, make it mode 2, or else it won't work
            options.mode = 2;
        }else if(strlen(search_string)>20){
            // If the search string is bigger than 20 characters long, use mode 3 because it is faster
            options.mode = 3;
        }else{
            // Fallback to mode 1 for most strings
            options.mode = 1;
        }

        #ifdef DEBUG
        printf("\t--AUTO MODE: %d\n",options.mode);
        #endif

    }

    if(search(search_string,input_file,output_file,options) != (-1) ){
        printf("Search successful! Exiting...\n");
    }else{
        printf("Search unsuccessful! Exiting...\n");
    }

    free(search_string);
    free(input_file);
    free(output_file);

    return 0;

}