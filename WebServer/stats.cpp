#include "stats.h"

HttpRequest ParseRequest(string_view line) {
    HttpRequest req;
    int pos = line.find_first_not_of(' ');
    req.protocol={line.begin()+line.find_last_of(' ')+1,line.length()-line.find_last_of(' ')-1};
    req.method={line.begin()+pos,line.find(' ',pos)-pos};
    req.uri={line.begin()+line.find(' ',pos)+1,line.find_last_of(' ')-(line.find(' ',pos)+1)};
    return req;
};

