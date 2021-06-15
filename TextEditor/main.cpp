#include <iostream>
#include <list>
#include <string>

#include "test_runner.h"

using namespace std::literals;

class Editor {
public:
    Editor() = default;
    // сдвинуть курсор влево
    void Left() {
        if (cursor_ == content_.begin()) {
            return;
        }
        
        --cursor_;
    }
    
    // сдвинуть курсор вправо
    void Right() {
        if (cursor_ == content_.end()) {
            return;
        }
        
        ++cursor_;
    }
    
    // вставить символ token
    void Insert(char token) {
        content_.insert(cursor_, token);
    }
    
    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1) {
        auto iterator_up_to_which_to_cut = SaveToBufferUpToNTokens(tokens);
        
        cursor_ = content_.erase(cursor_, iterator_up_to_which_to_cut);
    }
    
    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1) {
        SaveToBufferUpToNTokens(tokens);
    }
    
    // вставить содержимое буфера в текущую позицию курсора
    void Paste() {
        for (auto it = buffer_.begin(); it != buffer_.end(); ++it) {
            Insert(*it);
        }
    }
    
    // получить текущее содержимое текстового редактора
    std::string GetText() const {
        return {content_.begin(), content_.end()};
    }
    
private:
    // Returns iterator to last saved element from content_
    std::list<char>::iterator SaveToBufferUpToNTokens(size_t tokens) {
        auto iterator_up_to_which_to_cut = cursor_;
        
        for (size_t i = 0; i < tokens; ++i) {
            if (iterator_up_to_which_to_cut == content_.end()) {
                break;
            }
            
            ++iterator_up_to_which_to_cut;
        }
        
        buffer_ = std::list<char>{cursor_, iterator_up_to_which_to_cut};
        
        return iterator_up_to_which_to_cut;
    }
    
private:
    std::list<char> content_;
    std::list<char> buffer_;
    std::list<char>::iterator cursor_ = content_.begin();
};

void TypeText(Editor& editor, const std::string& text) {
    for(char c : text) {
        editor.Insert(c);
    }
}

void TestEditing() {
    {
        Editor editor;
        
        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world"s);
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
        
        ASSERT_EQUAL(editor.GetText(), "world, hello"s);
    }
    {
        Editor editor;
        
        TypeText(editor, "misprnit"s);
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();
        
        ASSERT_EQUAL(editor.GetText(), "misprint"s);
    }
}

void TestReverse() {
    Editor editor;
    
    const std::string text = "esreveR"s;
    for(char c : text) {
        editor.Insert(c);
        editor.Left();
    }
    
    ASSERT_EQUAL(editor.GetText(), "Reverse"s);
}

void TestNoText() {
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), ""s);
    
    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), ""s);
}

void TestEmptyBuffer() {
    Editor editor;
    
    editor.Paste();
    TypeText(editor, "example"s);
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
    
    ASSERT_EQUAL(editor.GetText(), "example"s);
}

int main() {
    RUN_TEST(TestEditing);
    RUN_TEST(TestReverse);
    RUN_TEST(TestNoText);
    RUN_TEST(TestEmptyBuffer);
    
    Editor editor;
    const std::string text = "hello, world"s;
    for (char c : text) {
      editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
      editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
      editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3); // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    std::cout << editor.GetText();
    return 0;
}


