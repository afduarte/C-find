// Include stdio.h(Standard Input/Output)
#include <stdio.h>

// Include the header file
#include "search.h"

// Test search function
int search(char *string, char *input, char *output, struct opts options){
    if(options.match_case && options.output_lines)
        printf("Search for %s in file %s, output to file %s, mode: %d, case sensitive and print lines\n",string,input,output,options.mode);
    else if(!options.match_case && options.output_lines)
        printf("Search for %s in file %s, output to file %s, mode: %d, case insensitive and print lines\n",string,input,output,options.mode);
    else if (options.match_case && !options.output_lines)
        printf("Search for %s in file %s, output to file %s, mode: %d, case sensitive and don't print lines\n",string,input,output,options.mode);
    else if (!options.match_case && !options.output_lines)
        printf("Search for %s in file %s, output to file %s, mode: %d, case insensitive and don't print lines\n",string,input,output,options.mode);
    else{
    	printf("Error parsing options\n");
        return 0;
    }

    return 1;
}