.PHONY: clean build_and_run 

clean:
	rm -rf out

build_and_run:
	@mkdir -p out
	gcc -o out/stranger_things src/*.c
	out/stranger_things
