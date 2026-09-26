CXX = /opt/homebrew/bin/g++-16
TBB = $(shell brew --prefix tbb)
FLAGS = -std=c++20 -I$(TBB)/include -L$(TBB)/lib -ltbb -pthread

all: generate lab_O0 lab_O3

generate: generate.cpp
	$(CXX) $(FLAGS) -O2 generate.cpp -o generate

lab_O0: lab.cpp
	$(CXX) $(FLAGS) -O0 lab.cpp -o lab_O0

lab_O3: lab.cpp
	$(CXX) $(FLAGS) -O3 lab.cpp -o lab_O3

run: all
	./generate
	./lab_O0 > results/output_O0.txt
	./lab_O3 > results/output_O3.txt

clean:
	rm -f generate lab_O0 lab_O3
