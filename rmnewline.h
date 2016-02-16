#pragma once
// Function to remove newline character from input
void remove_newline(char *str){
    // Get string length
    int len = strlen(str);
    // Check last character
    if(len > 0 && str[len - 1] == '\n')
        // If so, make it a null terminator
        str[len - 1] = '\0';
}