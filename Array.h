/**
 * ai-utils -- C++ Core utilities
 *
 * @file
 * @brief Definition of template classes ArrayIndex and Array in namespace utils.
 *
 * @Copyright (C) 2018  Carlo Wood.
 *
 * RSA-1024 0x624ACAD5 1997-01-26                    Sign & Encrypt
 * Fingerprint16 = 32 EC A7 B6 AC DB 65 A6  F6 F6 55 DD 1C DC FF 61
 *
 * This file is part of ai-utils.
 *
 * ai-utils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ai-utils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ai-utils.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <array>
#include <iostream>
#include "debug.h"

namespace utils {

template <typename Category>
class ArrayIndex;

template<typename Category>
std::ostream& operator<<(std::ostream& os, ArrayIndex<Category> const& index);

template <typename Category>
class ArrayIndex
{
 private:
  int m_value;

 public:
  ArrayIndex() : m_value(-1) { }
  explicit constexpr ArrayIndex(int value) : m_value(value) { }
  int get_value() const { return m_value; }

  ArrayIndex& operator++() { ++m_value; return *this; }
  ArrayIndex operator++(int) { ArrayIndex old(m_value); ++m_value; return old; }
  ArrayIndex& operator--() { --m_value; return *this; }
  ArrayIndex operator--(int) { ArrayIndex old(m_value); --m_value; return old; }

  void set_to_zero() { m_value = 0; }
  bool is_zero() const { return m_value == 0; }
  void set_to_undefined() { m_value = -1; }
  bool undefined() const { return m_value == -1; }

  bool operator==(ArrayIndex const& index) const { return m_value == index.m_value; }
  bool operator!=(ArrayIndex const& index) const { return m_value != index.m_value; }
  bool operator<(ArrayIndex const& index) const { return m_value < index.m_value; }
  bool operator>(ArrayIndex const& index) const { return m_value > index.m_value; }
  bool operator<=(ArrayIndex const& index) const { return m_value <= index.m_value; }
  bool operator>=(ArrayIndex const& index) const { return m_value >= index.m_value; }

  ArrayIndex operator-(int n) const { return ArrayIndex{m_value - n}; }
  ArrayIndex operator+(int n) const { return ArrayIndex{m_value + n}; }

  friend std::ostream& operator<<<>(std::ostream& os, ArrayIndex<Category> const& index);
};

template<typename Category>
std::ostream& operator<<(std::ostream& os, ArrayIndex<Category> const& index)
{
  os << '#' << index.m_value;
  return os;
}

template<typename T, std::size_t N, typename _Index = ArrayIndex<T>>
class Array : public std::array<T, N>
{
 public:
  using _Base = std::array<T, N>;
  using reference = typename _Base::reference;
  using const_reference = typename _Base::const_reference;
  using index_type = _Index;

  reference operator[](index_type __n) _GLIBCXX_NOEXCEPT { return _Base::operator[](__n.get_value()); }
  const_reference operator[](index_type __n) const _GLIBCXX_NOEXCEPT { return _Base::operator[](__n.get_value()); }

  reference at(index_type __n) { return _Base::at(__n.get_value()); }
  const_reference at(index_type __n) const { return _Base::at(__n.get_value()); }

  index_type ibegin() const { return index_type(0); }
  index_type iend() const { return index_type((int)N); }
};

} // namespace utils
