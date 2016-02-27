 // Multiple inclusion guard
#ifndef SEARCH_H
#define SEARCH_H

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

int find_match(const char *haystack, const char *needle, int mode);

int find_match_nocase(const char *haystack, const char *needle, int mode);

// Search function
int search(char *string, char *input, char *output, struct opts options);

#endif