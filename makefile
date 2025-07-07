run : test.c
	gcc -g test.c htable.c -o test

clean:
	rm run
