#include "test_runner.h"
#include "profile.h"

#include <algorithm>
#include <numeric>
#include <vector>
#include <string>
#include <random>
#include <future>
using namespace std;

template <typename Iterator>
class Page{
public:
    Page(const Iterator& begin,const Iterator& end):begin_(begin),end_(end),size_(distance(begin,end)) {}

    [[nodiscard]] size_t size() const {return size_;};

    [[nodiscard]] Iterator begin() const {return begin_;};

    [[nodiscard]] Iterator end() const {return end_;};

    Iterator begin() {return begin_;};

    Iterator end() {return end_;};
private:
    Iterator begin_;
    Iterator end_;
    size_t size_;
};

template <typename Iterator>
class Paginator {
public:
    Paginator(const Iterator& begin,const Iterator& end,size_t page_size):begin_(begin),end_(end),page_size_(page_size) {
        for (size_t left = distance(begin, end); left > 0; ) {
            size_t current_page_size = min(page_size, left);
            Iterator current_page_end = next(begin_, current_page_size);
            pages_.push_back({begin_, current_page_end});
            left -= current_page_size;
            begin_ = current_page_end;
        }
    }

    [[nodiscard]] size_t size() const {return pages_.size();}

    [[nodiscard]] auto begin() const {return pages_.begin();};

    auto begin() {return pages_.begin();};

    [[nodiscard]] auto end() const {return pages_.end();};

    auto end() {return pages_.end();};


private:
    Iterator begin_;
    Iterator end_;
    size_t page_size_;
    std::vector<Page<Iterator>> pages_;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
    return Paginator<C>(c.begin(),c.end(),page_size);
}

template <typename K, typename V>
class ConcurrentMap {
public:
  static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");

  struct Access {
      lock_guard<mutex> lock;
    V& ref_to_value;
  };

  explicit ConcurrentMap(size_t bucket_count):size_(bucket_count),mutexes_(size_),maps_(size_) {};

  Access operator[](const K& key) {
      int position = (size_t)key%size_;
      return {lock_guard<mutex>(mutexes_[position]),maps_[position][key]};
  };

  map<K, V> BuildOrdinaryMap() {
      map<K,V> resultMap;
      int i=0;
      for (auto& x:maps_) {
          lock_guard<mutex> lock(mutexes_[i++]);
          for (auto &y:x) resultMap[y.first] = y.second;
      }
      return resultMap;
  };

private:
        size_t size_;
        vector<map<K,V>> maps_;
        vector<mutex> mutexes_;
};

void RunConcurrentUpdates(
    ConcurrentMap<int, int>& cm, size_t thread_count, int key_count
) {
  auto kernel = [&cm, key_count](int seed) {
    vector<int> updates(key_count);
    iota(begin(updates), end(updates), -key_count / 2);
    shuffle(begin(updates), end(updates), default_random_engine(seed));

    for (int i = 0; i < 2; ++i) {
      for (auto key : updates) {
        cm[key].ref_to_value++;
      }
    }
  };

  vector<future<void>> futures;
  for (size_t i = 0; i < thread_count; ++i) {
    futures.push_back(async(kernel, i));
  }
}

void TestConcurrentUpdate() {
  const size_t thread_count = 3;
  const size_t key_count = 50000;

  ConcurrentMap<int, int> cm(thread_count);
  RunConcurrentUpdates(cm, thread_count, key_count);

  const auto result = cm.BuildOrdinaryMap();
  ASSERT_EQUAL(result.size(), key_count);
  for (auto& [k, v] : result) {
    AssertEqual(v, 6, "Key = " + to_string(k));
  }
}

void TestReadAndWrite() {
  ConcurrentMap<size_t, string> cm(5);

  auto updater = [&cm] {
    for (size_t i = 0; i < 50000; ++i) {
      cm[i].ref_to_value += 'a';
    }
  };
  auto reader = [&cm] {
    vector<string> result(50000);
    for (size_t i = 0; i < result.size(); ++i) {
      result[i] = cm[i].ref_to_value;
    }
    return result;
  };

  auto u1 = async(updater);
  auto r1 = async(reader);
  auto u2 = async(updater);
  auto r2 = async(reader);

  u1.get();
  u2.get();

  for (auto f : {&r1, &r2}) {
    auto result = f->get();
    ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
      return s.empty() || s == "a" || s == "aa";
    }));
  }
}

void TestSpeedup() {
  {
    ConcurrentMap<int, int> single_lock(1);

    LOG_DURATION("Single lock");
    RunConcurrentUpdates(single_lock, 4, 50000);
  }
  {
    ConcurrentMap<int, int> many_locks(100);

    LOG_DURATION("100 locks");
    RunConcurrentUpdates(many_locks, 4, 50000);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestConcurrentUpdate);
  RUN_TEST(tr, TestReadAndWrite);
  RUN_TEST(tr, TestSpeedup);
}