#include <cstddef>
#include <iostream>

template <typename T> class Node {
public:
  Node(T value) : value_(value), next_(nullptr) {}

  // Destructor isn't necessary as value_ is primitive type
  // and next_ doesn't manage its own memory

  T value_;
  Node<T> *next_;
};

template <typename T> class LinkedList {
public:
  LinkedList(T value = T()) {
    Node<T> *node = new Node<T>(value);
    head_ = node;
    tail_ = node;
    length_ = 1;
  }

  ~LinkedList() {
    std::cout << "Destruct LinkedList\n";
    Node<T> *current_node = head_;
    while (head_ != nullptr) {
      head_ = head_->next_;
      delete current_node;
      current_node = head_;
    }
  }

  void Append(T value) {
    Node<T> *new_node = new Node<T>(value);

    if (length_ == 0) {
      head_ = new_node;
      tail_ = new_node;
    } else {
      tail_->next_ = new_node;
      tail_ = new_node;
    }

    ++length_;
  }

  void Pop() {
    if (length_ == 0) {
      return;
    }

    if (length_ == 1) {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
      --length_;
      return;
    }

    Node<T> *front_node = head_;
    Node<T> *back_node = head_;
    while (front_node != tail_) {
      back_node = front_node;
      front_node = front_node->next_;
    }
    tail_ = back_node;
    tail_->next_ = nullptr;
    delete front_node;
    --length_;
  }

  void Prepend(T value) {
    Node<T> *new_node = new Node<T>(value);

    if (length_ == 0) {
      head_ = new_node;
      tail_ = new_node;
    } else {
      new_node->next_ = head_;
      head_ = new_node;
    }

    ++length_;
  }

  void PopFirst() {
    if (length_ == 0) {
      return;
    }

    if (length_ == 1) {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
      --length_;
      return;
    }

    Node<T> *current_node = head_;
    head_ = head_->next_;
    delete current_node;
    --length_;
  }

  void Print() const {
    std::cout << "\n========================================\n";
    Node<T> *current_node = head_;
    while (current_node != nullptr) {
      std::cout << current_node->value_ << "\t";
      current_node = current_node->next_;
    }
    std::cout << "\n========================================\n\n";
  }

  Node<T> *GetByIndex(size_t index) const {
    if (index < 0 || index >= length_) {
      std::cout << "Invalid Index\n";
      return nullptr;
    }

    Node<T> *current_node = head_;
    for (size_t i = 0; i < index; i++) {
      current_node = current_node->next_;
    }
    return current_node;
  }

  Node<T> *SetByIndex(T value, size_t index) {
    Node<T> *current_node = GetByIndex(index);

    if (current_node != nullptr) {
      current_node->value_ = value;
    }

    return current_node;
  }

  void Insert(T value, size_t index) {
    if (index == 0) {
      Prepend(value);
      return;
    }

    if (index == length_) {
      Append(value);
      return;
    }

    Node<T> *new_node = new Node<T>(value);
    Node<T> *previous_node = GetByIndex(index - 1);

    new_node->next_ = previous_node->next_;
    previous_node->next_ = new_node;
    ++length_;
  }

  void Delete(size_t index) {
    if (index == 0) {
      PopFirst();
      return;
    }

    if (index == length_ - 1) {
      Pop();
      return;
    }

    Node<T> *previous_node = GetByIndex(index - 1);
    Node<T> *index_node = previous_node->next_;
    previous_node->next_ = index_node->next_;
    delete index_node;
    --length_;
  }

  void Reverse() {
    // Swap head and tail
    Node<T> *temp;
    temp = head_;
    head_ = tail_;
    tail_ = temp;

    // Get 2 nodes that tail is pointing to
    Node<T> *previous_node = tail_->next_;
    Node<T> *previous_node_next = previous_node->next_;

    Node<T> *current_node = tail_;
    for (size_t i = 0; i < length_ - 1; i++) {
      // Save previous node's next
      previous_node_next = previous_node->next_;
      // Point previous node to current node
      previous_node->next_ = current_node;
      // Move places
      current_node = previous_node;
      previous_node = previous_node_next;
    }
    tail_->next_ = nullptr;
  }

private:
  Node<T> *head_;
  Node<T> *tail_;
  size_t length_;
};

int main() {
  LinkedList<int> *l = new LinkedList<int>(1);
  l->Append(2);
  l->Print();

  l->Pop();
  l->Pop();
  l->Pop();
  l->Print();

  l->Prepend(2);
  l->Prepend(1);
  l->Print();

  l->PopFirst();
  l->PopFirst();
  l->PopFirst();
  l->Print();

  l->Insert(0, 0);
  l->Insert(2, 1);
  l->Print();

  std::cout << l->GetByIndex(0) << std::endl;
  std::cout << l->SetByIndex(1, 1) << std::endl;

  l->Delete(1);
  l->Print();

  l->Append(1);
  l->Append(2);
  l->Append(3);
  l->Print();
  l->Reverse();
  l->Print();

  delete l;
  return 0;
}