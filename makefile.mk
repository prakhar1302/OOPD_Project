all: compile link

compile: main1.cpp main3.cpp main4_1.cpp main4_2.cpp
	g++ -c main1.cpp
	g++ -c main3.cpp
	g++ -c main4_1.cpp
	g++ -c main4_2.cpp

link: 
	g++ -o main1 main1.o
	g++ -o main2 main3.o
	g++ -o main3 main4_1.o
	g++ -o main4 main4_2.o

run:
	./main1
	./main3
	./main4_1
	./main4_2

clean: 
	rm -f main1.o
	rm -f main3.o
	rm -f main4_1.o
	rm -f main4_2.o