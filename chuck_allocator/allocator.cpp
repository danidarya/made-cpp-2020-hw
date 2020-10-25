#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;
const size_t CHUNK_SIZE = 1024;

struct Chunk {
  uint8_t *ptr_data;
  uint8_t *ptr_first_free;
  size_t free_size;
  Chunk *next;

  Chunk(size_t size) {
    ptr_data = new uint8_t[size];
    ptr_first_free = ptr_data;
    free_size = size;
    next = nullptr;
  }
  ~Chunk() {
    delete[] ptr_data;
  }
};

template <typename T>
class Allocator {
  int counter = 0;
  int *ptr_counter = &counter;
  Chunk *ptr = nullptr;

public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

  template <typename U>
  struct rebind {
    typedef Allocator<T> other;
  };

  Allocator() {
    ptr = new Chunk(CHUNK_SIZE * sizeof(T));
    (*ptr_counter)++;
    cout << "Alloc" << endl;
  }

  Allocator(const Allocator &other) {
    ptr_counter = other.ptr_counter;
    (*ptr_counter)++;
    ptr = other.ptr;
  }

  Allocator &operator=(const Allocator &other) {
    if (this == &other) {
      return *this;
    }
    (*ptr_counter)--;
    if (*ptr_counter == 0) {
      this->~Allocator();
    }
    ptr_counter = other.ptr_counter;
    (*other.ptr_counter)++;
    ptr = other.ptr;
    return *this;
  }

  ~Allocator() {
    if (*ptr_counter == 1 || *ptr_counter == 0) {
      while (ptr->next != nullptr) {
        ptr->~Chunk();
        ptr = ptr->next;
      }
    } else {
      (*ptr_counter)--;
    }
  }

  pointer allocate_n_objects(Chunk *p, size_type n) {
    uint8_t *res = p->ptr_first_free;
    p->free_size -= n * sizeof(T);
    p->ptr_first_free = p->ptr_first_free + n * sizeof(T);
    return (pointer)res;
  }

  pointer allocate(size_type n) {
    if (n * sizeof(T) > CHUNK_SIZE) {
      cerr << "Error. Not enough memory" << endl;
      return nullptr;
    }
    Chunk *ptr_1 = ptr;
    while (ptr_1 != nullptr) {
      if (n * sizeof(T) <= ptr_1->free_size) {
        return allocate_n_objects(ptr_1, n);
      } else {
        ptr_1 = ptr_1->next;
      }
    }
    ptr_1 = ptr;
    ptr = new Chunk(CHUNK_SIZE);
    ptr->next = ptr_1;
    return allocate_n_objects(ptr, n);
  }

  void deallocate(pointer p, const size_type n) {}

  template <typename... Args> void construct(pointer p, const Args &&... args) {
    new (p) T(args...);
  }

  void destroy(pointer p) {
    p->~T();
  }
};

template <typename T>
bool operator==(const Allocator<T> &left, const Allocator<T> &right) {
  return (left.ptr == right.ptr);
}


