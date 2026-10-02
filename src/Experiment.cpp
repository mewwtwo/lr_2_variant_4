#include "Experiment.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <thread>
#include <vector>

#include "Timer.h"

Experiment::Experiment(const DataSet &dataSet)
    : dataSet_(dataSet), predicates_(getPredicates()) {
    algorithms_.push_back(std::make_unique<CountNone>());
    algorithms_.push_back(std::make_unique<CountSeq>());
    algorithms_.push_back(std::make_unique<CountPar>());
    algorithms_.push_back(std::make_unique<CountParUnseq>());
}

void Experiment::runOne(int n, const std::string &label,
                        const CountAlgorithm &algorithm, PredicateFn pred) const {
    std::vector<int> data;
    long long result = 0;

    double totalTime = measureTime([&]() {
        data = dataSet_.load(n);
        result = algorithm.count(data, pred);
        std::cout << label << " result=" << result;
    });

    double computeTime = measureTime([&]() {
        result = algorithm.count(data, pred);
    });

    std::cout << " total=" << totalTime << "ms compute=" << computeTime << "ms\n";
}

void Experiment::runLibrary() const {
    for (int n : dataSet_.sizes()) {
        runForSize(n);
        std::cout << "\n";
    }
}

void Experiment::runForSize(int n) const {
    for (const Predicate &predicate : predicates_) {
        runForPredicate(n, predicate);
    }
}

void Experiment::runForPredicate(int n, const Predicate &predicate) const {
    for (const auto &algorithm : algorithms_) {
        std::string label = "N=" + std::to_string(n) + " predicate=" + predicate.name +
                            " policy=" + algorithm->name();
        runOne(n, label, *algorithm, predicate.test);
    }
}

void Experiment::runCustomParallel() const {
    int n = dataSet_.sizes().back();
    std::vector<int> data = dataSet_.load(n);
    unsigned hardwareThreads = std::thread::hardware_concurrency();

    std::cout << "custom parallel count_if, N=" << n << " predicate=prime\n";
    std::cout << std::setw(6) << "K" << std::setw(14) << "compute_ms" << std::setw(12) << "result" << "\n";

    int bestK = 0;
    double bestTime = std::numeric_limits<double>::max();

    for (int k : {1, 2, 4, 6, 8, 12, 16, 24, 32}) {
        CountCustomParallel algorithm(k);
        long long result = 0;

        double computeTime = measureTime([&]() {
            result = algorithm.count(data, isPrime);
        });

        std::cout << std::setw(6) << k
                  << std::setw(14) << std::fixed << std::setprecision(3) << computeTime
                  << std::setw(12) << result << "\n";

        if (computeTime < bestTime) {
            bestTime = computeTime;
            bestK = k;
        }
    }

    std::cout << "best K = " << bestK << " (compute=" << bestTime << " ms), "
              << "hardware_concurrency = " << hardwareThreads << ", "
              << "K / hardware_concurrency = " << std::setprecision(2)
              << (double)bestK / hardwareThreads << "\n";
    std::cout << std::defaultfloat;
}
