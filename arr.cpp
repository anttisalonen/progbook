#include <iostream>
#include <array>
#include <set>
#include <vector>
#include <algorithm>

int main()
{
	bool found;
	int size;

	// C type array
	int old_type_array[5] = {2, 3, 4, -1, -1};
	found = false;
	for(int i = 0; i < 5; i++) {
		if(old_type_array[i] == 3) {
			found = true;
			break;
		}
	}
	size = 0;
	for(int i = 0; i < 5; i++) {
		if(old_type_array[i] != -1) {
			size++;
		}
	}
	std::cout << "Found: " << found << "; size: " << size << "\n";

	// C++ has a specific array type
	std::array<int, 3> arr;
	arr[0] = 2;
	arr[1] = 3;
	arr[2] = 4;
	found = std::find(arr.begin(), arr.end(), 3) != arr.end();
	size = arr.size();
	std::cout << "Found: " << found << "; size: " << size << "\n";
	// printing all values in the array:
	for(const auto& v : arr) {
		std::cout << v << "\n";
	}

	// C++ vector
	std::vector<int> vec;
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	found = std::find(vec.begin(), vec.end(), 3) != vec.end();
	size = vec.size();
	std::cout << "Found: " << found << "; size: " << size << "\n";
	// vec.clear() would remove all elements in the vector
	// The following line removes the element 3 from the vector
    vec.erase(std::remove(vec.begin(), vec.end(), 3), vec.end());
	std::cout << "Size: " << vec.size() << "\n";

	// printing all values in the vector:
	for(const auto& v : vec) {
		std::cout << v << "\n";
	}

	// C++ set
	std::set<int> s;
	s.insert(2);
	s.insert(3);
	s.insert(4);
	found = s.find(3) != s.end();
	size = s.size();
	std::cout << "Found: " << found << "; size: " << size << "\n";
	// s.clear() would remove all elements in the set
	s.erase(3);
	std::cout << "Size: " << s.size() << "\n";

	// printing all values in the set:
	for(const auto& v : s) {
		std::cout << v << "\n";
	}

	return 0;
}

