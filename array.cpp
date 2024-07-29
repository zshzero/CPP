#include <iostream>

template<typename T, size_t s>
class Array {
private:
  T data[s];
public:
  size_t const Size() const { return s; }
  T& operator[](size_t index) { return data[index]; }
  const T& operator[] (size_t index) const { return data[index]; }
};

int main() {
  Array<int, 5> a;
  for(size_t i = 0; i < a.Size(); i++) {
    std::cout << a[i];
    a[i] = i;
    std::cout << a[i] + 1 << std::endl;
  }

  const Array<int, 5> b = a;
  for(size_t i = 0; i < b.Size(); i++) {
    std::cout << b[i];
    // b[i] = i; 
    std::cout << b[i] + 1 << std::endl;
  }

  Array<std::string, 2> c;
  c[0] = "abc";
  c[1] = "def";
  for(size_t i = 0; i < c.Size(); i++) {
    std::cout << c[i] << std::endl;
  }
}