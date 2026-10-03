#include "CountAlgorithm.h"

#include <algorithm>
#include <numeric>
#include <thread>

std::string CountNone::name() const { return "none"; }

long long CountNone::count(const std::vector<int> &data, PredicateFn pred) const {
    return std::count_if(data.begin(), data.end(), pred);
}

CountCustomParallel::CountCustomParallel(int partsCount)
    : partsCount_(std::max(1, partsCount)) {}

std::string CountCustomParallel::name() const {
    return "custom(K=" + std::to_string(partsCount_) + ")";
}

long long CountCustomParallel::count(const std::vector<int> &data, PredicateFn pred) const {
    const std::size_t n = data.size();
    std::vector<long long> partialCounts(partsCount_);
    std::vector<std::thread> threads;

    for (int t = 0; t < partsCount_; t++) {
        auto first = data.begin() + n * t / partsCount_;
        auto last = data.begin() + n * (t + 1) / partsCount_;
        threads.emplace_back([=, &partialCounts] {
            partialCounts[t] = std::count_if(first, last, pred);
        });
    }

    for (std::thread &thread : threads) {
        thread.join();
    }

    return std::reduce(partialCounts.begin(), partialCounts.end(), 0LL);
}
