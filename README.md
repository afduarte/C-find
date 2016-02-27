Find tool in C
==============

## Building instructions

To build run the command `make`, this will build the tool in release mode.

The available targets are the following:
* `all`
    * Default target, runs the `release` target
* `prepare`
    * Creates the directory `obj/` and creates the .obj files from the .c files in the new directory.
* `debug`
    * Runs the `prepare` target
    * Creates the directory `out/` and compiles **find** with `search.obj` and `rmnewline.obj` to the newly created dir and sets `DEBUG`.
* `test_debug`
    * Runs the `prepare` target
    * Creates the directory `out/` and compiles **find**  with `test_search.obj` and `rmnewline.obj` to the newly created dir and sets `DEBUG` and `TEST`.
* `release`
    * Runs the `prepare` target
      * Creates the directory `out/` and compiles **find** with `search.obj` and `rmnewline.obj` to the newly created dir and sets `RELEASE`.
* `clean`
    * Deletes `out/` and `obj/` and with it all the previously compiled filed.

The makefile is prepared for building with gcc, if you don't have gcc installed, either change the appropriate values in the makefile to compile with your prefered compiler or install gcc.

## Usage: 

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
            2 = line search (e.g. if you search 'hello', you get every whole line where 'hello' appears in the input)
            3 = inclusive search (e.g. if you search 'hell', you get one occurence for each time 'hell' appears even if the whole word is 'hello' in the input)
        NOTE: Forcing the mode might return unexpected results! 
        -h: Show this help screen.
```

## Notes:

* Explicitly forcing the mode might return unexpected results.
* Input can be piped into the program, but it is recommended for best results that the -i and -o flags are used respectively for input and output.
    * If input is to be piped, a search string has to be provided, or else the search string will be whatever is in the first line of the piped input.
        * Something like: 
            * `ps aux | find -o spotify.txt` 
        * Would result in:
            * `No matches found for USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND in stdin.`
* Mode 2 never accounts for multiple occurrences of the same string on the same line. This is because mode two is read line by line as opposed to word by word like the other two modes.


**This project was developed as a coursework for the Programming Fundamentals Module**

**Edinburgh Napier University @ 2016**

**Antero Duarte - Beng Computing**