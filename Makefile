all: src/linear.o src/multithread.o src/multiprocess.o

src/linear.o:
	gcc -o src/linear.o src/linear.c src/imageprocessing.c -I./ -lfreeimage
src/multithread.o:
	gcc -o src/multithread.o src/multithread.c src/imageprocessing.c -I./ -lfreeimage -lpthread
src/multiprocess.o:
	gcc -o src/multiprocess.o src/multiprocess.c src/imageprocessing.c -I/ -lfreeimage

clean:
	rm src/multiprocess.o src/multithread.o src/linear.o

