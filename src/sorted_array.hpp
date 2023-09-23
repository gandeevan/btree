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
    struct Iterator {
    private:
        const SortedArray<T>& arr;
        int idx;
        bool reverse;

    public:
        Iterator(const SortedArray<T>& arr, size_t idx, bool reverse) : arr(arr), idx(idx), reverse(reverse) {

        }

        const T& operator*() const {
            if(*this == arr.end() || *this == arr.rend()) {
                THROW_EXCEPTION("cannot dereference iterator past end");
            }
            return arr.at(idx);
        }

        const T* operator->() const {
            // Return a pointer to the object pointed to by the iterator
            return &arr.at(idx);
        }

        bool operator==(const Iterator& other) const {
            return idx == other.idx && arr == other.arr && reverse == other.reverse;
        }

        Iterator operator++(int) {
            if(reverse) {
                if(*this == arr.rend()) {
                    THROW_EXCEPTION("cannot increment reverse iterator past end");
                }
                idx--;
            } else {
                if(*this == arr.end()) {
                    THROW_EXCEPTION("cannot increment iterator past end");
                }
                idx++;     
            }
            return *this;
        }
    };

    /// @brief Returns an iterator to the first element in the array
    /// @return Iterator to the first element in the array
    Iterator begin() const {
        return Iterator(*this, 0, false);
    }

    /// @brief Returns an iterator to the element past the last element in the array
    /// @return Iterator to the element past the last element in the array
    Iterator end() const {
        return Iterator(*this, size(), false);
    }

    /// @brief Returns a reverse iterator to the last element in the array
    /// @return Iterator to the last element in the array
    Iterator rbegin() const {
        return Iterator(*this, size()-1, true);
    }

    /// @brief Returns a reverse iterator to the element before the first element in the array
    /// @return Iterator to the element before the first element in the array
    Iterator rend() const {
        return Iterator(*this, -1, true);
    }

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

    /// @brief Returns true if the array is equal to the given array
    /// @param other The array to compare to 
    /// @return true if the array is equal to the given array else false
    bool operator==(const SortedArray<T>& other) const {
        if(this->size() != other.size()) {
            std::cout << "size doesn't match, returning false";
            return false;
        }

        for(size_t i=0; i<this->size(); i++) {
            if(this->at(i) != other.at(i)) {
                return false;
            }
        }

        return true;
    }

    /// @brief Returns the index of the first element with the given value
    /// @param val value to find
    /// @return Index of the first element with the given value or empty optional if not found
    std::optional<size_t> find(const T& val) const {
        int idx = getInsertionPoint(val);
        if(idx>0 && arr_[idx-1] == val)
            return idx-1;
        return {};
    }

    /// @brief Inserts the given value into the array
    /// @param val value to insert
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

    /// @brief Erases the first element with the given value
    /// @param val value to erase
    /// @return If the element was erased
    bool eraseElement(const T& val) {
        auto idx = find(val);
        if(!idx.has_value()) {
            return false;
        }
        eraseIndex(idx.value());
        return true;
    }

    /// @brief Erases the element at the given index
    /// @param idx index of the element to erase
    void eraseIndex(size_t idx) {
        if(idx >= size()) {
            THROW_EXCEPTION("index out of bounds");
        }
        eraseIndexRange(idx, idx+1);
    }

    /// @brief Erases the elements in the range [start, end)
    /// @param start start index (inclusive)
    /// @param end end index (exclusive)
    void eraseIndexRange(size_t start, size_t end) {
        if(start >= end || end >= size()) {
            THROW_EXCEPTION("invalid range");
        }
        size_t count = end-start;
        memset((void *)(arr_.get()+start), 0, count * sizeof(T));
        if(end != len_) {
            moveArray(end, -count);
        }
        len_ -= count;
    }

    /// @brief Returns the capacity of the array
    /// @return Capacity of the array
    size_t capacity() const {
        return capacity_;
    }

    /// @brief Returns the number of elements in the array
    /// @return Number of elements in the array
    size_t size() const {
        return len_;
    }

    /// @brief Returns the element at the given index
    /// @param index index of the element to return
    /// @return Element at the given index
    const T& at(size_t index) const {
        if(index >= size()) {
            THROW_EXCEPTION("index out of bounds");
        }
        return arr_[index];
    }

    const T& operator[](size_t index) {
        if(index >= size()) {
            THROW_EXCEPTION("index out of bounds");
        }
        return arr_[index];
    }

    /// @brief Replace the element at the given index with the given value. Will throw an exception if 
    /// the replacement would cause the array to become unsorted.
    /// @param index 
    /// @param val 
    void replace(size_t index, const T& val) {
        if(index >= size()) {
            THROW_EXCEPTION("index out of bounds");
        }   
        if(index > 0 && arr_[index-1] > val) {
            THROW_EXCEPTION("cannot replace element - the array would become unsorted!");
        }
        if(index < size()-1 && arr_[index+1] < val) {
            THROW_EXCEPTION("cannot replace element - the array would become unsorted!");
        }
        
        arr_[index] = val;
    }
};

