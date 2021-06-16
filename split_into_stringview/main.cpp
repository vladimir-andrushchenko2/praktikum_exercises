#include <vector>
#include <iostream>
#include <string_view>
#include <cassert>

using namespace std;

vector<string_view> SplitIntoWordsView(string_view str) {
    vector<string_view> result;
    const int64_t pos = 0;
    const int64_t pos_end = str.npos;
    while (true) {
        int64_t space = str.find(' ', pos);
        result.push_back(str.substr(pos, space));
        if (space == pos_end) {
            break;
        } else {
            str.remove_prefix(space + 1);
        }
    }

    return result;
}


int main(int argc, const char * argv[]) {
    vector<string_view> expected_output = {"Hello"sv, "World"sv};
    
    auto output = SplitIntoWordsView("Hello World"s);
    
    assert(expected_output == output);
    std::cout << "all good" << std::endl;
    return 0;
}
