#include "vector.hpp"
#include "map.hpp"
#include "stack.hpp"
#include <iostream>
#include <vector>

// minimal C++11 allocator with debug output

template <class Tp>
struct NAlloc : std::allocator<Tp> {
  typedef Tp value_type;
  NAlloc()
  {
    std::cout << "constructor called\n";
  };
  template <class T> NAlloc(const NAlloc<T>&) {}

  Tp* allocate(std::size_t n)
  {
    n *= sizeof(Tp);
    Tp* p = static_cast<Tp*>(::operator new(n));
    std::cout << "allocating " << n << " bytes @ " << p << '\n';
    return p;
  }

  void deallocate(Tp* p, std::size_t n)
  {
    std::cout << "deallocating " << n*sizeof*p << " bytes @ " << p << "\n\n";
    ::operator delete(p);
  }
};
template <class T, class U>
bool operator==(const NAlloc<T>&, const NAlloc<U>&) { return true; }
template <class T, class U>
bool operator!=(const NAlloc<T>&, const NAlloc<U>&) { return false; }

int main()
{
  int max_elements = 32;

  std::cout << "using reserve: \n";
  {
    std::cout << "std:\n";
    std::vector<int, NAlloc<int> > v3;
    std::cout << "ft:\n";
    ft::vector<int, NAlloc<int> > v1;

    v1.reserve( max_elements );
    v3.reserve( max_elements );

    for(int n = 0; n < max_elements; ++n)
      v1.push_back(n);
  }

  std::cout << "not using reserve: \n";
  {
    ft::vector<int, NAlloc<int> > v1;

    for(int n = 0; n < max_elements; ++n) {
      if(v1.size() == v1.capacity()) {
        std::cout << "size() == capacity() == " << v1.size() << '\n';
      }
      v1.push_back(n);
    }
  }
}
