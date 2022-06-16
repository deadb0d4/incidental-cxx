/*
 * taken from github.com/lewissbaker/cppcoro
 */
#pragma once

#include <exception>
#include <coroutine>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <traits/concepts.hpp>

namespace coro {

/// @brief: single pass generator to pass a view,
/// use only for looping once
template <traits::IsSimple T>
class SinglePass {
 private:
  template <traits::IsSimple S>
  struct SimpleInputIterator {
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = S;
    using reference = const S&;
    using pointer = const S*;
  };

 public:
  struct promise_type {
   private:
    friend class SinglePass<T>;

    void RethrowIf() const {
      if (ptr) {
        std::rethrow_exception(ptr);
      }
    }

    const T* GetValue() const {
      return value;
    }

   public:
    SinglePass get_return_object() {
      return {std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    std::suspend_never initial_suspend() const noexcept {
      return {};
    }

    std::suspend_always final_suspend() const noexcept {
      return {};
    }

    void unhandled_exception() {
      ptr = std::current_exception();
    }

    void return_void() {
    }

    std::suspend_always yield_value(const T& t) {
      value = std::addressof(t);
      return {};
    }

   private:
    const T* value;
    std::exception_ptr ptr;
  };

  using coro_handle = std::coroutine_handle<promise_type>;

  SinglePass(coro_handle h) : handle(h) {
  }

  ~SinglePass() {
    if (handle) {
      handle.destroy();
    }
  }

  SinglePass(SinglePass&& gen) {
    std::swap(handle, gen.handle);
  }

  SinglePass(const SinglePass&) = delete;

  class Iterator : public SimpleInputIterator<T> {
   public:
    struct End : public SimpleInputIterator<T> {
      End& operator++() {
        throw std::out_of_range("SinglePass end iterator");
      }

      void operator++(int) {
        throw std::out_of_range("SinglePass end iterator");
      }

      typename End::reference operator*() const {
        throw std::out_of_range("SinglePass end iterator");
      }

      typename End::pointer operator->() const {
        throw std::out_of_range("SinglePass end iterator");
      }
    };

    Iterator(SinglePass<T>& g) : owner(g) {
    }

    Iterator(Iterator&& other) noexcept {
      owner = other.owner;
    }

    Iterator& operator=(Iterator&& other) noexcept {
      owner = other.owner;
      return *this;
    }

    friend bool operator==(const Iterator& it, End) noexcept {
      return it.owner.Done();
    }

    friend bool operator!=(const Iterator& it, End s) noexcept {
      return !(it == s);
    }

    friend bool operator==(End s, const Iterator& it) noexcept {
      return (it == s);
    }

    friend bool operator!=(End s, const Iterator& it) noexcept {
      return it != s;
    }

    Iterator& operator++() {
      owner.Next();
      return *this;
    }

    void operator++(int) {
      operator++();
    }

    typename Iterator::reference operator*() const noexcept {
      return *owner.Get();
    }

    typename Iterator::pointer operator->() const noexcept {
      return owner.Get();
    }

   private:
    SinglePass<T>& owner;
  };

  Iterator begin() {
    handle.promise().RethrowIf();
    return Iterator{*this};
  }

  typename Iterator::End end() {
    return {};
  }

  bool Done() const {
    return handle.done();
  }

  const T* Get() const {
    if (Done()) {
      return nullptr;
    }
    return handle.promise().GetValue();
  }

  void Next() {
    if (Done()) {
      return;
    }
    handle();
    handle.promise().RethrowIf();
  }

 private:
  friend class SinglePass::Iterator;

  coro_handle handle{};
};

template <
    traits::Iterable T,
    class V = std::remove_cvref_t<decltype(*T().begin())>>
coro::SinglePass<V> Loop(const T& t) {
  for (const auto& v : t) {
    co_yield v;
  }
};

}  // namespace coro
