#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <string>

template<typename T>
auto filt(const std::vector<T>& arr)
{
	std::vector<T> res;
	std::copy_if(arr.begin(),
			arr.end(),
			std::back_inserter(res),
			[](T i) { return i < 0; });
	return res;
}

int main()
{
    std::vector<int> inp = {3, -1, 4, -2, 5, -3, 6, -4};
    auto res = filt(inp);
    for(const auto& i : res) {
        std::cout << i << "\n";
    }
}

