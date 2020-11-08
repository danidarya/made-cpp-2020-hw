#pragma once

#include <algorithm>
#include <stdexcept>

namespace task {
class NullPtrException : public std::exception {};

template<class T>
class UniquePtr {
  T *ptr = nullptr;

public:
  UniquePtr<T>() : ptr(nullptr) {}

  explicit UniquePtr<T>(T *p) : ptr(p) {}

  UniquePtr<T>(UniquePtr<T> &other) = delete;

  UniquePtr<T> &operator=(UniquePtr<T> &other) = delete;

  UniquePtr<T>(UniquePtr<T> &&other) noexcept : ptr(other.ptr) {
    other.ptr = nullptr;
  }

  UniquePtr<T> &operator=(UniquePtr<T> &&other) noexcept {
    if (this != &other) {
      delete ptr;
      ptr = other.ptr;
      other.ptr = nullptr;
    }
    return *this;
  }

  ~UniquePtr() {
    delete ptr;
  }

  T &operator*() {
    if (ptr) {
      return *ptr;
    }
    throw NullPtrException();
  }

  T *operator->() {
    if (ptr) {
      return ptr;
    }
    throw NullPtrException();
  }

  T *get() {
    if (!ptr) {
      return nullptr;
    }
    return &(*ptr);
  }

  void release() {
    ptr = nullptr;
  }

  void reset(T *other) {
    T *old_ptr = ptr;
    ptr = other;
    if (old_ptr) {
      delete old_ptr;
    }
  }

  void swap(UniquePtr<T> &other) {
    std::swap(ptr, other.ptr);
  }
};


template<class T>
class WeakPtr;

template<class T>
class SharedPtr {
  T *ptr;
  size_t *sh_counter;
  size_t *w_counter;

public:
  template<typename U>
  friend class WeakPtr;
  SharedPtr<T>() : ptr(nullptr), sh_counter(nullptr), w_counter(nullptr) {}

  explicit SharedPtr<T>(T *p) : ptr(p), sh_counter(new size_t(1)), w_counter(new size_t(0)) {}

  explicit SharedPtr<T>(const WeakPtr<T> &other) : ptr(other.ptr), sh_counter(other.sh_counter), w_counter(other.w_counter) {
    (*w_counter)--;
    (*sh_counter)++;
  }

  SharedPtr<T>(const SharedPtr<T> &other) : sh_counter(other.sh_counter), ptr(other.ptr), w_counter(other.w_counter) {
    if (sh_counter) {
      (*sh_counter)++;
    } else {
      sh_counter = new size_t(1);
    }
  }

  SharedPtr<T>(SharedPtr<T> &&other) noexcept : sh_counter(other.sh_counter), ptr(other.ptr), w_counter(other.w_counter) {
    other.ptr = nullptr;
    other.sh_counter = nullptr;
    other.w_counter = nullptr;
  }

  SharedPtr<T> &operator=(const SharedPtr<T> &other) {
    if (this != &other) {
      if (sh_counter) {
        if (*sh_counter == 1) {
          delete sh_counter;
          delete ptr;
          delete w_counter;
        } else {
          (*sh_counter)--;
        }
      }
      ptr = other.ptr;
      sh_counter = other.sh_counter;
      w_counter = other.w_counter;
      (*sh_counter)++;
    }
    return *this;
  }

  SharedPtr<T> &operator=(SharedPtr<T> &&other) noexcept {
    if (this != &other) {
      if (sh_counter) {
        if (*sh_counter != 1) {
          (*sh_counter)--;
        } else {
          delete sh_counter;
          delete w_counter;
          delete ptr;
        }
      }
      ptr = other.ptr;
      sh_counter = other.sh_counter;
      w_counter = other.w_counter;
      other.ptr = nullptr;
      other.sh_counter = nullptr;
      other.w_counter = nullptr;
    }
    return *this;
  }

