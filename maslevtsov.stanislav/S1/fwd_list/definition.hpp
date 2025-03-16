#ifndef FWD_LIST_DEFINITION_HPP
#define FWD_LIST_DEFINITION_HPP

#include "declaration.hpp"
#include <utility>

template< typename T >
maslevtsov::FwdList< T >::FwdList() noexcept:
  tail_(nullptr),
  size_(0)
{}

template< typename T >
maslevtsov::FwdList< T >::FwdList(const FwdList& rhs):
  FwdList()
{
  if (rhs.empty()) {
    return;
  }
  push_back(*(rhs.cbegin()));
  for (auto i = ++rhs.cbegin(); i != rhs.cend(); ++i) {
    push_back(*i);
  }
}

template< typename T >
maslevtsov::FwdList< T >::FwdList(FwdList&& rhs) noexcept:
  tail_(std::exchange(rhs.tail_, nullptr)),
  size_(std::exchange(rhs.size_, 0))
{}

template< typename T >
maslevtsov::FwdList< T >::FwdList(std::size_t count, T value):
  FwdList()
{
  if (count == 0) {
    return;
  }
  for (std::size_t i = 0; i != count; ++i) {
    push_front(value);
  }
}

template< typename T >
template< class InputIt >
maslevtsov::FwdList< T >::FwdList(InputIt first, InputIt last):
  FwdList()
{
  if (first == last) {
    return;
  }
  for (; first != last; ++first) {
    push_back(*first);
  }
}

template< typename T >
maslevtsov::FwdList< T >::FwdList(std::initializer_list< T > init):
  FwdList(init.begin(), init.end())
{}

template< typename T >
maslevtsov::FwdList< T >::~FwdList()
{
  clear();
}

template< typename T >
typename maslevtsov::FwdList< T >::FwdList& maslevtsov::FwdList< T >::operator=(const FwdList& rhs) noexcept
{
  FwdList< T > copied_rhs(rhs);
  swap(copied_rhs);
  return *this;
}

template< typename T >
typename maslevtsov::FwdList< T >::FwdList& maslevtsov::FwdList< T >::operator=(FwdList&& rhs) noexcept
{
  FwdList< T > copied_rhs(std::move(rhs));
  swap(copied_rhs);
  return *this;
}

template< typename T >
typename maslevtsov::FwdList< T >::FwdList& maslevtsov::FwdList< T >::operator=(std::initializer_list< T > ilist)
{
  FwdList< T > copied_rhs(std::move(ilist));
  swap(copied_rhs);
  return *this;
}

template< typename T >
void maslevtsov::FwdList< T >::assign(std::size_t count, const T& value)
{
  FwdList< T > assigned(count, value);
  swap(assigned);
}

template< typename T >
template< class InputIt >
void maslevtsov::FwdList< T >::assign(InputIt first, InputIt last)
{
  FwdList< T > assigned(first, last);
  swap(assigned);
}

template< typename T >
void maslevtsov::FwdList< T >::assign(std::initializer_list< T > ilist)
{
  FwdList< T > assigned(std::move(ilist));
  swap(assigned);
}

template< typename T >
T& maslevtsov::FwdList< T >::front() noexcept
{
  assert(!empty() && "element access to empty list");
  return tail_->next_->data_;
}

template< typename T >
const T& maslevtsov::FwdList< T >::front() const noexcept
{
  assert(!empty() && "element access to empty list");
  return tail_->next_->data_;
}

template< typename T >
T& maslevtsov::FwdList< T >::back() noexcept
{
  assert(!empty() && "element access to empty list");
  return tail_->data_;
}

template< typename T >
const T& maslevtsov::FwdList< T >::back() const noexcept
{
  assert(!empty() && "element access to empty list");
  return tail_->data_;
}

template< typename T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::begin() noexcept
{
  return iterator(tail_->next_);
}

template< typename T >
typename maslevtsov::FwdList< T >::const_iterator maslevtsov::FwdList< T >::begin() const noexcept
{
  return const_iterator(tail_->next_);
}

template< typename T >
typename maslevtsov::FwdList< T >::const_iterator maslevtsov::FwdList< T >::cbegin() const noexcept
{
  return const_iterator(tail_->next_);
}

template< typename T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::end() noexcept
{
  return iterator(tail_->next_);
}

template< typename T >
typename maslevtsov::FwdList< T >::const_iterator maslevtsov::FwdList< T >::end() const noexcept
{
  return const_iterator(tail_->next_);
}

template< typename T >
typename maslevtsov::FwdList< T >::const_iterator maslevtsov::FwdList< T >::cend() const noexcept
{
  return const_iterator(tail_->next_);
}

template< typename T >
bool maslevtsov::FwdList< T >::empty() const noexcept
{
  return size_ == 0;
}

template< typename T >
std::size_t maslevtsov::FwdList< T >::size() const noexcept
{
  return size_;
}

template< typename T >
void maslevtsov::FwdList< T >::clear() noexcept
{
  while (!empty()) {
    pop_front();
  }
}

template< typename T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::insert_after(const_iterator pos, const T& value)
{
  FwdListNode< T >* new_node = new FwdListNode< T >{value, pos.node_->next_};
  pos.node_->next_ = new_node;
  ++size_;
  if (pos == cend()) {
    tail_ = new_node;
  }
  return iterator(new_node);
}

template< typename T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::insert_after(const_iterator pos, T&& value)
{
  return insert_after(pos, value);
}

