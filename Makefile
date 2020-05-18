all : unit_test speed_test total_test
lib : libtimer.a

libtimer.a:
	tar jxvf libtimer-1.00.tar.bz2
	cd libtimer-1.00/; ./configure; make

unit_test :
	gcc -O3 -o unit_test -I./src -I./test test/unit_test.c test/check_error.c src/ddot.c src/fast_ddot.c

speed_test : 
	gcc -O3 -o speed_test -I./src -I./test -I./libtimer-1.00/src test/speed_test.c test/check_speed.c test/check_error.c src/ddot.c src/fast_ddot.c -L./libtimer-1.00/src -ltimer

total_test :
	gcc -O3 -o total_test -I./src -I./test -I./libtimer-1.00/src test/total_test.c test/check_speed.c test/check_error.c src/ddot.c src/fast_ddot.c -L./libtimer-1.00/src -ltimer

	
.PHONY: clean distclean
clean :
	rm -f unit_test speed_test total_test

distclean : clean
	rm -f -r libtimer-1.00/ */*.o
