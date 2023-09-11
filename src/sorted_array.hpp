#pragma once

#include <cstring>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <algorithm>
#include <optional>
#include "constants.hpp"

using namespace std;

template<typename T>
class SortedArray {
private:
    std::unique_ptr<T[]> arr_;
    size_t len_ = 0;
    size_t capacity_ = 0;

    size_t getInsertionPoint(const T& elem) const {
        size_t left = 0, right = len_;
        while(left < right) {
            int mid = (left+right)/2;
            if(arr_[mid] > elem) {
                right = mid;        
            } else {
               left = mid+1;     
            }
        }
        return left;
    }

    
    void moveArray(size_t idx, size_t pos) {
        memcpy((void *)(arr_.get()+idx+pos), (void *)(arr_.get()+idx), (len_-idx)*sizeof(T));
    }

public:
    SortedArray(size_t n) : capacity_(n) {
        arr_.reset(new T[n]);
        memset((void *)arr_.get(), 0, capacity_ * sizeof(T));
    }

    SortedArray(vector<T>&& vec) : SortedArray(vec.size()) {
        std::sort(vec.begin(), vec.end());
        memcpy(arr_.get(), vec.data(), sizeof(T) * vec.size());
        len_ = vec.size();
    }

    ~SortedArray() {
    }

    bool operator==(const SortedArray<T>& other) const {
        if(this->size() != other.size()) {
            std::cout << "size doesn't match, returning false";
            return false;
        }

        for(size_t i=0; i<this->size(); i++) {
            if(this->at(i) != other.at(i)) {
                std::cout << "Elements don't match at index " << i << " " << this->at(i) << " != " << other.at(i);
                return false;
            }
        }

        return true;
    }

    std::optional<size_t> find(const T& val) const {
        int idx = getInsertionPoint(val);
        if(idx>0 && arr_[idx-1] == val)
            return idx-1;
        return {};
    }

    void insert(const T& val) {
        if(len_ == capacity_) {
            THROW_EXCEPTION("cannot insert element - the array is full!");
        }
        auto idx = getInsertionPoint(val);
        if(idx != len_) {
            moveArray(idx, 1);
        } 
        arr_[idx] = val;
        len_++;
    }

    bool eraseElement(const T& val) {
        auto idx = find(val);
        if(!idx.has_value()) {
            return false;
        }
        eraseIndex(idx.value());
        return true;
    }

    void eraseIndex(size_t idx) {
        if(idx >= size()) {
            THROW_EXCEPTION("index out of bounds");
        }
        eraseIndexRange(idx, idx+1);
    }

    void eraseIndexRange(size_t start, size_t end) {
        size_t count = end-start;
        memset((void *)(arr_.get()+start), 0, count * sizeof(T));
        if(end != len_) {
            moveArray(end, -count);
        }
        len_ -= count;
    }

    size_t capacity() const {
        return capacity_;
    }

    size_t size() const {
        return len_;
    }

    const T& at(size_t index) const {
        return arr_[index];
    }
};

