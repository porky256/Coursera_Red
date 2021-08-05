#pragma once

#include "iterator_range.h"
#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include "log_duration.h"
using namespace std;

class InvertedIndex {
public:
  void Add(const string& document){
      docs.push_back(document);

      const size_t docid = docs.size() - 1;
      istringstream words_input(document);
      vector<string> x = {istream_iterator<string>(words_input), istream_iterator<string>()};
      for (const auto& word : x) {
          index[word].push_back(docid);
      }
  }
  bool Find(const string& word){
      return (index.find(word)!=index.end());
  }

  const list<size_t>& Lookup(const string& word) const{
        if (auto it = index.find(word); it != index.end()) {
            return it->second;
        } else {
            return {};
        }
  }

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

private:
  unordered_map<string, list<size_t>> index;
  vector<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input){
      UpdateDocumentBase(document_input);
  }
  void UpdateDocumentBase(istream& document_input){
      InvertedIndex new_index;

      for (string current_document; getline(document_input, current_document); ) {
          new_index.Add(move(current_document));
      }

      index = move(new_index);
  }
  void AddQueriesStream(istream& query_input, ostream& search_results_output){
      vector<std::pair<size_t,size_t>> docid_count;
      docid_count.resize(50000);
      for (string current_query; getline(query_input, current_query); ) {
          istringstream words_input(current_query);
          const vector<string> words = {istream_iterator<string>(words_input), istream_iterator<string>()};
          {
              AddDuration(lookewfe212up.value);
              for (int i = 0; i < 50000; ++i) docid_count[i] = {i, 0};
              for (const auto &word : words) {
                  if (index.Find(word))
                      for (const size_t docid : index.Lookup(word)) {
                          ++docid_count[docid].second;
                      }
              }
          }

          AddDuration(forming.value);
          vector<pair<size_t,size_t>> ans;
          partial_sort(docid_count.begin(),docid_count.end(),docid_count.begin()+5,
                       [](const pair<size_t,size_t>& lhs,
                          const pair<size_t,size_t>& rhs) {
                           if (lhs.second==rhs.second)
                               return lhs.first<rhs.first;
                           else return lhs.second>rhs.second;
                       });

          sort(docid_count.begin(),docid_count.begin()+5,[](const pair<size_t,size_t>& lhs,
                                                            const pair<size_t,size_t>& rhs) {
              if (lhs.second==rhs.second)
                  return lhs.first<rhs.first;
              else return lhs.second>rhs.second;
          });

          search_results_output << current_query << ':';
          for (auto [docid, hitcount] : Head(docid_count, 5)) {
              if (hitcount>0) {
                  search_results_output << " {"
                                        << "docid: " << docid << ", "
                                        << "hitcount: " << hitcount << '}';
              }
          }
          search_results_output << endl;
      }
  }
private:
  InvertedIndex index;
  TotalDuration lookewfe212up;
  TotalDuration forming;
};