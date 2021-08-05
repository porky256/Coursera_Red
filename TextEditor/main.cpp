#include <string>
#include "test_runner.h"
#include <list>
using namespace std;

#pragma once

class Editor {
public:
    // Реализуйте конструктор по умолчанию и объявленные методы
    Editor(){
        buffer_.clear();
        text_.clear();
        position_=text_.begin();
    };

    void Left() {
        if (position_!=text_.begin()) --position_;
    };

    void Right() {
        if (position_!=text_.end()) ++position_;
    };

    void Insert(char token) {
        text_.insert(position_,token);
    };

    void Cut(size_t tokens = 1) {
        if (!tokens) return;
        buffer_.clear();
        auto iter = position_;
        while (tokens--&&iter!=text_.end()) {
            buffer_.push_back(*iter++);
        }
        auto pos = position_;
        --pos;
        text_.erase(position_,iter);
        position_=++pos;
    };

    void Copy(size_t tokens = 1) {
        if (!tokens) return;
        buffer_.clear();
        auto iter = position_;
        while (tokens--&&iter!=text_.end()) {
            buffer_.push_back(*iter++);
        }
    };

    void Paste() {
        auto dist = std::distance(text_.begin(),position_);
        std::list<char> newbuffer(buffer_);
        text_.splice(position_,newbuffer);
        position_=std::next(text_.begin(),dist+buffer_.size());
    };

    string GetText() const {
        string ans;
        ans.reserve(text_.size());
        for (auto x:text_) ans+=x;
        return ans;
    };

private:
    list<char> text_;
    list<char>::iterator position_;
    list<char> buffer_;
};

void TypeText(Editor& editor, const string& text) {
    for(char c : text) {
        editor.Insert(c);
    }
}

void TestPaste() {
    Editor editor;
    for (int i=0;i<10;++i) editor.Insert('a');
    for (int i=0;i<5;++i) editor.Left();
    editor.Cut(5);
    for (int i=0;i<5;++i) editor.Insert('b');
    for (int i=0;i<3;++i) editor.Left();
    editor.Paste();
    editor.Right();editor.Right();editor.Right();
    editor.Paste();
    ASSERT_EQUAL(editor.GetText(), "aaaaabbaaaaabbbaaaaa");
}

void TestEditing() {
    {
        Editor editor;

        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world");
        for(size_t i = 0; i < text_len; ++i) {
            editor.Left();
        }
        editor.Cut(first_part_len);
        for(size_t i = 0; i < text_len - first_part_len; ++i) {
            editor.Right();
        }
        TypeText(editor, ", ");
        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);

        ASSERT_EQUAL(editor.GetText(), "world, hello");
    }
    {
        Editor editor;

        TypeText(editor, "misprnit");
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "misprint");
    }
}

void TestCut()
{
    Editor editor;
    //1
    editor.Cut(10);
    editor.Insert('a');
    editor.Left();
    //2
    editor.Cut(1);
    ASSERT_EQUAL(editor.GetText(), "");
    editor.Paste();
    ASSERT_EQUAL(editor.GetText(), "a");
    //3
    editor.Cut(0);
    editor.Paste();
    ASSERT_EQUAL(editor.GetText(), "a");
    TypeText(editor, "bcde");
    editor.Left();editor.Left();editor.Left();editor.Left();editor.Left();
    //4
    editor.Cut(10);
    ASSERT_EQUAL(editor.GetText(), "");
    editor.Paste();
    ASSERT_EQUAL(editor.GetText(), "abcde");
    editor.Left();editor.Left();editor.Left();editor.Left();editor.Left();
    //5
    editor.Cut(5);
    ASSERT_EQUAL(editor.GetText(), "");
    editor.Paste();
    //6
    editor.Left();editor.Left();editor.Left();editor.Left();editor.Left();
    editor.Cut(1);
    ASSERT_EQUAL(editor.GetText(), "bcde");
    editor.Right();
    editor.Cut(1);
    ASSERT_EQUAL(editor.GetText(), "bde");
    editor.Cut(1);
    editor.Cut(1);
    ASSERT_EQUAL(editor.GetText(), "b");
}

void TestCopy()
{
    Editor editor;
    //1
    editor.Copy(10);
    editor.Insert('a');
    editor.Paste();
    editor.Left();
    ASSERT_EQUAL(editor.GetText(), "a");
    //2
    editor.Copy(1);
    editor.Paste();
    ASSERT_EQUAL(editor.GetText(), "aa");//between a
    //3
    editor.Copy(0);
    editor.Paste();
    ASSERT_EQUAL(editor.GetText(), "aa");
    TypeText(editor, "bcde");
    editor.Insert('d');
    ASSERT_EQUAL(editor.GetText(), "abcdeda");
    editor.Left();editor.Left();editor.Left();
    editor.Cut(2);
    editor.Right();
    editor.Right();
    editor.Paste();
    ASSERT_EQUAL(editor.GetText(), "abcdade");
    editor.Right(); editor.Right(); editor.Right(); editor.Right();
    editor.Insert('t');
    editor.Left();
    editor.Left();editor.Left();editor.Left();editor.Left();editor.Left();editor.Left();editor.Left();
    editor.Insert('t');
    editor.Left();
    //4
    editor.Cut(10);
    ASSERT_EQUAL(editor.GetText(), "");
}

void TestReverse() {
    Editor editor;

    const string text = "esreveR";
    for(char c : text) {
        editor.Insert(c);
        editor.Left();
    }

    ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), "");

    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
    Editor editor;

    editor.Paste();
    TypeText(editor, "example");
    editor.Left();
    editor.Left();
    editor.Paste();
    editor.Right();
    editor.Paste();
    editor.Copy(0);
    editor.Paste();
    editor.Left();
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
    TestRunner tr;
//    RUN_TEST(tr, TestEditing);
//    RUN_TEST(tr, TestReverse);
//    RUN_TEST(tr, TestNoText);
 //   RUN_TEST(tr, TestEmptyBuffer);
 //   RUN_TEST(tr, TestCut);
 //   RUN_TEST(tr,TestCopy);
    RUN_TEST(tr,TestPaste);
    return 0;
}