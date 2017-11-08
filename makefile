all: driver.exe

driver.exe: Builder.o Parser.o ConfigLoader.o driver.cpp
	g++ -std=c++11 Builder.o Parser.o ConfigLoader.o driver.cpp -o driver.exe

Builder.o: util.h Builder/Builder.h Builder/Builder.cpp
	g++ -std=c++11 -c Builder/Builder.cpp

Parser.o: util.h Parser/Parser.h Parser/Parser.cpp
	g++ -std=c++11 -c Parser/Parser.cpp

ConfigLoader.o: util.h ConfigLoader/ConfigLoader.h ConfigLoader/ConfigLoader.cpp
	g++ -std=c++11 -c ConfigLoader/ConfigLoader.cpp

run:
	./driver.exe

runl:
	./driver.exe
	cat log.txt

mem:
	valgrind --leak-check=full ./driver.exe

clean:
	rm *.o
	rm driver.exe

del:
	rm -r Title/*
	rmdir Title/
