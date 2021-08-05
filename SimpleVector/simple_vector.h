#pragma once

#include <cstdlib>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
    SimpleVector() {
        capacity_ = 0;
        size_ = 0;
        vector_ = nullptr;
        begin_ = nullptr;
        end_ = nullptr;
    }
    explicit SimpleVector(size_t size) {
        capacity_ = size;
        size_ = size;
        vector_ = new T[size];
        begin_ = vector_;
        end_ = vector_+size;
    };
    ~SimpleVector() {
        delete[]vector_;
    };

    T& operator[](size_t index) {
        return vector_[index];
    };

    T* begin() {return begin_;}
    T* end() {return end_;}

    size_t Size() const {return size_;};
    size_t Capacity() const {return capacity_;};
    void PushBack(const T& value) {
        if (capacity_ == size_) {
            capacity_<<=1;
            if (!capacity_) capacity_=1;
            T* new_vector_ = new T[capacity_];
            if (vector_) for (int i=0;i<size_;++i) new_vector_[i]=vector_[i];
            delete[] vector_;
            vector_=new_vector_;
            begin_ = vector_;
        }
        vector_[size_++] = value;
        end_ = vector_+size_;
    };

private:
    size_t capacity_;
    size_t size_;
    T* begin_;
    T* end_;
    T* vector_;
};
