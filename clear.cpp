#include <algorithm>
#include <iostream>
#include <vector>
 
void print(const int &n){ std::cout << " " << n; }
void clear()
{
    std::vector<int> container;
 
    container.push_back(1); 
    container.push_back(2); 
    container.push_back(3); 
    std::cout << "Before clear:";
    std::for_each(container.begin(), container.end(), print);
    std::cout << "\nSize=" << container.size() << ", Capacity=" << container.capacity() << '\n';
 
    std::cout << "Clear\n";
    container.clear();
 
    std::cout << "After clear:";
    std::for_each(container.begin(), container.end(), print);
    std::cout << "\nSize=" << container.size() << ", Capacity=" << container.capacity() << '\n';
}
