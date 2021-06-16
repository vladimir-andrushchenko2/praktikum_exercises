#include <iostream>
#include <cassert>
#include <string_view>

class Translator {
public:
    void Add(string_view source, string_view target);
    string_view TranslateForward(string_view source) const;
    string_view TranslateBackward(string_view target) const;

private:
    // ...
}

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"), string("window"));
    translator.Add(string("stol"), string("table"));
    
    assert(translator.TranslateForward("okno") == "window");
    assert(translator.TranslateBackward("table") == "stol");
    assert(translator.TranslateBackward("stol") == "");
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
