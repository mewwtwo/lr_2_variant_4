#pragma once

#include <string>
#include <vector>

using PredicateFn = bool (*)(int);

struct Predicate {
    std::string name;
    PredicateFn test;
};

bool isEvenNumber(int x);
bool isPrime(int x);

std::vector<Predicate> getPredicates();
