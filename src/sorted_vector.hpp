#pragma once

#include <vector>

using namespace std;

template<typename T>
class SortedVector {
    std::vector<T> data;

public:
    void insert(T &val) {
        throw std::runtime_error("Not implemented");
    }


    unsigned size() {
        return data.size();
    }


    const T& at(int index) {
        return data.at(index);
    }
};

