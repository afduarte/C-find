#pragma once

struct opts{
    // Mode: 
    //  1 = literal search (e.g. if you search "hello", you get one occurence for each time "hello" appears in the output file)
    //  2 = line search (e.g. if you search "hello", you get every whole line where "hello" appears in the output file)
    //  3 = inclusive search (e.g. if you search "hell", you get one occurence for each time "hell" appears even if the whole world is "hello" in the output file)
    int mode;
    // Match case if set to 1
    int match_case;
    // Output line numbers if set to 1
    int output_lines;
};

// Test search function
void search(char *string, char *input, char *output, struct opts options){
    if(options.match_case && options.output_lines)
        printf("Search for %s in file %s, output to file %s, mode: %d, case sensitive and print lines\n",string,input,output,options.mode);
    else if(!options.match_case && options.output_lines)
        printf("Search for %s in file %s, output to file %s, mode: %d, case insensitive and print lines\n",string,input,output,options.mode);
    else if (options.match_case && !options.output_lines)
        printf("Search for %s in file %s, output to file %s, mode: %d, case sensitive and don't print lines\n",string,input,output,options.mode);
    else if (!options.match_case && !options.output_lines)
        printf("Search for %s in file %s, output to file %s, mode: %d, case insensitive and don't print lines\n",string,input,output,options.mode);
    else
    	printf("Error parsing options\n");
}