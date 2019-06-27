all : temp
	gnuplot plot.plt
temp : run
	./run 0 4.7124 > temp
run : NumInt.c NumInt.h test.c
	gcc -lm NumInt.h NumInt.c test.c -o run
clean :
	rm temp run pic.svg
