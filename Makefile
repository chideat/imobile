itdb: itdb.cpp convert.o itdb.h util.h itdb_zlib.h
	g++ itdb.cpp convert.o itdb.h util.h itdb_zlib.h -lz -o itdb
convert.o: ConvertUTF.h ConvertUTF.cpp
	g++ -c ConvertUTF.cpp -o convert.o
