#include "test_runner.h"

#include <ostream>
using namespace std;

#define PRINT_VALUES(out, x, y) do{ out << (x) << '\n'; out << (y) << '\n';}while(0)

int main() {
    TestRunner tr;
    tr.RunTest([] {
        ostringstream output;
        ASSERT_EQUAL(output.str(), "5\nred belt\n");
    }, "PRINT_VALUES usage example");
}
