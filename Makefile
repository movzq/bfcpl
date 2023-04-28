compiler = g++

main: bfcpl.o asm.o
	$(compiler) -o bfc bfcpl.o asm.o
	./bfc test

rm:
	rm *.o bfc out.s a.out

asm:
	cc out.s
	./a.out
