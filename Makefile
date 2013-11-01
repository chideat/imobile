itdb: main.cpp convert.o parse.o
	g++ main.cpp convert.o parse.o -lz -o itdb
convert.o: ConvertUTF.h ConvertUTF.cpp
	g++ -c ConvertUTF.cpp -o convert.o
parse.o: ringtone-parse.cpp ringtone-parse.h
	g++ -c ringtone-parse.cpp -o parse.o


rm:
	rm itdb parse.o convert.o
