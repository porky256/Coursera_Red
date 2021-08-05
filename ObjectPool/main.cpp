#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
#include <queue>
#include <set>

using namespace std;

template <class T>
class ObjectPool {
public:
    T* Allocate() {
        if (!free.empty()) {
            T* x = free.front();
            used.insert(x);
            free.pop();
            return x;
        }
        else {
            T* x = new T;
            used.insert(x);
            return x;
        }
    }
    T* TryAllocate() {
        if (!free.empty()) {
            T* x = free.front();
            used.insert(x);
            free.pop();
            return x;
        } else return nullptr;
    }

    void Deallocate(T* object) {
        if (used.find(object)==used.end()) throw std::invalid_argument("");
        used.erase(object);
        free.push(object);
    }

    ~ObjectPool() {
        while (!free.empty()) {
            delete free.front();
            free.pop();
        }
        for (auto &x:used) delete x;
        used.clear();
    }

private:
    std::queue<T*> free;
    std::set<T*> used;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}
