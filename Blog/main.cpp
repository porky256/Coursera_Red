#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <vector>
#include <string_view>
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
    return Paginator(c.begin(),c.end(),page_size);
}


struct Stats {
    map<string, int> word_frequences;

    void operator += (const Stats& other) {
        for (auto &x:other.word_frequences) word_frequences[x.first]+=x.second;
    };
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
    Stats answer;
    auto iter = line.begin();
    auto prev=iter;
    while (iter!=line.end()) {
        if (*iter==' ') {
            if (distance(prev,iter)>1) {
                string word = {prev,iter};
                if (key_words.find(word)!=key_words.end()) answer.word_frequences[word]++;
            }
            prev=iter+1;
        }
        iter++;
    }
    if (distance(prev,iter)>2) {
        string word = {prev,iter};
        if (key_words.find(word)!=key_words.end()) answer.word_frequences[word]++;
    }
    return answer;
}

Stats ExploreKeyWordsSingleThread(
        const set<string>& key_words, Page<vector<string>::iterator>& input
) {
    Stats result;
    for (auto& line:input ) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    std::vector<future<Stats>> futures;
    std::vector<string> strings;
    string str;
    while (getline(input,str)) {
        strings.push_back(move(str));
    }
    auto Pager = Paginate(strings,strings.size()/4);
    for (auto &x:Pager) {
        futures.push_back(async(ExploreKeyWordsSingleThread,ref(key_words),ref(x)));
    }
    Stats answer;
    for (auto&x :futures) answer+=x.get();
    return answer;
}

void TestBasic() {
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
            {"yangle", 6},
            {"rocks", 2},
            {"sucks", 1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
}
