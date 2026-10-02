#pragma once

#include <algorithm>
#include <execution>
#include <memory>
#include <string>
#include <utility>
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

template <class Policy>
class CountWithPolicy : public CountAlgorithm {
public:
    CountWithPolicy(std::string name, Policy policy) : name_(std::move(name)), policy_(policy) {}

    std::string name() const override { return name_; }

    long long count(const std::vector<int> &data, PredicateFn pred) const override {
        return std::count_if(policy_, data.begin(), data.end(), pred);
    }

private:
    std::string name_;
    Policy policy_;
};

template <class Policy>
std::unique_ptr<CountAlgorithm> makePolicyAlgorithm(std::string name, Policy policy) {
    return std::make_unique<CountWithPolicy<Policy>>(std::move(name), policy);
}

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
