#include "DataSet.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <utility>

DataSet::DataSet(std::vector<int> sizes, std::string folder)
    : sizes_(std::move(sizes)), folder_(std::move(folder)) {}

std::string DataSet::fileName(int n) const {
    return folder_ + "/seq_" + std::to_string(n) + ".txt";
}

bool DataSet::allFilesExist() const {
    for (int n : sizes_) {
        if (!std::filesystem::exists(fileName(n))) return false;
    }
    return true;
}

void DataSet::generate(int n, unsigned seed) const {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(2, 100000);

    std::ofstream out(fileName(n));
    out << n << "\n";
    for (int i = 0; i < n; i++) {
        out << dist(rng) << " ";
    }
    out << "\n";
}

void DataSet::prepare(bool forceNew) {
    if (!forceNew && allFilesExist()) {
        std::cout << "data: using existing files from " << folder_ << "/\n";
        return;
    }

    std::filesystem::create_directories(folder_);

    unsigned seed = forceNew ? std::random_device{}() : 42;
    for (int n : sizes_) {
        generate(n, seed);
    }
    std::cout << "data: generated new files in " << folder_ << "/ (seed=" << seed << ")\n";
}

std::vector<int> DataSet::load(int n) const {
    std::ifstream in(fileName(n));
    int count;
    in >> count;
    std::vector<int> data(count);

    for (auto &x : data)
        in >> x;
    return data;
}

const std::vector<int> &DataSet::sizes() const {
    return sizes_;
}
