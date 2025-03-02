#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include "iteratorc.hpp"
#include "iterator.hpp"

namespace averenkov
{
  template< class T >
  class List
  {

  public:
    List();
    List(const List< T >&);
    List(List< T >&&);
    List(size_t, const T&);
    ~List();

    using Iter = ListIterator< T >;
    using Iterc = ConstListIterator< T >;

    Iter begin() const;
    Iter end() const;

    Iterc cbegin() const;
    Iterc cend() const;

    T& front();
    T& back();

    bool empty();

    size_t size() const;

    void push_front(const T&);
    void push_back(const T&);
    void pop_front();
    void pop_back();
    void swap(List&);
    void clear();

    void fill(Iter first, Iter last, const T&);

    void remove(const T&);
    template< class UnaryPredicate >
    void removeIf(UnaryPredicate);

    void splice(Iterc pos, List< T >&);
    void splice(Iterc pos, List< T >&&);
    void splice(Iterc pos, List< T >&, Iterc it);
    void splice(Iterc pos, List< T >&&, Iterc it);
    void splice(Iterc pos, List< T >&, Iterc first, Iterc last);
    void splice(Iterc pos, List< T >&&, Iterc first, Iterc last);

    void assign(size_t, const T&);

  private:

    Node< T >* fake_;
    Node< T >* tail_;
    size_t size_;

  };

  template < class T >
  List< T >::List()
    : size_(0)
  {
    fake_ = new Node< T >(T());
    fake_->next_ = fake_;
  }

  template < class T >
  List< T >::List(const List< T >& rhs)
    : List()
  {
    Node< T >* current = rhs.fake_->next_;
    while (current != rhs.fake_)
    {
      push_back(current->data_);
      current = current->next_;
    }
  }

  template < class T >
  List< T >::List(List< T >&& rhs): List()
  {
    swap(rhs);
  }

  template < class T >
  List< T >::List(size_t count, const T& value): List()
  {
    while (count--)
    {
      push_back(value);
    }
  }

  template < class T >
  List< T >::~List()
  {
    clear();
    delete fake_;
  }

  template < class T >
  size_t List<T>::size() const
  {
    return size_;
  }

  template < class T >
  bool List<T>::empty()
  {
    return size_ == 0;
  }

  template < class T >
  typename List< T >::Iter List< T >::begin() const
  {
    return Iter(fake_->next_);
  }

  template < class T >
  typename List< T >::Iter List< T >::end() const
  {
    return Iter(fake_);
  }

  template < class T >
  typename List< T >::Iterc List<T>::cbegin() const
  {
    return Iterc(fake_->next_);
  }

  template < class T>
  typename List< T >::Iterc List<T>::cend() const
  {
    return Iterc(fake_);
  }

  template < class T >
  T& List< T >::front()
  {
    assert(!empty());
    return fake_->next_->data_;
  }

  template < class T >
  T& List< T >::back()
  {
    assert(!empty());
    Node< T >* current = fake_;
    while (current->next_ != fake_)
    {
      current = current->next_;
    }
    return current->data_;
  }

  template < class T >
  void List< T >::push_front(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    newNode->next_ = fake_->next_;
    fake_->next_ = newNode;
    size_++;
  }

  template < class T >
  void List< T >::push_back(const T& value)
  {
    Node< T >* newNode = new Node< T >(value);
    newNode->next_ = fake_;
    Node< T >* current = fake_;
    while (current->next_ != fake_)
    {
      current = current->next_;
    }
    current->next_ = newNode;
    size_++;
  }

  template < class T >
  void List< T >::pop_front()
  {
    assert(!empty());
    Node< T >* temp = fake_->next_;
    fake_->next_ = temp->next_;
    delete temp;
    size_--;
  }

  template < class T >
  void List< T >::pop_back()
  {
    assert(!empty());
    Node< T >* current = fake_;
    while (current->next_->next_ != fake_)
    {
      current = current->next_;
    }
    delete current->next_;
    current->next_ = fake_;
    size_--;
  }

  template < class T >
  void List< T >::clear()
  {
    while (!empty())
    {
      pop_front();
    }
  }

  template < class T >
  void List< T >::swap(List& rhs)
  {
    std::swap(fake_, rhs.fake_);
    std::swap(size_, rhs.size_);
  }

  template < class T >
  void List< T >::remove(const T& value)
  {
    Node< T >* current = fake_;
    while (current->next_ != fake_)
    {
      if (current->next_->data_ == value)
      {
        Node< T >* toDelete = current->next_;
        current->next_ = current->next_->next_;
        delete toDelete;
        size_--;
      }
      else
      {
        current = current->next_;
      }
    }
  }

  template < class T >
  template <class UnaryPredicate>
  void List< T >::removeIf(UnaryPredicate pred)
  {
    Node< T >* current = fake_;
    while (current->next_ != fake_)
    {
      if (pred(current->next_->data_))
      {
        Node< T >* toDelete = current->next_;
        current->next_ = current->next_->next_;
        delete toDelete;
        size_--;
      }
      else
      {
        current = current->next_;
      }
    }
  }

  template < class T >
  void List< T >::splice(Iterc pos, List< T >& rhs, Iterc first, Iterc last)
  {
    if (first == last || rhs.empty())
    {
      return;
    }
    Node< T >* posNode = pos.getNode();
    Node< T >* firstNode = first.getNode();
    Node< T >* lastNode = firstNode;
    Node< T >* prevNode = rhs.fake_;
    while (prevNode->next_ != firstNode)
    {
      prevNode = prevNode->next_;
    }
    while (lastNode->next_ != last.getNode())
    {
      lastNode = lastNode->next_;
    }
    prevNode->next_ = lastNode->next_;
    lastNode->next_ = posNode->next_;
    posNode->next_ = firstNode;
    size_t count = 0;
    Node< T >* temp = firstNode;
    while (temp != last.getNode())
    {
      count++;
      temp = temp->next_;
    }
    size_ += count;
    rhs.size_ -= count;
  }

  template < class T >
  void List< T >::splice(Iterc pos, List< T >& rhs)
  {
    splice(pos, rhs, rhs.cbegin(), rhs.cend());
  }

  template < class T >
  void List< T >::splice(Iterc pos, List< T >&& rhs)
  {
    splice(pos, rhs);
  }

  template < class T >
  void List< T >::splice(Iterc pos, List< T >& rhs, Iterc it)
  {
    Iterc next_ = it;
    next_++;
    splice(pos, rhs, it, next_);
  }

  template < class T >
  void List< T >::splice(Iterc pos, List< T >&& rhs, Iterc it)
  {
    splice(pos, rhs, it);
  }

  template < class T >
  void List< T >::splice(Iterc pos, List< T >&& rhs, Iterc first, Iterc last)
  {
    splice(pos, rhs, first, last);
  }

  template < class T >
  void List< T >::assign(size_t count, const T& value)
  {
    clear();
    while (count--)
    {
      push_back(value);
    }
  }

};


#endif
