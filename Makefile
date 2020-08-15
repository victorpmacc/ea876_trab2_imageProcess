all: src/linear.o src/multiprocess.o src/multithread.o

test: versao1 versao2 versao3 src/graficos_tempo.py
	python3 src/graficos_tempo.py
	
versao1: src/linear.o
	src/linear.o
src/linear.o:
	gcc -o src/linear.o src/linear.c src/imageprocessing.c -I./ -lfreeimage

versao2: src/multiprocess.o
	src/multiprocess.o
src/multiprocess.o:
	gcc -o src/multiprocess.o src/multiprocess.c src/imageprocessing.c -I/ -lfreeimage

versao3: src/multithread.o
	src/multithread.o
src/multithread.o:
	gcc -o src/multithread.o src/multithread.c src/imageprocessing.c -I./ -lfreeimage -lpthread

clean:
	rm src/multiprocess.o src/multithread.o src/linear.o doc/tempos_linear.txt doc/tempos_multiprocess.txt doc/tempos_multithread.txt out/cachorro-out_linear.jpg out/cachorro-out-multiprocess.jpg out/cachorro-out-multithread.jpg

