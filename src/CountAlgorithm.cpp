#include "CountAlgorithm.h"

#include <algorithm>
#include <execution>
#include <functional>
#include <numeric>
#include <thread>

std::string CountNone::name() const { return "none"; }

long long CountNone::count(const std::vector<int> &data, PredicateFn pred) const {
    return std::count_if(data.begin(), data.end(), pred);
}

std::string CountSeq::name() const { return "seq"; }

long long CountSeq::count(const std::vector<int> &data, PredicateFn pred) const {
    return std::count_if(std::execution::seq, data.begin(), data.end(), pred);
}

std::string CountPar::name() const { return "par"; }

long long CountPar::count(const std::vector<int> &data, PredicateFn pred) const {
    return std::count_if(std::execution::par, data.begin(), data.end(), pred);
}

std::string CountParUnseq::name() const { return "par_unseq"; }

long long CountParUnseq::count(const std::vector<int> &data, PredicateFn pred) const {
    return std::count_if(std::execution::par_unseq, data.begin(), data.end(), pred);
}

CountCustomParallel::CountCustomParallel(int partsCount)
    : partsCount_(std::max(1, partsCount)) {}

std::string CountCustomParallel::name() const {
    return "custom(K=" + std::to_string(partsCount_) + ")";
}

void CountCustomParallel::countInRange(const std::vector<int> &data, PredicateFn pred,
                                       int begin, int end, long long &count) {
    count = std::count_if(data.begin() + begin, data.begin() + end, pred);
}

long long CountCustomParallel::count(const std::vector<int> &data, PredicateFn pred) const {
    int n = (int)data.size();
    int chunkSize = n / partsCount_;

    std::vector<std::thread> threads(partsCount_);
    std::vector<long long> partialCounts(partsCount_);

    int begin = 0;
    for (int t = 0; t < partsCount_; t++) {
        int end = (t == partsCount_ - 1) ? n : begin + chunkSize;
        threads[t] = std::thread(countInRange, std::cref(data), pred, begin, end, std::ref(partialCounts[t]));
        begin = end;
    }

    for (int t = 0; t < partsCount_; t++) {
        threads[t].join();
    }

    return std::reduce(partialCounts.begin(), partialCounts.end(), 0LL);
}
