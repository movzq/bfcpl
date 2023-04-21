compiler = gcc

main: bfcpl.o gen.o ustring/ustring.o temp.o
	$(compiler) -o bfcpl bfcpl.o gen.o ustring/ustring.o temp.o
	./bfcpl bfcodes/*

rm:
	rm *.o bfcpl out.s rm a.out
