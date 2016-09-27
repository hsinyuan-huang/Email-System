mail_searcher: main.cpp pagerank.h dataST.h dataST2.h sha1.cpp sha1.h
	g++ -std=c++0x -Wall -Wshadow sha1.cpp main.cpp -o mail_searcher

run: mail_searcher
	./mail_searcher < input > output

runstd: mail_searcher
	./mail_searcher
