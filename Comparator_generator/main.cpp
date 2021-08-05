#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

#define UPDATE_FIELD(ticket, field, values) \
 if (values.find(#field) != values.end()) { \
     istringstream is(values.find(#field)->second); \
     is >> ticket.field; \
     }

bool operator<(const Date &lhs, const Date &rhs) {
    if (lhs.year != rhs.year) return lhs.year < rhs.year;
    else if (lhs.month != rhs.month) return lhs.month < rhs.month;
    else return lhs.day < rhs.day;
}

bool operator==(const Date&lhs,const Date &rhs) {
    return lhs.day==rhs.day &&
           lhs.month==rhs.month&&
           lhs.year==rhs.year;
}
std::ostream& operator<<(std::ostream& os,const Date& date) {
    return os<<date.year<<' '<<date.month<<' '<<date.day;
}

std::istream& operator>>(std::istream& is,Date& date) {
    is >>date.year;
    is.ignore(1);
    is >>date.month;
    is.ignore(1);
    is >>date.day;
    return is;
}


bool operator<(const Time &lhs, const Time &rhs) {
    if (lhs.hours != rhs.hours) return lhs.hours < rhs.hours;
    else return lhs.minutes < rhs.minutes;
}

bool operator==(const Time&lhs,const Time &rhs) {
    return lhs.hours==rhs.hours &&
           lhs.minutes==rhs.minutes;
}

std::ostream& operator<<(std::ostream& os,const Time& date) {
    return os<<date.hours<<' '<<date.minutes;
}

std::istream& operator>>(std::istream& is,Time& date) {
    is >>date.hours;
    is.ignore(1);
    is >>date.minutes;
    return is;
}


void TestUpdate() {
    AirlineTicket t;
    t.price = 0;

    const map<string, string> updates1 = {
            {"departure_date", "2018-2-28"},
            {"departure_time", "17:40"},
    };
    UPDATE_FIELD(t, departure_date, updates1);
    UPDATE_FIELD(t, departure_time, updates1);
    UPDATE_FIELD(t, price, updates1);

    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 0);

    const map<string, string> updates2 = {
            {"price", "12550"},
            {"arrival_time", "20:33"},
    };
    UPDATE_FIELD(t, departure_date, updates2);
    UPDATE_FIELD(t, departure_time, updates2);
    UPDATE_FIELD(t, arrival_time, updates2);
    UPDATE_FIELD(t, price, updates2);

    // updates2 не содержит ключей "departure_date" и "departure_time", поэтому
    // значения этих полей не должны измениться
    ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
    ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
    ASSERT_EQUAL(t.price, 12550);
    ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestUpdate);
}
