#include <iostream>
#include <vector>

template <typename T>
class Deque{
public:
    bool Empty() const {
        return (front_.empty()&&back_.empty());
    }

    size_t Size() const {
        return front_.size()+back_.size();
    }

    T& operator[](size_t index) {
        if (Size()<=index) throw std::out_of_range("");
        if (back_.size()<=index) return front_[index-back_.size()];
        return back_[back_.size()-index-1];
    }

    const T& operator[](size_t index) const {
        if (Size()<=index) throw std::out_of_range("");
        if (back_.size()<=index) return front_[index-back_.size()];
        return back_[back_.size()-index-1];
    }

    T& At(size_t index) {
        if (Size()<=index) throw std::out_of_range("");
        if (back_.size()<=index) return front_[index-back_.size()];
        return back_[back_.size()-index-1];
    }

    const T& At(size_t index) const {
        if (Size()<=index) throw std::out_of_range("");
        if (back_.size()<=index) return front_[index-back_.size()];
        return back_[back_.size()-index-1];
    }

    T& Front() {
        return At(0);
    }

    const T& Front() const {
        return At(0);
    }

    T& Back() {
        return At(Size()-1);
    }

    const T& Back() const {
        return At(Size()-1);
    }

    void PushBack(const T& elem) {
        front_.push_back(elem);
    }

    void PushFront(const T& elem) {
        back_.push_back(elem);
    }

private:
    std::vector<T> front_;
    std::vector<T> back_;
};

int main() {
    Deque<int> deq;
    deq.PushBack(3);
    deq.PushBack(3);
    deq.PushBack(3);
    deq.PushBack(4);
    deq[0]=5;
    for (int i=0;i<deq.Size();++i) std::cout << deq[i]<<' ';
    std::cout <<"\n"<<deq.Back()<<' '<<deq.Front();
    return 0;
}
