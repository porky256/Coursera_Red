#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <map>
#include <queue>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  using Id = size_t;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object) {
      elements.insert({counter,move(object)});
      priority_to_element.push({0,counter});
      priorities.insert({counter,0});
      return counter++;
  };

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin) {
      for (;range_begin!=range_end;range_begin++) {
          elements.insert({counter,move(*range_begin)});
          priority_to_element.push({0,counter});
          priorities.insert({counter,0});
          *ids_begin++=counter++;
      }
  };

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const {
      return elements.find(id)!=elements.end();
  };

  // Получить объект по идентификатору
  const T& Get(Id id) const {
      const T& element = elements.find(id)->second;
      return element;
  };

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
      size_t priority=priorities[id];
      priority_to_element.push({priority+1,id});
      priorities[id]=priority+1;
      auto pai = priority_to_element.top();
  };

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const {
      Id id =priority_to_element.top().second;
      const T& element = elements.find(id)->second;
      return {element,priorities.find(id)->second};
  };

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax() {
      Id id =priority_to_element.top().second;
      priority_to_element.pop();
      pair<T,int> answer = {move(elements[id]),priorities[id]};
      elements.erase(id);
      priorities.erase(id);
      auto pai = priority_to_element.top();
      while (!priority_to_element.empty() && (priorities.find(pai.second)==priorities.end() ||
                                     priorities.find(pai.second)->second!=pai.first)) {
          priority_to_element.pop();
          pai = priority_to_element.top();
      }
      return answer;
  };

private:
  Id counter=0;
  priority_queue<pair<size_t,Id>,vector<pair<size_t,Id>>,less<pair<size_t,Id>>> priority_to_element;
  map<Id,T> elements;
  map<Id,size_t> priorities;
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy1()
{
    PriorityCollection<StringNonCopyable> strings;

    std::vector<StringNonCopyable> words;
    words.emplace_back("a");
    words.emplace_back("b");
    words.emplace_back("c");

    std::vector<int> indexes;
    strings.Add(words.begin(), words.end(), std::back_inserter(indexes));
    for (int i : indexes)
        std::cout << i << ' ';
}

void TestAnother() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");
    strings.Get(white_id);
    auto x = strings.GetMax();
}
void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  RUN_TEST(tr, TestNoCopy1);
  TestAnother();
  return 0;
}

