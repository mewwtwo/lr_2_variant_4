// GCC (Homebrew g++-16), C++20
// Dmytro Demchuk K-25

#include <algorithm>
#include <chrono>
#include <execution>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

template <typename Func>
double measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

bool isEven(int x) { return x % 2 == 0; }

bool isPrime(int x) {
    if (x < 2) return false;
    for (int d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

std::vector<int> loadData(int n) {
    std::ifstream in("data/seq_" + std::to_string(n) + ".txt");
    int count;
    in >> count;

    std::vector<int> data(count);

    for (auto &x : data)
        in >> x;
    return data;
}

long long countIfNone(const std::vector<int> &data, bool (*pred)(int)) {
    return std::count_if(data.begin(), data.end(), pred);
}

long long countIfSeq(const std::vector<int> &data, bool (*pred)(int)) {
    return std::count_if(std::execution::seq, data.begin(), data.end(), pred);
}

long long countIfPar(const std::vector<int> &data, bool (*pred)(int)) {
    return std::count_if(std::execution::par, data.begin(), data.end(), pred);
}

long long countIfParUnseq(const std::vector<int> &data, bool (*pred)(int)) {
    return std::count_if(std::execution::par_unseq, data.begin(), data.end(), pred);
}

void countChunk(const std::vector<int> &data, bool (*pred)(int), int begin, int end, long long &count) {
    count = std::count_if(data.begin() + begin, data.begin() + end, pred);
}

// ділимо масив на K частин, кожну рахуємо в своєму потоці функцією
// countChunk, потім складаємо часткові результати std::reduce
long long countIfCustomParallel(const std::vector<int> &data, bool (*pred)(int), int K) {
    int n = (int)data.size();
    int chunkSize = n / K;

    std::vector<std::thread> threads(K);
    std::vector<long long> partialCounts(K);

    int begin = 0;
    for (int t = 0; t < K; t++) {
        int end = (t == K - 1) ? n : begin + chunkSize; // останній потік забирає залишок
        threads[t] = std::thread(countChunk, std::cref(data), pred, begin, end, std::ref(partialCounts[t]));
        begin = end;
    }

    for (int t = 0; t < K; t++) {
        threads[t].join();
    }

    return std::reduce(partialCounts.begin(), partialCounts.end(), 0LL);
}

using CountFunc = long long (*)(const std::vector<int> &, bool (*)(int));

void runExperiment(int n, const std::string &label, CountFunc countFunc, bool (*pred)(int)) {
    std::vector<int> data;
    long long result = 0;

    double totalTime = measureTime([&]() {
        data = loadData(n);
        result = countFunc(data, pred);
    });

    double computeTime = measureTime([&]() {
        result = countFunc(data, pred);
    });

    std::cout << label << " result=" << result
               << " total=" << totalTime << "ms compute=" << computeTime << "ms\n";
}

int sizes[] = {100000, 1000000, 10000000};

int main() {
    std::cout << "hardware_concurrency = " << std::thread::hardware_concurrency() << "\n\n";

    std::pair<const char *, bool (*)(int)> preds[] = {
        {"even", isEven},
        {"prime", isPrime},
    };

    for (int n : sizes) {
        for (auto [predName, pred] : preds) {
            std::string base = "N=" + std::to_string(n) + " predicate=" + predName + " policy=";
            runExperiment(n, base + "none", countIfNone, pred);
            runExperiment(n, base + "seq", countIfSeq, pred);
            runExperiment(n, base + "par", countIfPar, pred);
            runExperiment(n, base + "par_unseq", countIfParUnseq, pred);
        }
        std::cout << "\n";
    }

    std::cout << " custom parallel count_if \n";
    std::cout << "N=10000000 predicate=prime\n";
    std::vector<int> data = loadData(10000000);
    int Ks[] = {1, 2, 4, 6, 8, 12};
    for (int k : Ks) {
        long long result = 0;
        double computeTime = measureTime([&]() {
            result = countIfCustomParallel(data, isPrime, k);
        });
        std::cout << "K=" << k << " result=" << result << " compute=" << computeTime << " ms\n";
    }
}
