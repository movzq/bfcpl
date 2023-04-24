compiler = gcc

main: bfcpl.o assembly.o ustring/ustring.o label.o
	$(compiler) -o bfc bfcpl.o assembly.o ustring/ustring.o label.o
	./bfc tets

rm:
	rm *.o out.s a.out
