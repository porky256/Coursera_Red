#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>

using namespace std;

class ReadingManager {
public:
    ReadingManager():pages(1001,0){}

    void Read(int user_id, int page_count) {
        if (user_page_count.find(user_id)!=user_page_count.end()) {
            for (int i=user_page_count[user_id];i<1001;++i) --pages[i];
        }
        user_page_count[user_id]=page_count;
        for (int i=page_count;i<1001;++i) ++pages[i];
    }

    double Cheer(int user_id) const {
        int user_count = user_page_count.size();
        if (user_page_count.find(user_id)==user_page_count.end()) return 0;
        if (user_count==1) return 1;
        return (pages[user_page_count.find(user_id)->second-1]) * 1.0 / (user_count - 1);
    }

private:
    // Статическое поле не принадлежит какому-то конкретному
    // объекту класса. По сути это глобальная переменная,
    // в данном случае константная.
    // Будь она публичной, к ней можно было бы обратиться снаружи
    // следующим образом: ReadingManager::MAX_USER_COUNT.
    static const int MAX_USER_COUNT_ = 100'000;

    map<int,int> user_page_count;
    std::vector<int> pages;

};


int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}