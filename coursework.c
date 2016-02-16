// Include stdio.h(Standard Input/Output)
#include <stdio.h>
// Helper function
#include <string.h>
#include <stdlib.h>


struct io{
    int input;
    int output;
};


// Test search function
void search(char *string, char *input, char *output, int mode, int match_case, int print_lines){
    if(match_case && print_lines)
        printf("Search for %s in file %s, output to file %s, mode: %d, case sensitive and print lines\n",string,input,output,mode);
    else if(!match_case && print_lines)
        printf("Search for %s in file %s, output to file %s, mode: %d, case insensitive and print lines\n",string,input,output,mode);
    else if (match_case && !print_lines)
        printf("Search for %s in file %s, output to file %s, mode: %d, case sensitive and don't print lines\n",string,input,output,mode);
    else if (!match_case && !print_lines)
        printf("Search for %s in file %s, output to file %s, mode: %d, case insensitive and don't print lines\n",string,input,output,mode);
}

// Function to remove newline character from input
void remove_newline(char *str){
    // Get string length
    int len = strlen(str);
    // Check last character
    if(len > 0 && str[len - 1] == '\n')
        // If so, make it a null terminator
        str[len - 1] = '\0';
}



int main(int argc, char **argv){

    // Declarations

    struct io state = {0,0};

    // File to search in
    char *input_file;
    
    // File to output to
    char *output_file;
    // Mode: 
    //  1 = literal search (e.g. if you search "hello", you get one occurence for each time "hello" appears in the output file)
    //  2 = line search (e.g. if you search "hello", you get every whole line where "hello" appears in the output file)
    //  3 = inclusive search (e.g. if you search "hell", you get one occurence for each time "hell" appears even if the whole world is "hello" in the output file)
    int mode=0;
    int match_case=1;
    int output_lines=0;


    // Parse first user argument as string to search for
    char *search_string;
    

    if(argc>=2){
        //search_string = malloc(len(argv[1]));
        strcpy(search_string,argv[1]);
        printf("Got search from arguments: %s\n",search_string);
    }

    if(strcmp(search_string,argv[1])){
        // Prompt for search string
        char search_buffer[50];
        printf("Input search string:\n");
        fgets(search_buffer, 50, stdin);
        remove_newline(search_buffer);
        printf("Got search from stdin:%s\n",search_buffer);
        strcpy(search_string,search_buffer);
    }

    // Loop through arguments to get passed in options
    // Start on argv[1] because [0] is program name
    printf("Given %d arguments\n",argc);

    for(int i=2;i<=argc-1;i++){
        printf("Loop: %d\n",i-1 );
        // Parse arguments
        if(argv[i][0]=='-'){
            printf("found '-'\n");
            
             if(argv[i][1]=='i'){
                    printf("found 'i'\n");
                    // if "-i" change input_file to filename passed in
                    //input_file = malloc(len(argv[i+1]));
                    strcpy(input_file,argv[i+1]);
                    printf("--%s\n",input_file);
                    state.input = 1;

                }else if(argv[i][1]=='o'){
                    printf("found 'o'\n");
                    // if "-o" change output_file to filename passed in
                    //output_file = malloc(len(argv[i+1]));
                    strcpy(output_file,argv[i+1]);
                    printf("--%s\n",output_file);
                    state.output = 1;

                }else if(argv[i][1]=='c'){
                    printf("found 'c'\n");
                    // Case "-c" change match_case to 0
                    match_case=0;
                }else if(argv[i][1]=='l'){
                    printf("found 'l'\n");
                    // Case "-l" output line number with the occurrence to the output file
                    output_lines=1;
                }else if(argv[i][1]=='m'){
                    printf("found 'm'\n");
                    // Case "-m" record mode
                    mode=atoi(argv[i+1]);
                    printf("--%d\n", mode );
                }else{
                    continue;
                }
        }
    }
    
    char input_text_buffer[1024];
    
    if(!state.input){
        printf("No input file specified, enter the text you want to search in: \n");
        fgets(input_text_buffer, 1024, stdin);
        remove_newline(input_text_buffer);
    }

    if(!state.output){
        printf("No output file specified, printing to stdout: \n");
        strcpy(output_file,"stdout");
    }

    if(state.input)
        search(search_string,input_file,output_file,mode,match_case,output_lines);
    else
        search(search_string,input_text_buffer,output_file,mode,match_case,output_lines);


    return 0;
    
}