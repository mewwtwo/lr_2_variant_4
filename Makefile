ifeq ($(OS),Windows_NT)
    CXX = g++
    EXE = .exe
else
    CXX = g++-16
    TBB = $(shell brew --prefix tbb)
    TBB_FLAGS = -I$(TBB)/include -L$(TBB)/lib
endif

FILES = $(wildcard src/*.cpp include/*.h)
FLAGS = -std=c++20 -Iinclude $(TBB_FLAGS) src/*.cpp -ltbb -pthread

all: lab_O0$(EXE) lab_O3$(EXE)

lab_O0$(EXE): $(FILES)
	$(CXX) -O0 $(FLAGS) -o $@

lab_O3$(EXE): $(FILES)
	$(CXX) -O3 $(FLAGS) -o $@

run: all
	mkdir -p results
	./lab_O0$(EXE) > results/output_O0.txt
	./lab_O3$(EXE) > results/output_O3.txt

clean:
	rm -f lab_O0$(EXE) lab_O3$(EXE)
