#pragma once

#include <memory>
#include <string>
#include <vector>

#include "CountAlgorithm.h"
#include "DataSet.h"

class Experiment {
public:
    explicit Experiment(const DataSet &dataSet);

    void runLibrary() const;

    void runCustomParallel() const;

private:
    void runForSize(int n) const;
    void runForPredicate(int n, const Predicate &predicate) const;
    void runOne(int n, const std::string &label,
                const CountAlgorithm &algorithm, PredicateFn pred) const;

    const DataSet &dataSet_;
    std::vector<Predicate> predicates_;
    std::vector<std::unique_ptr<CountAlgorithm>> algorithms_;
};
