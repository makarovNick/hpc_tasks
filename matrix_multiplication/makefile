CC=gcc
CCFLAGS=-Wall -O3
NAME = mm_benchmark
LD_LIBRARY_PATH=/opt/OpenBLAS/lib/

all : run

run: main.o matrix.o mm_sse.o strassen.o #blas.o
	$(CC) -o $(NAME) $^ -L$(LD_LIBRARY_PATH) -lopenblas

main.o : src/main.c
	$(CC) $(CCFLAGS) -c $<

matrix.o : src/matrix.c include/matrix.h
	$(CC) $(CCFLAGS) -c $<

mm_sse.o : src/mm_sse.c include/mm_sse.h
	$(CC) $(CCFLAGS) -c $<

strassen.o : src/strassen.c include/strassen.h
	$(CC) $(CCFLAGS) -c $<

blas.o : src/blas.c include/blas.h
	$(CC) $(CCFLAGS) -c -L$(LD_LIBRARY_PATH) -lopenblas $< 

clean: 
	rm *.o  $(NAME) 