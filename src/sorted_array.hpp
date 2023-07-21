#pragma once

#include <stdexcept>
#include <vector>

using namespace std;

template<typename T>
class SortedArray {
    T* arr;
    int len = 0;
    int maxSize = 0;

    int getInsertionPoint(T& elem) {
        int left = 0, right = len;
        while(left < right) {
            int mid = (left+right)/2;
            if(arr[mid] > elem) {
                right = mid;        
            } else {
               left = mid+1;     
            }
        }
        return left;
    }

    void moveArray(int idx, int pos) {
        memcpy(arr+idx+pos, arr+idx, (len-idx)*sizeof(T));
    }

public:
    SortedArray(int n) : maxSize(n) {
        arr = new T[n];
    }

    ~SortedArray() {
        delete arr;
    }

    void insert(T &&val) {
        if(len == maxSize) {
            throw runtime_error("cannot insert element - the array is full!");
        }
        auto idx = getInsertionPoint(val);
        if(idx != len) {
            moveArray(idx, 1);
        } 
        arr[idx] = val;
        len++;
    }


    unsigned size() {
        return len;
    }


    const T& at(int index) {
        return arr[index];
    }
};

