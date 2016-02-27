all: release

prepare:
	mkdir -p obj
	gcc -c src/search.c -o obj/search.o -std=c99
	gcc -c src/rmnewline.c -o obj/rmnewline.o -std=c99
	gcc -c src/test_search.c -o obj/test_search.o -std=c99


debug: prepare
	mkdir -p out
	gcc src/main.c obj/search.o obj/rmnewline.o -o out/find -std=c99 -D DEBUG

test_debug: prepare
	mkdir -p out
	gcc src/main.c obj/test_search.o obj/rmnewline.o -o out/find -std=c99 -D TEST -D DEBUG

release: prepare
	mkdir -p out
	gcc src/main.c obj/search.o obj/rmnewline.o -o out/find -std=c99 -D RELEASE

clean:
	rm -r out
	rm -r obj