#makefile for building the serialize test

all: 
	opcpp -gd generated/Serialize/ -ohd ./ -doh dialect.doh -verbose -globmode -nodebug
	g++ main.cpp -o ./bin/serializegcc
	icpc main.cpp -o ./bin/serializeicc -static
