Find tool in C
==============

Usage: 
```
find [string or -s string] [-i input_file] [-o output_file] [-c] [-l] [-m 1-3]
    -v version: Output the version of the program and exit
    -s string: String to search for. If not passed in, user will be prompted for one.
    -i input_file: File containing the text to search in. If not passed in, user will be prompted for text until --quit-- is entered. Input can be piped in
    -o output_file: File to output to. If not passed in, output will be shown on the screen
    -c: Matching case. If passed, will only find matches where the case corresponds the one in the string to search for.
    -l: Line numbers. If passed, will output the number of the line where the match was found.
    -m: Mode. If passed, will force the search mode in one of the three listed below. If not, will be decided automatically:
        1 = literal search (e.g. if you search 'hello', you get one occurence for each time 'hello' appears in the input)
        2 = line search (e.g. if you search 'hello', you get every whole line where 'hello' appears in the input`)
        3 = inclusive search (e.g. if you search 'hell', you get one occurence for each time 'hell' appears even if the whole word is 'hello' in the input)
        NOTE: Forcing the mode might return unexpected results! 
    -h: Show this help screen.
```