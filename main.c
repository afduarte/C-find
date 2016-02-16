//Defining _GNU_SOURCE allows us to use strcasestr() to do case insensitive search on a string, because it is a nonstandard
#define _GNU_SOURCE
#include <string.h>

// Include stdio.h(Standard Input/Output)
#include <stdio.h>
// Helper function
#include <stdlib.h>

//include newline and search functions
#include "rmnewline.h"

#ifdef TEST
#include "test_search.h"
#endif

#ifndef TEST
#include "test1_search.h"
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
    printf("Usage: search [-s string] [-i input_file] [-o output_file] [-c] [-l] [-m 1-3]\n");
    printf("\t-s string: String to search for. If not passed in, user will be prompted for one.\n");
    printf("\t-i input_file: File containing the text to search in. If not passed in, user will be prompted for a text block.\n");
    printf("\t-o output_file: File to output to. If not passed in, output will be shown on the screen\n");
    printf("\t-c: Matching case. If passed, will only find matches where the case corresponds the one in the string to search for.\n");
    printf("\t-l: Line numbers. If passed, will output the number of the line where the match was found.\n");
    printf("\t-m: Mode. If passed, will select the search mode in one of the three:\n");
    printf("\t\t1 = literal search (e.g. if you search 'hello', you get one occurence for each time 'hello' appears in the input)\n");
    printf("\t\t2 = line search (e.g. if you search 'hello', you get every whole line where 'hello' appears in the input`)\n");
    printf("\t\t3 = inclusive search (e.g. if you search 'hell', you get one occurence for each time 'hell' appears even if the whole word is 'hello' in the input)\n");
    printf("\t-h: Show this help screen.\n");
}

int main(int argc, char **argv){

    // Declarations

    struct io state = {0,0,0};
    struct opts options = {0,1,0};

    //if(DEBUG) printf("State:\n \t--input:%d, \n\t--output:%d\n",state.input,state.output);

    //if(DEBUG) printf("Options:\n \t--mode:%d, \n\t--case:%d, \n\t--line nrs:%d\n",options.mode,options.match_case,options.output_lines);

    // File to search in
    char *input_file;
    
    // File to output to
    char *output_file;

    // Parse first user argument as string to search for
    char *search_string;
    

    // Loop through arguments to get passed in options
    // Start on argv[1] because [0] is program name
        //if(DEBUG) printf("Given %d arguments\n",argc);

        for(int i=1;i<=argc-1;i++){
            //if(DEBUG) printf("Loop: %d\n",i );
        // Parse arguments
            if(argv[i][0]=='-' && strlen(argv[i])==2){

                //if(DEBUG) printf("found '-'\n");

                if(argv[i][1]=='h'){
                    print_usage();
                    return 0;

                }else if(argv[i][1]=='s'){

                    //if(DEBUG) printf("found 's'\n");

                    // if "-i" change input_file to filename passed in
                    search_string = (char*)malloc(strlen(argv[i+1]));
                    strcpy(search_string,argv[i+1]);
                    printf("--%s\n",search_string);
                    state.string = 1;

                }else if(argv[i][1]=='i'){

                    //if(DEBUG) printf("found 'i'\n");

                    // if "-i" change input_file to filename passed in
                    input_file = (char*)malloc(strlen(argv[i+1]));
                    strcpy(input_file,argv[i+1]);
                    printf("--%s\n",input_file);
                    state.input = 1;

                }else if(argv[i][1]=='o'){

                    //if(DEBUG) printf("found 'o'\n");

                    // if "-o" change output_file to filename passed in
                    output_file = (char*)malloc(strlen(argv[i+1]));
                    strcpy(output_file,argv[i+1]);
                    printf("--%s\n",output_file);
                    state.output = 1;

                }else if(argv[i][1]=='c'){

                    //if(DEBUG) printf("found 'c'\n");

                    // Case "-c" change match_case to 0
                    options.match_case=0;
                }else if(argv[i][1]=='l'){

                    //if(DEBUG) printf("found 'l'\n");

                    // Case "-l" output line number with the occurrence to the output file
                    options.output_lines=1;
                }else if(argv[i][1]=='m'){

                    //if(DEBUG) printf("found 'm'\n");

                    // Case "-m" record mode
                    options.mode=atoi(argv[i+1]);
                    printf("--%d\n", options.mode );
                }else{
                    printf("--Error parsing arguments.--\n");
                    print_usage();
                    return 0;
                }
            }
        }

        if(!state.string){
            char search_string_buffer[100];
            printf("No search string specified, enter the text you want to look for: \n");
            fgets(search_string_buffer, 100, stdin);
            remove_newline(search_string_buffer);
            search_string = (char*)malloc(strlen(search_string_buffer)+2);
            strcpy(search_string,search_string_buffer);
        }

        //TODO: copy input_text_buffer to input_file, use malloc(size of text given
        if(!state.input){
            char input_text_buffer[1024];
            printf("No input file specified, enter the text you want to search in: \n");
            fgets(input_text_buffer, 1024, stdin);
            remove_newline(input_text_buffer);
            input_file = (char*)malloc(strlen(input_text_buffer)+1);
            strcpy(input_file,input_text_buffer);
        }

        if(!state.output){
            printf("No output file specified, printing to stdout: \n");
            output_file = (char*)malloc(7);
            strcpy(output_file,"stdout\0");

            //if(DEBUG) printf("Output file: %s\n",output_file);
        }

        if(!options.mode){
            printf("No mode specified, pick one: [1-3] \n");
            char mode_buffer[3];
            fgets(mode_buffer, 3, stdin);
            remove_newline(mode_buffer);
            options.mode = atoi(mode_buffer);
            
            //if(DEBUG) printf("Mode: %d\n",options.mode);
        }

        search(search_string,input_file,output_file,options)?printf("Search successful! Exiting...\n"):printf("Search unsuccessful! Exiting...\n");;

        free(search_string);
        free(input_file);
        free(output_file);


        return 0;

    }