#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <set>

#define DAY 86400

struct Hotel {
    std::queue<std::tuple<int64_t,int,int>> orders_;
    std::map<int,int> clients_;
    int number_of_ordered_rooms=0;

    void Book(int64_t time,int client_id,int room_count) {
        orders_.push({time,client_id,room_count});
        number_of_ordered_rooms+=room_count;
        if (clients_.find(client_id)==clients_.end()) {
            clients_.insert({client_id,room_count});
        } else {
            clients_[client_id]+=room_count;
        }
    }

    int Clients(int64_t time) {
        Clear(time);
        return clients_.size();
    }

    int Rooms(int64_t time) {
        Clear(time);
        return number_of_ordered_rooms;
    }

private:
    void Clear(int64_t time) {
        while (!orders_.empty()&&std::get<0>(orders_.front())<=time-DAY) {
            number_of_ordered_rooms-=std::get<2>(orders_.front());
            int client_id = std::get<1>(orders_.front());
            clients_[client_id]-=std::get<2>(orders_.front());
            if (clients_[client_id]==0) clients_.erase(client_id);
            orders_.pop();
        }
    }
};

class Booking {
public:
    void Book(int64_t time,const std::string& hotel_name,int client_id,int room_count) {
        if (hotels_.find(hotel_name)==hotels_.end()) {
            hotels_.insert({hotel_name,Hotel()});
        }
        last_time_ = time;
        hotels_[hotel_name].Book(time,client_id,room_count);
    }

    int Clients(const std::string& hotel_name) {
        if (hotels_.find(hotel_name)==hotels_.end()) return 0;
        return hotels_[hotel_name].Clients(last_time_);
    }

    int Rooms(const std::string& hotel_name) {
        if (hotels_.find(hotel_name)==hotels_.end()) return 0;
        return hotels_[hotel_name].Rooms(last_time_);
    }

private:
    std::map<std::string,Hotel> hotels_;
    int64_t last_time_;
};

int main() {
    Booking booking;
    int n;
    std::cin >>n;
    while (n--) {
        std::string command;
        std::string hotel_name;
        std::cin >> command;
        if (command == "CLIENTS") {
            std::cin >>hotel_name;
            std::cout <<booking.Clients(hotel_name)<<'\n';
            continue;
        }
        if (command == "ROOMS") {
            std::cin >>hotel_name;
            std::cout <<booking.Rooms(hotel_name)<<'\n';
            continue;
        }
        if (command == "BOOK") {
            int64_t time;
            int client_id,room_count;
            std::cin >>time>>hotel_name>>client_id>>room_count;
            booking.Book(time,hotel_name,client_id,room_count);
        }
    }
    return 0;
}