template< typename T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::insert_after(const_iterator pos,
  std::size_t count, const T& value) noexcept
{
  if (count == 0) {
    return iterator(pos.node_);
  }
  splice_after(pos, FwdList< T >(count, value));
  iterator result(pos.node_);
  for (std::size_t i = 0; i != count; ++i, ++result)
  {}
  return result;
}

template< typename T >
template< class InputIt >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::insert_after(const_iterator pos, InputIt first,
  InputIt last) noexcept
{
  if (first == last) {
    return iterator(pos.node_);
  }
  iterator result(pos.node_);
  splice_after(pos, FwdList< T >(first, last));
  for (auto it = ++first; it != last; ++it, ++result)
  {}
  return result;
}

template< typename T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::insert_after(const_iterator pos,
  std::initializer_list< T > ilist) noexcept
{
  return insert_after(pos, ilist.begin(), ilist.end());
}

template< typename T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::erase_after(const_iterator pos) noexcept
{
  assert(!empty() && "empty list erasing");
  if (size_ == 1) {
    delete tail_;
    --size_;
    return iterator();
  }
  if (pos.node_->next_ == tail_) {
    tail_ = pos.node_;
  }
  FwdListNode< T >* to_delete = pos.node_->next_;
  pos.node_->next_ = to_delete->next_;
  delete to_delete;
  --size_;
  return iterator(pos.node_->next_);
}

template< typename T >
typename maslevtsov::FwdList< T >::iterator maslevtsov::FwdList< T >::erase_after(const_iterator first,
  const_iterator last) noexcept
{
  assert(!empty() && "empty list erasing");
  while (first.node_->next_ != last.node_) {
    erase_after(first);
  }
  return iterator(last.node_);
}

template< typename T >
void maslevtsov::FwdList< T >::push_front(const T& value)
{
  FwdListNode< T >* new_node = new FwdListNode< T >{value, nullptr};
  if (empty()) {
    tail_ = new_node;
    tail_->next_ = tail_;
  } else {
    new_node->next_ = tail_->next_;
    tail_->next_ = new_node;
  }
  ++size_;
}

template< typename T >
void maslevtsov::FwdList< T >::push_front(T&& value)
{
  push_front(value);
}

template< typename T >
void maslevtsov::FwdList< T >::push_back(const T& value)
{
  FwdListNode< T >* new_node = new FwdListNode< T >{value, nullptr};
  if (empty()) {
    tail_ = new_node;
    tail_->next_ = tail_;
  } else {
    new_node->next_ = tail_->next_;
    tail_->next_ = new_node;
    tail_ = new_node;
  }
  ++size_;
}

template< typename T >
void maslevtsov::FwdList< T >::push_back(T&& value)
{
  push_back(value);
}

template< typename T >
void maslevtsov::FwdList< T >::pop_front() noexcept
{
  assert(!empty() && "pop_front() to empty list");
  FwdListNode< T >* to_delete = tail_->next_;
  tail_->next_ = to_delete->next_;
  delete to_delete;
  --size_;
}

template< typename T >
void maslevtsov::FwdList< T >::swap(FwdList& other) noexcept
{
  std::swap(tail_, other.tail_);
  std::swap(size_, other.size_);
}

template< typename T >
void maslevtsov::FwdList< T >::splice_after(const_iterator pos, FwdList& other) noexcept
{
  splice_after(pos, other, other.cbegin(), other.cend());
}

template< typename T >
void maslevtsov::FwdList< T >::splice_after(const_iterator pos, FwdList&& other) noexcept
{
  splice_after(pos, other);
}

template< typename T >
void maslevtsov::FwdList< T >::splice_after(const_iterator pos, FwdList& other, const_iterator it) noexcept
{
  splice_after(pos, other, it, ++(++const_iterator(it.node_)));
}

template< typename T >
void maslevtsov::FwdList< T >::splice_after(const_iterator pos, FwdList& other, const_iterator first,
  const_iterator last) noexcept
{
  if (last == const_iterator(first.node_->next_) || other.empty()) {
    return;
  }
  std::size_t size_increase = 0;
  if (other.cbegin() == first && other.cend() == last) {
    size_increase = other.size_;
  } else {
    for (auto it = ++first; it != last; ++it, ++size_increase)
    {}
  }
  FwdListNode< T >* pos_tmp = pos.node_;
  FwdListNode< T >* first_tmp = first.node_;
  FwdListNode< T >* last_tmp = last.node_;
  if (pos_tmp == tail_) {
    tail_ = last_tmp;
  }
  if (last_tmp == other.tail_->next_) {
    other.tail_ = first_tmp;
  }
  last.node_->next_ = pos.node_->next_;
  pos_tmp->next_ = first.node_->next_;
  first_tmp->next_ = last_tmp->next_;
}

template< typename T >
void maslevtsov::FwdList< T >::remove(const T& value) noexcept
{
  assert(!empty() && "removing from empty list");
  remove_if([&](const T& list_value)
  {
    return list_value == value;
  });
}

template< typename T >
template< class UnaryPredicate >
void maslevtsov::FwdList< T >::remove_if(UnaryPredicate condition)
{
  assert(!empty() && "removing from empty list");
  auto it = ++cbegin();
  while (it != cend()) {
    if (condition(it.node_->next_->data_)) {
      erase_after(it);
    } else {
      ++it;
    }
  }
  if (condition(*++it)) {
    erase_after(const_iterator(tail_->next_));
  }
}

#endif
