#pragma once

#include <string>
#include <vector>


class DataSet {
public:
    DataSet(std::vector<int> sizes, std::string folder = "data");

    void prepare(bool forceNew);

    std::vector<int> load(int n) const;
    const std::vector<int> &sizes() const;

private:
    std::string fileName(int n) const;
    bool allFilesExist() const;
    void generate(int n, unsigned seed) const;

    std::vector<int> sizes_;
    std::string folder_;
};
