test_int : test.c
	gcc -g test.c -o test_int
test_string: test_htable_string.c
	gcc -g test_htable_string.c  -o test_string
test_struct :test_htable_struct.c
	gcc -g test_htable_struct.c -o test_struct
clean:
	rm test_int test_struct test_string