  ~SharedPtr() {
    if (sh_counter) {
      if (*sh_counter == 1) {
        delete ptr;
        (*sh_counter)--;
        if (!w_counter || *w_counter == 0) {
          delete sh_counter;
        }

      } else {
        (*sh_counter)--;
      }
    }
  }

  T &operator*() {
    if (ptr) {
      return *ptr;
    }
    throw NullPtrException();
  }

  T *operator->() {
    if (ptr) {
      return ptr;
    }
    throw NullPtrException();
  }

  T *get() {
    if (!ptr) {
      return nullptr;
    }
    return &(*ptr);
  }

  size_t use_count() {
    if (sh_counter) {
      return *sh_counter;
    } else
      return 0;
  }

  void reset() {
    if (ptr) {
      delete ptr;
    }
    ptr = nullptr;
  }

  void reset(T *other) {
    T *old_ptr = ptr;
    ptr = other;
    if (old_ptr) {
      if (*sh_counter == 1) {
        delete old_ptr;
      } else {
        (*sh_counter)--;
      }
    }
  }

  void swap(SharedPtr<T> &other) {
    std::swap(ptr, other.ptr);
    std::swap(w_counter, other.w_counter);
    std::swap(sh_counter, other.sh_counter);
  }
};


template<class T>
class WeakPtr {
  T *ptr;
  size_t *sh_counter;
  size_t *w_counter;

public:
  template<typename U>
  friend class SharedPtr;
  WeakPtr() : ptr(nullptr), w_counter(nullptr), sh_counter(nullptr) {}

  WeakPtr<T>(const SharedPtr<T> &other) : ptr(other.ptr), w_counter(other.w_counter), sh_counter(other.sh_counter) {
    (*other.w_counter)++;
  }

  WeakPtr<T>(const WeakPtr<T> &other) : w_counter(other.w_counter), ptr(other.ptr), sh_counter(other.sh_counter) {
    (*w_counter)++;
  }

  WeakPtr<T>(WeakPtr<T> &&other) noexcept : w_counter(other.w_counter), ptr(other.ptr), sh_counter(other.sh_counter) {
    other.ptr = nullptr;
    other.w_counter = nullptr;
    other.sh_counter = nullptr;
  }

  ~WeakPtr() {
    if (w_counter) {
      (*w_counter)--;
    }
    sh_counter = nullptr;
  }

  WeakPtr<T> &operator=(const WeakPtr<T> &other) {
    if (this != &other) {
      (*w_counter)--;
      ptr = other.ptr;
      sh_counter = other.sh_counter;
      w_counter = other.w_counter;
      (*w_counter)++;
    }
    return *this;
  }

  WeakPtr<T> &operator=(WeakPtr<T> &&other) noexcept {
    if (this != &other) {
      (*w_counter)--;
      ptr = other.ptr;
      w_counter = other.w_counter;
      sh_counter = other.sh_counter;
      other.ptr = nullptr;
      other.w_counter = nullptr;
      other.sh_counter = nullptr;
    }
    return *this;
  }

  WeakPtr<T> &operator=(const SharedPtr<T> &other) {
    if (w_counter) {
      (*w_counter)--;
    }
    ptr = other.ptr;
    (*other.w_counter)++;
    w_counter = other.w_counter;
    sh_counter = other.sh_counter;
    return *this;
  }

  bool expired() {
    return (*sh_counter == 0);
  }

  size_t use_count() {
    return sh_counter ? *sh_counter : 0;
  }

  SharedPtr<T> lock() {
    return SharedPtr<T>(*this);
  }

  void reset(T *p) {
    if (w_counter) {
      (*w_counter)--;
    }
    ptr = p;
    sh_counter = nullptr;
    w_counter = new size_t(1);
  }

  void swap(WeakPtr<T> &other) {
    std::swap(ptr, other.ptr);
    std::swap(w_counter, other.w_counter);
    std::swap(sh_counter, other.sh_counter);
  }
};
}// namespace task

//#include "smart_pointers.tpp"
