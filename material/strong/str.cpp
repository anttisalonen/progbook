#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <string>

int main()
{
    std::string s("this is a string.\n");
    std::vector<std::string> res;
    std::istringstream iss(s);
    std::copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         std::back_inserter(res));

    for(const auto& w : res) {
        std::cout << w << "\n";
    }
}

