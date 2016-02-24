all: release

debug:
	mkdir -p out
	gcc main.c -o out/find -std=c99 -D DEBUG

test_debug:
	mkdir -p out
	gcc main.c -o out/find -std=c99 -D TEST -D DEBUG

release:
	mkdir -p out
	gcc main.c -o out/find -std=c99 -D RELEASE

clean:
	rm -r out