#include "Predicates.h"

bool isEvenNumber(int x) {
    return x % 2 == 0;
}

bool isPrime(int x) {
    if (x < 2) return false;
    for (int d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

std::vector<Predicate> getPredicates() {
    return {
        {"even", isEvenNumber},
        {"prime", isPrime},
    };
}
