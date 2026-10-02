// GCC (Homebrew g++-16), C++20
// Dmytro Demchuk K-25


#include <iostream>
#include <string>
#include <thread>

#include "DataSet.h"
#include "Experiment.h"

int main(int argc, char **argv) {
    bool forceNew = false;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--new")
            forceNew = true;
    }

    std::cout << "hardware_concurrency = " << std::thread::hardware_concurrency() << "\n";

    DataSet dataSet({100000, 1000000, 10000000});
    dataSet.prepare(forceNew);
    std::cout << "\n";

    Experiment experiment(dataSet);
    experiment.runLibrary();
    experiment.runCustomParallel();
}
