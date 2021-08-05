#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
using namespace std;

class Stats {
public:
    void AddMethod(string_view method) {
        if (method_.find(method)==method_.end()) method_["UNKNOWN"]++; else
        method_[method]++;
    };
    void AddUri(string_view uri) {
        if (uri_.find(uri)==uri_.end()) uri_["unknown"]++;
        else uri_[uri]++;
    };
    const map<string_view, int>& GetMethodStats() const {return method_;};
    const map<string_view, int>& GetUriStats() const {return uri_;};

private:
    map<string_view, int> method_ = {
            {"GET", 0},
            {"PUT", 0},
            {"POST", 0},
            {"DELETE", 0},
            {"UNKNOWN", 0},
    };
    map<string_view, int> uri_ = {
            {"/", 0},
            {"/order", 0},
            {"/product", 0},
            {"/basket", 0},
            {"/help", 0},
            {"unknown", 0},
    };
};

HttpRequest ParseRequest(string_view line);
