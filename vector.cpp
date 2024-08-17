#include <cmath>
#include <iostream>
#include <string>
#include <utility>

template <typename T> class Vector {
public:
  Vector() {}

  ~Vector() {
    // delete[] m_data_;
    // calling destructor manually for each element and
    // deallocating vector memory
    Clear();
    ::operator delete(m_data_);
  }

  // index operator for vector
  const T &operator[](size_t index) const { return m_data_[index]; }
  T &operator[](size_t index) { return m_data_[index]; }

  size_t const Size() const { return m_size_; }

  // just lvalue reference doesn't make sense as value is copied rather than
  // referenced even when variable is reference type.(=) operator in C++
  // typically performs a copy operation
  void PushBack(const T /*const lvalue reference*/ &value) {
    ReAllocate();
    // Referencing the original value directly in the array could lead to issues
    // with object lifetimes. If the original value goes out of scope or is
    // destroyed, the reference in the array would become invalid
    m_data_[m_size_++] = value;
  }

  // accepts rvalue value
  void PushBack(T /*rvalue reference*/ &&value) { // becomes lvalue again here
    ReAllocate(true);
    // std::move changes behavior from copying to moving during assignment
    // compiler will treat value as an lvalue if you don’t use std::move
    // m_data_[m_size_++] = std::move(value);
    // constructing object in passed memory using Placement new operator
    new (&m_data_[m_size_++]) T(std::move(value));
  }

  void PopBack() {
    if (m_size_ <= 0) {
      return;
    }
    m_data_[m_size_--].~T();
    ReAllocate(false);
  }

  void Clear() {
    for (size_t i = 0; i < m_size_; i++) {
      m_data_[i].~T();
    }
    m_size_ = 0;
  }

  template <typename... Args> T &EmplaceBack(Args &&...args) {
    ReAllocate(true);
    // constructing the object in stack and moving it
    // m_data_[m_size_++] = T(std::forward<Args>(args)...);

    // constructing inplace (placement new operator) without needing to move
    new (&m_data_[m_size_]) T(std::forward<Args>(args)...);
    return m_data_[m_size_++];
  }

private:
  T *m_data_ = nullptr;
  size_t m_capacity_ = 0;
  size_t m_size_ = 0;
  size_t m_resize_power = 0;

  const size_t m_resize_base = 2;

  void ReAllocate(bool is_addition) {
    // Check realloacation and calculate new capacity if necessary
    bool is_resize = false;
    if (is_addition) {
      // Check if it has enough capacity
      is_resize = m_size_ + 1 > m_capacity_;
      m_resize_power = std::ceil(std::log2(m_size_ + 1));
    } else {
      // Check if it has too much capacity
      is_resize = m_capacity_ >= std::pow(m_resize_base, m_resize_power);
      m_resize_power = std::ceil(std::log2(m_size_));
    }

    if (!is_resize) {
      return;
    }

    m_capacity_ = std::pow(m_resize_base, m_resize_power);

    // create array with new capacity and move the elements
    // T *new_block = new T[m_capacity_];
    // changing the way we allocate
    // Object initialization isn't required. Only memory needs to be reserved
    T *new_block = (T *)(operator new(m_capacity_ * sizeof(T)));

    for (size_t i = 0; i < m_size_; i++) {
      new (&new_block[i]) T(std::move(m_data_[i]));
      // calling destructor manually after move as delete keyword is not used
      m_data_[i].~T();
    }

    // deallocates previous memory and point to the new one
    // delete[] m_data_;
    // changing the way we deallocate
    // Clearing only memory and not calling destructor
    ::operator delete(m_data_);
    m_data_ = new_block;
  }
};

struct Point {
  float x, y, z;
  int *memory_block;

  Point() : x(0), y(0), z(0) { memory_block = new int[5]; }
  Point(float x, float y, float z) : x(x), y(y), z(z) {
    memory_block = new int[5];
  }

  // Deleting copy constructor
  Point(const Point &other) = delete;
  Point &operator=(const Point &other) = delete;

  Point(Point &&other) : x(other.x), y(other.y), z(other.z) {
    memory_block = other.memory_block;
    other.memory_block = nullptr;
    std::cout << "Move\n";
  }
  Point &operator=(Point &&other) {
    x = other.x;
    y = other.y;
    z = other.z;
    memory_block = other.memory_block;
    other.memory_block = nullptr;
    std::cout << "Move\n";
    return *this;
  }

  ~Point() {
    std::cout << "Destroy\n";
    delete[] memory_block;
  }

  friend std::ostream &operator<<(std::ostream &os, const Point &value) {
    std::cout << value.x << "\t" << value.y << "\t" << value.z;
    return os;
  }
};

template <typename T> void PrintVector(const Vector<T> &v) {
  std::cout << "---------------------------------------\n";
  for (size_t i = 0; i < v.Size(); i++) {
    std::cout << v[i] << std::endl;
  }
  std::cout << "---------------------------------------\n";
}

int main() {
  Vector<std::string> v;
  v.PushBack("a");
  v.PushBack("b");
  v.PushBack("c");
  PrintVector(v);
  v.PopBack();
  PrintVector(v);
  v.Clear();
  PrintVector(v);

  Vector<Point> v1;
  v1.PushBack(Point());
  v1.PushBack(Point(1, 2, 3));
  v1.PopBack();
  PrintVector(v1);

  v1.EmplaceBack(0, 0, 0);
  v1.EmplaceBack(1, 2, 3);
  v1.EmplaceBack(4, 5, 6);
  PrintVector(v1);

  return 0;
}