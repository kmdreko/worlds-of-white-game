// FILE: narrayiterator.hpp
// DATE: 7/30/2014
// AUTH: Trevor Wilson [kmdreko@gmail.com]
// DESC: Defines an N-dimensional templated matrix iterator class

// #include guards
#ifndef WILT_NARRAYITERATOR_HPP
#define WILT_NARRAYITERATOR_HPP

// libraries
#include <iterator>
// - std::random_access_iterator_tag

#include "util.h"
#include "point.hpp"
#include "narray.hpp"

// @brief wilt scope
WILT_BEGIN
WILT_COMMON_BEGIN

  //! @class  NArrayIterator
  //!
  //! The iterator class retains its own reference to the array data, meaning
  //! the NArray it is constructed from may be deleted and the iterator will
  //! still iterate over the data
  template <class T, dim_t N>
  class NArrayIterator
  {
  public:
    typedef std::random_access_iterator_tag iterator_category;
    typedef typename std::remove_const<T>::type type;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T& reference;
    typedef T* pointer;

    //! @brief  Default constructor. Useless
    NArrayIterator()
      : m_header(),
        m_base(nullptr),
        m_dims(),
        m_step(),
        m_pos(0)
    {
  
    }

    //! @brief      NArray constructor from an array and offset
    //! @param[in]  arr - array whose data to reference
    //! @param[in]  pos - data offset value, defaults to 0
    NArrayIterator(const wilt::NArray<T, N>& arr, pos_t pos = 0)
      : m_header(arr.m_header),
        m_base(arr.m_base),
        m_dims(arr.m_dims),
        m_step(arr.m_step),
        m_pos(pos)
    {

    }

    //! @brief      Copy constructor
    //! @param[in]  iter - iterator to copy from
    NArrayIterator(const NArrayIterator<T, N>& iter)
      : m_header(iter.m_header),
        m_base(iter.m_base),
        m_dims(iter.m_dims),
        m_step(iter.m_step),
        m_pos(iter.m_pos)
    {

    }

    //! @brief      Copy constructor with offset
    //! @param[in]  iter - iterator to copy from
    //! @param[in]  pos - data offset value
    NArrayIterator(const NArrayIterator<T, N>& iter, pos_t pos)
      : m_header(iter.m_header),
        m_base(iter.m_base),
        m_dims(iter.m_dims),
        m_step(iter.m_step),
        m_pos(pos)
    {

    }

    //! @brief      assignment operator
    //! @param[in]  iter - iterator to copy from
    //! @return     reference to this object
    NArrayIterator<T, N>& operator= (const NArrayIterator<T, N>& iter)
    {
      m_header = iter.m_header;
      m_base = iter.m_base;
      m_dims = iter.m_dims;
      m_step = iter.m_step;
      m_pos = iter.m_pos;

      return *this;
    }

    //! @brief      in-place addition operator, offsets data by +pos
    //! @param[in]  pos - value to offset
    //! @return     reference to this object
    NArrayIterator<T, N>& operator+= (const ptrdiff_t pos)
    {
      m_pos += pos;
      return *this;
    }

    //! @brief      in-place addition operator, offsets data by -pos
    //! @param[in]  pos - value to offset
    //! @return     reference to this object
    NArrayIterator<T, N>& operator-= (const ptrdiff_t pos)
    {
      m_pos -= pos;
      return *this;
    }

    //! @brief      de-reference operator, invalid if <begin or >=end
    //! @return     reference to data at the iterator position
    reference operator* () const
    {
      return *_at(m_pos);
    }

    //! @brief      pointer operator, invalid if <begin or >=end
    //! @return     pointer to data at the iterator position
    pointer operator-> () const
    {
      return _at(m_pos);
    }

    //! @brief      index operator
    //! @param[in]  pos - data offset value
    //! @return     reference to data at the iterator position + offset
    reference operator[] (pos_t pos) const
    {
      return *_at(m_pos + pos);
    }

    //! @brief      equal operator, determines both if it references the same
    //!             NArray data and if at same position
    //! @param[in]  iter - iterator to compare
    //! @return     true if iterators are the same, false otherwise
    bool operator== (const NArrayIterator<T, N>& iter) const
    {
      return m_base == iter.m_base && 
             m_dims == iter.m_dims && 
             m_step == iter.m_step && 
             m_pos  == iter.m_pos;
    }

    //! @brief      not equal operator, determines both if it references the
    //!             same NArray data and if at same position
    //! @param[in]  iter - iterator to compare
    //! @return     false if iterators are the same, true otherwise
    bool operator!= (const NArrayIterator<T, N>& iter) const
    {
      return !(*this == iter);
    }

    //! @brief      less than operator, is only calculated from data offset, 
    //!             does not determine if they share a data-space
    //! @param[in]  iter - iterator to compare
    //! @return     true if data offset is less than that of iter
    bool operator<  (const NArrayIterator<T, N>& iter) const
    {
      return m_pos < iter.m_pos;
    }

    //! @brief      greater than operator, is only calculated from data offset,
    //!             does not determine if they share a data-space
    //! @param[in]  iter - iterator to compare
    //! @return     true if data offset is greater than that of iter
    bool operator>  (const NArrayIterator<T, N>& iter) const
    {
      return m_pos > iter.m_pos;
    }

    //! @brief      less than or equal operator, is only calculated from data 
    //!             offset, does not determine if they share a data-space
    //! @param[in]  iter - iterator to compare
    //! @return     true if data offset is less than or equal to that of iter
    bool operator<= (const NArrayIterator<T, N>& iter) const
    {
      return m_pos <= iter.m_pos;
    }

    //! @brief      greater than or equal operator, is only calculated from data
    //!             offset, does not determine if they share a data-space
    //! @param[in]  iter - iterator to compare
    //! @return     true if data offset is greater than or equal to that of iter
    bool operator>= (const NArrayIterator<T, N>& iter) const
    {
      return !(*this < iter);
    }

    //! @brief      increments the data offset value
    //! @return     reference to this object
    NArrayIterator<T, N>& operator++ ()
    {
      ++m_pos;
      return *this;
    }

    //! @brief      decrements the data offset value
    //! @return     reference to this object
    NArrayIterator<T, N>& operator-- ()
    {
      --m_pos;
      return *this;
    }

    //! @brief      increments the data offset value
    //! @return     copy of this object before incrementing
    NArrayIterator<T, N> operator++ (int)
    {
      return NArrayIterator<T, N>(*this, m_pos++);
    }

    //! @brief      decrements the data offset value
    //! @return     copy of this object before decrementing
    NArrayIterator<T, N> operator-- (int)
    {
      return NArrayIterator<T, N>(*this, m_pos--);
    }

    //! @brief      the addition of two iterators
    //! @param[in]  iter - the iterator to add
    //! @return     the addition of the two data offset values
    //!
    //! This value doesn't mean much, is only here to pair with operator-
    difference_type operator+ (const NArrayIterator<T, N>& iter) const
    {
      return m_pos + iter.m_pos;
    }

    //! @brief      the difference of two iterators
    //! @param[in]  iter - the iterator to diff
    //! @return     the difference of the two data offset values
    difference_type operator- (const NArrayIterator<T, N>& iter) const
    {
      return m_pos - iter.m_pos;
    }

    //! @brief      addition operator
    //! @param[in]  pos - the offset to add
    //! @return     this iterator plus the offset
    NArrayIterator<T, N> operator+ (const ptrdiff_t pos)
    {
      return NArrayIterator<T, N>(*this, m_pos + pos);
    }

    //! @brief      subtraction operator
    //! @param[in]  pos - the offset to subtract
    //! @return     this iterator minus the offset
    NArrayIterator<T, N> operator- (const ptrdiff_t pos)
    {
      return NArrayIterator<T, N>(*this, m_pos - pos);
    }

    //! @brief      gets the position of the iterator
    //! @return     point corresponding to the position in the NArray currently
    //!             pointing to
    Point<N> position() const
    {
      return _idx2pos(m_dims, m_pos);
    }

  private:
    NArrayHeader<type> m_header;
    type* m_base;
    Point<N> m_dims;
    Point<N> m_step;
    pos_t m_pos;

    //! @brief      gets the pointer at the data offset
    //! @param[in]  pos - data offset value
    //! @return     pointer to data at the offset
    pointer _at(pos_t pos) const
    {
      Point<N> loc = _idx2pos(m_dims, m_pos);
      pointer ptr = m_base;
      for (dim_t i = 0; i < N; ++i)
        ptr += loc[i] * m_step[i];

      return ptr;
    }

  }; // class NArrayIterator

WILT_COMMON_END
WILT_END

// WILT_NARRAYITERATOR_HPP
#endif 