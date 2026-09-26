// GCC (Homebrew g++-16), C++20
// Dmytro Demchuk K-25

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

int sizes[] = {100000, 1000000, 10000000};

int main() {
    std::filesystem::create_directory("data");

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(2, 100000);

    for (int n : sizes) {
        std::string fileName = "data/seq_" + std::to_string(n) + ".txt";
        std::ofstream out(fileName);
        out << n << "\n";
        for (int i = 0; i < n; i++) {
            out << dist(rng) << " ";
        }
        out << "\n";
        std::cout << "generated " << fileName << "\n";
    }
}
