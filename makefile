debug:
	mkdir -p out
	gcc main.c -o out/coursework -std=c99 -D DEBUG

test_debug:
	mkdir -p out
	gcc main.c -o out/coursework -std=c99 -D TEST -D DEBUG