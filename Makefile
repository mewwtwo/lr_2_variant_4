CXX = /opt/homebrew/bin/g++-16
TBB = $(shell brew --prefix tbb)
INCLUDES = -Iinclude -I$(TBB)/include
LIBS = -L$(TBB)/lib -ltbb -pthread
SRC = $(wildcard src/*.cpp)
HDR = $(wildcard include/*.h)

all: lab_O0 lab_O3

lab_O0: $(SRC) $(HDR)
	$(CXX) -std=c++20 -O0 $(INCLUDES) $(SRC) -o lab_O0 $(LIBS)

lab_O3: $(SRC) $(HDR)
	$(CXX) -std=c++20 -O3 $(INCLUDES) $(SRC) -o lab_O3 $(LIBS)

run: all
	mkdir -p results
	./lab_O0 > results/output_O0.txt
	./lab_O3 > results/output_O3.txt

clean:
	rm -f lab_O0 lab_O3
