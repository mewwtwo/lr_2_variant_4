#pragma once

#include <string>
#include <vector>

#include "Predicates.h"

class CountAlgorithm {
public:
    virtual ~CountAlgorithm() = default;

    virtual std::string name() const = 0;
    virtual long long count(const std::vector<int> &data, PredicateFn pred) const = 0;
};

class CountNone : public CountAlgorithm {
public:
    std::string name() const override;
    long long count(const std::vector<int> &data, PredicateFn pred) const override;
};

class CountSeq : public CountAlgorithm {
public:
    std::string name() const override;
    long long count(const std::vector<int> &data, PredicateFn pred) const override;
};

class CountPar : public CountAlgorithm {
public:
    std::string name() const override;
    long long count(const std::vector<int> &data, PredicateFn pred) const override;
};

class CountParUnseq : public CountAlgorithm {
public:
    std::string name() const override;
    long long count(const std::vector<int> &data, PredicateFn pred) const override;
};

class CountCustomParallel : public CountAlgorithm {
public:
    explicit CountCustomParallel(int partsCount);

    std::string name() const override;
    long long count(const std::vector<int> &data, PredicateFn pred) const override;

private:
    static void countInRange(const std::vector<int> &data, PredicateFn pred,
                             int begin, int end, long long &count);

    int partsCount_;
};
