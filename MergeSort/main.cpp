#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <iterator>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_end-range_begin<2) return;
    vector<typename RandomIt::value_type> elements1(make_move_iterator(range_begin),
                                               make_move_iterator(range_begin+(range_end-range_begin)/3));
    vector<typename RandomIt::value_type> elements2(make_move_iterator(range_begin+(range_end-range_begin)/3),
                                                    make_move_iterator(range_begin+2*(range_end-range_begin)/3));
    vector<typename RandomIt::value_type> elements3(make_move_iterator(range_begin+2*(range_end-range_begin)/3),
                                                    make_move_iterator(range_end));
    MergeSort(elements1.begin(),elements1.end());
    MergeSort(elements2.begin(),elements2.end());
    MergeSort(elements3.begin(),elements3.end());
    vector<typename RandomIt::value_type> mm;
    merge(make_move_iterator(elements1.begin()),make_move_iterator(elements1.end()),
          make_move_iterator(elements2.begin()),make_move_iterator(elements2.end()),
          back_inserter(mm));
    merge(make_move_iterator(mm.begin()),make_move_iterator(mm.end()),
          make_move_iterator(elements3.begin()),make_move_iterator(elements3.end()),
          range_begin);
}

void TestIntVector() {
    vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
    MergeSort(begin(numbers), end(numbers));
    ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    return 0;
}
