#pragma once

#include <cstring>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>

using namespace std;

template<typename T>
class SortedArray {
private:
    std::unique_ptr<T[]> arr_;
    int len_ = 0;
    int maxSize_ = 0;

    int getInsertionPoint(const T& elem) const {
        int left = 0, right = len_;
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

    
    void moveArray(int idx, int pos) {
        memcpy(arr_.get()+idx+pos, arr_.get()+idx, (len_-idx)*sizeof(T));
    }

public:
    SortedArray(int n) : maxSize_(n) {
        arr_.reset(new T[n]);
        memset(arr_.get(), 0, maxSize_ * sizeof(T));
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

        for(int i=0; i<this->size(); i++) {
            if(this->at(i) != other.at(i)) {
                std::cout << "Elements don't match at index " << i << " " << this->at(i) << " != " << other.at(i);
                return false;
            }
        }

        return true;
    }

    int find(const T& val) const {
        int idx = getInsertionPoint(val);
        if(idx>0 && arr_[idx-1] == val)
            return idx-1;
        return -1;
    }

    void insert(const T& val) {
        if(len_ == maxSize_) {
            throw runtime_error("cannot insert element - the array is full!");
        }
        auto idx = getInsertionPoint(val);
        if(idx != len_) {
            moveArray(idx, 1);
        } 
        arr_[idx] = val;
        len_++;
    }

    void remove(int idx) {
        remove(idx, idx+1);
    }

    void remove(int start, int end) {
        int count = end-start;
        memset(arr_.get()+start, 0, count * sizeof(T));
        if(end != len_) {
            moveArray(end, -count);
        }
        len_ -= count;
    }

    unsigned capacity() const {
        return maxSize_;
    }

    unsigned size() const {
        return len_;
    }

    const T& at(int index) const {
        return arr_[index];
    }
};

