#pragma once

#include <cstring>
#include <stdexcept>
#include <vector>

using namespace std;

template<typename T>
class SortedArray {
    T* arr;
    int len = 0;
    int maxSize = 0;

    int getInsertionPoint(const T& elem) {
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
        memset(arr, 0, maxSize * sizeof(T));
    }

    SortedArray(vector<T>&& vec) : SortedArray(vec.size()) {
        std::sort(vec.begin(), vec.end());
        memcpy(arr, vec.data(), sizeof(T) * vec.size());
        len = vec.size();
    }

    ~SortedArray() {
        delete arr;
    }

    int get(const T& val) {
        int idx = getInsertionPoint(val);
        if(idx>0 && arr[idx-1] == val)
            return idx-1;
        return -1;
    }

    void insert(const T& val) {
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

    void remove(int idx) {
        remove(idx, idx+1);
    }

    void remove(int start, int end) {
        int count = end-start;
        memset(arr+start, 0, count * sizeof(T));
        if(end != len) {
            moveArray(end, -count);
        }
        len -= count;
    }

    unsigned capacity() {
        return maxSize;
    }

    unsigned size() {
        return len;
    }


    const T& at(int index) {
        return arr[index];
    }
};

