/**
 * ai-utils -- C++ Core utilities
 *
 * @file
 * @brief Declaration of struct BitSetPOD, struct IndexPOD, class BitSet and class Index.
 *
 * @Copyright (C) 2019  Carlo Wood.
 *
 * RSA-1024 0x624ACAD5 1997-01-26                    Sign & Encrypt
 * Fingerprint16 = 32 EC A7 B6 AC DB 65 A6  F6 F6 55 DD 1C DC FF 61
 *
 * This file is part of ai-utils.
 *
 * Ai-utils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ai-utils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with ai-utils.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "ctz.h"
#include "log2.h"
#include "mssb.h"
#include "popcount.h"
#include "is_power_of_two.h"
#include <cstdint>
#include <type_traits>
#include <iosfwd>

namespace utils {

// Forward declaration.
template<typename T>
class BitSet;

namespace bitset {

// POD struct for Index.
//
// Let N = 8, 16, 32, 64 or 128 be the number of bits
// in the underlaying integral type T of the BitSet<T>
// that this index refers to (N = 8 * sizeof(T)).
//
// The meaning of m_index depends on its value.
//
// If the value is positive and in the range [0, N>, then
// it represents the bit index in the (unsigned) integral
// type T, where 0 refers to the least significant bit.
//
// If the value is negative it must be -1 (all bits set).
// This value is reserved for use in a bit iterator and means
// "one before begin()", where begin() refers to the least
// significant bit (index 0).
//
// A value of precisely N is reserved for use in a bit
// iterator and means "one past the end", or end().
//
// The use of IndexPOD is intended for constants.
// For example,
//
// constexpr IndexPOD ilsb = { 0 };
//
// could be defined to have ilsb refer to bit 0.
//
struct IndexPOD
{
  int8_t m_index;
};

// Define a few handy constants.
//
constexpr IndexPOD index_pre_begin = { -1 };
constexpr IndexPOD index_begin = { 0 };
template<typename T> constexpr IndexPOD index_end = { 8 * sizeof(T) };

// Compare constants (this should never be needed, but why not add it).
constexpr bool operator==(IndexPOD i1, IndexPOD i2) { return i1.m_index == i2.m_index; }
constexpr bool operator!=(IndexPOD i1, IndexPOD i2) { return i1.m_index != i2.m_index; }

class Index : protected IndexPOD
{
 public:
  // Constructors.

  // Construct an uninitialized Index.
  Index() { }
  // Copy-constructor.
  Index(Index const& i1) { m_index = i1.m_index; }
  // Construct a Index from a constant.
  Index(IndexPOD i1) { m_index = i1.m_index; }

  // Assignment operators.

  // Assign from another Index.
  Index& operator=(Index i1) { m_index = i1.m_index; return *this; }
  // Assign from a constant.
  Index& operator=(IndexPOD i1) { m_index = i1.m_index; return *this; }

  // Comparision operators.

  friend bool operator==(Index const& i1, Index const& i2) { return i1.m_index == i2.m_index; }
  friend bool operator==(Index const& i1, IndexPOD i2) { return i1.m_index == i2.m_index; }
  friend bool operator==(IndexPOD i1, Index const& i2) { return i1.m_index == i2.m_index; }
  friend bool operator!=(Index const& i1, Index const& i2) { return i1.m_index != i2.m_index; }
  friend bool operator!=(Index const& i1, IndexPOD i2) { return i1.m_index != i2.m_index; }
  friend bool operator!=(IndexPOD i1, Index const& i2) { return i1.m_index != i2.m_index; }

  friend bool operator<(Index const& i1, Index const& i2) { return i1.m_index < i2.m_index; }
  friend bool operator<(Index const& i1, IndexPOD const& i2) { return i1.m_index < i2.m_index; }
  friend bool operator<(IndexPOD const& i1, Index const& i2) { return i1.m_index < i2.m_index; }
  friend bool operator<=(Index const& i1, Index const& i2) { return i1.m_index <= i2.m_index; }
  friend bool operator<=(Index const& i1, IndexPOD const& i2) { return i1.m_index <= i2.m_index; }
  friend bool operator<=(IndexPOD const& i1, Index const& i2) { return i1.m_index <= i2.m_index; }
  friend bool operator>(Index const& i1, Index const& i2) { return i1.m_index > i2.m_index; }
  friend bool operator>(Index const& i1, IndexPOD const& i2) { return i1.m_index > i2.m_index; }
  friend bool operator>(IndexPOD const& i1, Index const& i2) { return i1.m_index > i2.m_index; }
  friend bool operator>=(Index const& i1, Index const& i2) { return i1.m_index >= i2.m_index; }
  friend bool operator>=(Index const& i1, IndexPOD const& i2) { return i1.m_index >= i2.m_index; }
  friend bool operator>=(IndexPOD const& i1, Index const& i2) { return i1.m_index >= i2.m_index; }

  // Manipulators.

  Index& operator+=(int offset) { m_index += offset; return *this; }
  friend Index operator+(Index index, int offset) { Index result(index); return result += offset; }
  friend Index operator+(int offset, Index index) { Index result(index); return result += offset; }
  Index& operator-=(int offset) { m_index -= offset; return *this; }
  friend Index operator-(Index index, int offset) { Index result(index); return result -= offset; }
  friend Index operator-(int offset, Index index) { Index result(index); return result -= offset; }

  Index& operator++() { ++m_index; return *this; }
  Index operator++(int) { Index result(*this); operator++(); return result; }
  Index& operator--() { --m_index; return *this; }
  Index operator--(int) { Index result(*this); operator--(); return result; }

  // Accessor.

  // Return the unlaying integral value.
  int8_t operator()() const { return m_index; }

  // Special functions.

  // Advance Index to the next bit that is set in mask.
  //
  // Index may be index_pre_begin, in which case it will be set
  // to the first bit that is set in the mask ([0, N>) if any,
  // or index_end if no bit is set.
  //
  // Otherwise Index must be in the range [0, N>, in which
  // case a value is returned larger than the current value.
  // If no more bits could be found, Index is set to index_end.
  //
  // Don't call this function when Index equals index_end.
  template<typename T>
  void next_bit_in(T mask)
  {
    constexpr int8_t N = index_end<T>.m_index;          // The number of bits in T, as well as 'end'.
    //                                                   ____________
    //                                                  v            \.
    // The general case, assume N == 8 and mask is 01000110 and index 2.
    //                                              ^^^^_______________________
    // In that case we require 6 to be returned.                               \.
    if (++m_index != N)                                 // m_index becomes 3.   |
    {                                                   //                     /
      mask >>= m_index;                                 // mask becomes 00001000.
      if (mask == 0)
        m_index = N;
      else
        m_index += ctz(mask);                           // m_index becomes 6.
    }
  }

  // Specialization for BitSet.
  template<typename T>
  [[gnu::always_inline]] inline void next_bit_in(BitSet<T> const& m1);

  // Decrease Index to the previous bit that is set in mask.
  //
  // Index may be index_end, in which case it will be set to the
  // last bit that is set in the mask ([0, N>) if any, or index_pre_begin
  // if no bit is set.
  //
  // Otherwise Index must be in the range <0, N>, in which
  // case a value is returned smaller than the current value.
  // If no more bits could be found, Index is set to index_pre_begin.
  //
  // Normally a value of 0 should always return index_pre_begin, but
  // that is not the case:
  //
  // Don't call this function when Index equals index_begin.
  template<typename T>
  void prev_bit_in(T mask)
  {
    constexpr int8_t N = index_end<T>.m_index;          // The number of bits in T.
    //                                                _______________
    //                                               v               \.
    // The general case, assume N == 8 and mask is 01100010 and index 5.
    //                                                ^^^^________________
    // In that case we require 1 to be returned.                          \.
    mask <<= N - m_index;                               // mask becomes 00010000.
    if (mask == 0)
      m_index = index_pre_begin.m_index;
    else
      m_index -= clz(mask) + 1;                         // m_index becomes 1.
  }

  // Specialization for BitSet.
  template<typename T>
  [[gnu::always_inline]] inline void prev_bit_in(BitSet<T> const& m1);

  // Return true iff Index is not index_pre_begin and also not index_begin.
  bool may_call_prev_bit_in() const { return m_index > 0; }
};

} //namespace bitset

// The POD base type of class BitSet.
//
// This class contains a unsigned integer of type T.
// It may not have constructors or destructors, because
// it is needed that this struct is POD in order for
// optimisation to work.
//
// The use of BitSetPOD is intended for constants.
// For example,
//
// constexpr BitSetPOD<uint32_t> bszero = { 1 };
//
// could be defined to have bszero refer to a mask just the least significant bit set.
//
template<typename T>
struct BitSetPOD
{
  T m_bitmask;
};

// Compare constants (this should never be needed, but why not add it).
template<typename T> constexpr bool operator==(BitSetPOD<T> m1, BitSetPOD<T> m2) { return m1.m_bitmask == m2.m_bitmask; }
template<typename T> constexpr bool operator!=(BitSetPOD<T> m1, BitSetPOD<T> m2) { return m1.m_bitmask != m2.m_bitmask; }

// Calculate the union of two bitsets constants.
template<typename T>
constexpr BitSetPOD<T> operator|(BitSetPOD<T> m1, BitSetPOD<T> m2)
{
  return {m1.m_bitmask | m2.m_bitmask};
}

// Calculate the intersection of two bitset constants.
template<typename T>
constexpr BitSetPOD<T> operator&(BitSetPOD<T> m1, BitSetPOD<T> m2)
{
  return {m1.m_bitmask & m2.m_bitmask};
}

// Calculate the union minus the intersection of two bitset constants.
template<typename T>
constexpr BitSetPOD<T> operator^(BitSetPOD<T> m1, BitSetPOD<T> m2)
{
  return {m1.m_bitmask ^ m2.m_bitmask};
}

// Calculate the inverse of a bitset constant.
template<typename T>
constexpr BitSetPOD<T> operator~(BitSetPOD<T> m1)
{
  return {~m1.m_bitmask};
}

namespace bitset {

template<typename T>
class const_iterator
{
  T m_mask;     // A copy of the bitset this iterator refers to, but with the lesser significant bits removed.
                // The actual position of the iterator is the least significant set bit.
                // A value of all zeros means 'end'.

 public:
  // Default constructor returns 'end'.
  const_iterator() : m_mask(0) { }
  // Construct a const_iterator from bitmask.
  const_iterator(T bitmask) : m_mask(bitmask) { }
  // Copy-constructor.
  const_iterator(const_iterator const& iter) : m_mask(iter.m_mask) { }
  // Assignment.
  const_iterator& operator=(const_iterator const& iter) { m_mask = iter.m_mask; return *this; }

  // Comparision operators.

  bool operator==(const_iterator const& iter) const { return m_mask == iter.m_mask; }
  bool operator!=(const_iterator const& iter) const { return m_mask != iter.m_mask; }

  // Forward iterator.

  const_iterator& operator++()
  {
    // Remove the least significant bit.
    m_mask &= m_mask - 1;
    return *this;
  }

  inline BitSet<T> operator*() const;
};

} // namespace bitset

template<typename T>
class BitSet : protected BitSetPOD<T>
{
 public:
  static_assert(std::is_unsigned<T>::value, "utils::BitSet<> template parameter must be an unsigned integral type.");
  using mask_type = T;
  using Index = bitset::Index;
  using BitSetPOD<T>::m_bitmask;

  // Convert Index to a mask_type.
  [[gnu::always_inline]] static T index2mask(Index i1) { return static_cast<T>(1) << i1(); }
  // Convert a mask_type to the index of its least significant set bit.
  [[gnu::always_inline]] static Index mask2index(T mask) { return ctz(mask); }

  // Constructors.

  // Construct an uninitialized BitSet.
  BitSet() { }

  // Copy-constructor.
  BitSet(BitSet const& m1) : BitSetPOD<T>{m1.m_bitmask} { }

  // Construct a BitSet with a single bit set at i1.
  BitSet(Index const& i1) : BitSetPOD<T>{index2mask(i1)} { }

  // Construct a BitSet from a constant.
  BitSet(BitSetPOD<T> m1) : BitSetPOD<T>{m1.m_bitmask} { }

  // Construct a BitSet from a constant or mask (for internal use only).
  explicit BitSet(mask_type bitmask) : BitSetPOD<T>{bitmask} { }

  // Assignment operators.

  // Assignment from other BitSet.
  BitSet& operator=(BitSet const& m1) { m_bitmask = m1.m_bitmask; return *this; }

  // Assignment from a constant.
  BitSet& operator=(BitSetPOD<T> m1) { m_bitmask = m1.m_bitmask; return *this; }

  // Comparison operators.

  friend bool operator==(BitSet const& m1, BitSet const& m2) { return m1.m_bitmask == m2.m_bitmask; }
  friend bool operator==(BitSet const& m1, BitSetPOD<T> m2) { return m1.m_bitmask == m2.m_bitmask; }
  friend bool operator==(BitSetPOD<T> m1, BitSet const& m2) { return m1.m_bitmask == m2.m_bitmask; }
  friend bool operator!=(BitSet const& m1, BitSet const& m2) { return m1.m_bitmask != m2.m_bitmask; }
  friend bool operator!=(BitSet const& m1, BitSetPOD<T> m2) { return m1.m_bitmask != m2.m_bitmask; }
  friend bool operator!=(BitSetPOD<T> m1, BitSet const& m2) { return m1.m_bitmask != m2.m_bitmask; }

  // Initialization.

  // Set all bits to zero.
  void reset() { m_bitmask = 0; }

  // Set all bits to one.
  void set() { m_bitmask = static_cast<T>(-1); }

  // Bit manipulation.

  // Reset the bit at index i1.
  void reset(Index const& i1) { m_bitmask &= ~index2mask(i1); }

  // Reset the bits from bitmask.
  void reset(mask_type bitmask) { m_bitmask &= ~bitmask; }

  // Reset the bits from m1.
  void reset(BitSetPOD<T> m1) { m_bitmask &= ~m1.m_bitmask; }

  // Reset the bits from m1.
  void reset(BitSet const& m1) { m_bitmask &= ~m1.m_bitmask; }

  // Set the bit at i1.
  void set(Index const& i1) { m_bitmask |= index2mask(i1); }

  // Set the bits from bitmask.
  void set(mask_type bitmask) { m_bitmask |= bitmask; }

  // Set the bits from m1.
  void set(BitSetPOD<T> m1) { m_bitmask |= m1.m_bitmask; }

  // Set the bits from m1.
  void set(BitSet const& m1) { m_bitmask |= m1.m_bitmask; }

  // Toggle the bit at i1.
  void flip(Index const& i1) { m_bitmask ^= index2mask(i1); }

  // Toggle the bits from bitmask.
  void flip(mask_type bitmask) { m_bitmask ^= bitmask; }

  // Toggle the bits from m1.
  void flip(BitSetPOD<T> m1) { m_bitmask ^= m1.m_bitmask; }

  // Toggle the bits from m1.
  void flip(BitSet const& m1) { m_bitmask ^= m1.m_bitmask; }

  // Left shift by n positions.
  BitSet& operator<<=(unsigned int n) { m_bitmask <<= n; return *this; }
  friend BitSet operator<<(BitSet m1, unsigned int n) { m1 <<= n; return m1; }

  // Right shift by n positions.
  BitSet& operator>>=(unsigned int n) { m_bitmask >>= n; return *this; }
  friend BitSet operator>>(BitSet m1, unsigned int n) { m1 >>= n; return m1; }

  // Accessors.

  // Test if all, any or none of the bits are set.
  bool all() const { return !~m_bitmask; }
  bool any() const { return m_bitmask; }
  bool none() const { return !m_bitmask; }
  bool is_single_bit() const { return is_power_of_two(m_bitmask); }

  // Returns the number of bits that the bitset can hold.
  constexpr std::size_t size() const { return sizeof(T) * 8; }

  // Returns the number of bits set to 1.
  std::size_t count() const { return ::utils::_popcount(m_bitmask); }

  // Return a mask with just the least significant set bit.
  BitSet lssb() const { return {m_bitmask & -m_bitmask}; }

  // Return a mask with just the most significant set bit.
  BitSet mssb() const { return {::utils::mssb(m_bitmask)}; }

  // Return the index to the least significant set bit.
  // Returns index_end<T> if the BitSet is zero.
  Index lssbi() const { return {bitset::IndexPOD{static_cast<int8_t>(m_bitmask ? ctz(m_bitmask) : static_cast<int>(size()))}}; }

  // Return the index to the most significant set bit.
  // Returns index_pre_begin if the BitSet is zero.
  Index mssbi() const { return {bitset::IndexPOD{static_cast<int8_t>(log2(m_bitmask))}}; }

  // Test if any bit is set at all.
  bool test() const { return m_bitmask; }

  // Test if the bit at i1 is set.
  bool test(Index const& i1) const { return m_bitmask & index2mask(i1); }

  // Test if any bit in bitmask is set.
  bool test(mask_type bitmask) const { return m_bitmask & bitmask; }

  // Test if any bit in m1 is set.
  bool test(BitSetPOD<T> m1) const { return m_bitmask & m1.m_bitmask; }

  // Test if any bit in m1 is set.
  bool test(BitSet const& m1) const { return m_bitmask & m1.m_bitmask; }

  // Return the inverse of the BitSet.
  BitSet operator~() const { return BitSet(~m_bitmask); }

  // Return the underlaying bitmask.
  mask_type operator()() const { return m_bitmask; }

  // Return the underlaying bitmask as unsigned long.
  unsigned long to_ulong() const { return static_cast<unsigned long>(m_bitmask); }

  // Return the underlaying bitmask as unsigned long long.
  unsigned long long to_ullong() const { return static_cast<unsigned long long>(m_bitmask); }

  // Converts the contents of the bitset to a string. Uses zero to represent bits with value of false and one to represent bits with value of true.
  std::string to_string(char zero = '0', char one = '1') const;

  // Bit-wise OR operators with another BitSet.
  BitSet& operator|=(BitSet const& m1) { m_bitmask |= m1.m_bitmask; return *this; }
  BitSet& operator|=(BitSetPOD<T> m1) { m_bitmask |= m1.m_bitmask; return *this; }
  BitSet& operator|=(mask_type bitmask) { m_bitmask |= bitmask; return *this; }
  friend BitSet operator|(BitSet const& m1, BitSet const& m2) { return BitSet(m1.m_bitmask | m2.m_bitmask); }
  friend BitSet operator|(BitSet const& m1, BitSetPOD<T> m2) { return BitSet(m1.m_bitmask | m2.m_bitmask); }
  friend BitSet operator|(BitSetPOD<T> m1, BitSet const& m2) { return BitSet(m1.m_bitmask | m2.m_bitmask); }

  // Bit-wise AND operators with another BitSet.
  BitSet& operator&=(BitSet const& m1) { m_bitmask &= m1.m_bitmask; return *this; }
  BitSet& operator&=(BitSetPOD<T> m1) { m_bitmask &= m1.m_bitmask; return *this; }
  BitSet& operator&=(mask_type bitmask) { m_bitmask &= bitmask; return *this; }
  friend BitSet operator&(BitSet m1, BitSet m2) { return BitSet(m1.m_bitmask & m2.m_bitmask); }
  friend BitSet operator&(BitSet m1, BitSetPOD<T> m2) { return BitSet(m1.m_bitmask & m2.m_bitmask); }
  friend BitSet operator&(BitSetPOD<T> m1, BitSet m2) { return BitSet(m1.m_bitmask & m2.m_bitmask); }

  // Bit-wise XOR operators with another BitSet.
  BitSet& operator^=(BitSet const& m1) { m_bitmask ^= m1.m_bitmask; return *this; }
  BitSet& operator^=(BitSetPOD<T> m1) { m_bitmask ^= m1.m_bitmask; return *this; }
  BitSet& operator^=(mask_type bitmask) { m_bitmask ^= bitmask; return *this; }
  friend BitSet operator^(BitSet m1, BitSet m2) { return BitSet(m1.m_bitmask ^ m2.m_bitmask); }
  friend BitSet operator^(BitSet m1, BitSetPOD<T> m2) { return BitSet(m1.m_bitmask ^ m2.m_bitmask); }
  friend BitSet operator^(BitSetPOD<T> m1, BitSet m2) { return BitSet(m1.m_bitmask ^ m2.m_bitmask); }

  // Comparing BitSets as unsigned integrals.

  friend bool operator<(BitSet const& m1, BitSet const& m2) { return m1.m_bitmask < m2.m_bitmask; }
  friend bool operator<=(BitSet const& m1, BitSet const& m2) { return m1.m_bitmask <= m2.m_bitmask; }
  friend bool operator>(BitSet const& m1, BitSet const& m2) { return m1.m_bitmask > m2.m_bitmask; }
  friend bool operator>=(BitSet const& m1, BitSet const& m2) { return m1.m_bitmask >= m2.m_bitmask; }

  // Arithemetic.

  // Support adding and subtracting 1 for special algorithms.

  // Pre decrement.
  BitSet& operator--() { --m_bitmask; return *this; }
  // Post decrement.
  BitSet operator--(int) const { BitSet prev_value(m_bitmask); --m_bitmask; return prev_value; }
  // Pre increment.
  BitSet& operator++() { ++m_bitmask; return *this; }
  // Post increment.
  BitSet operator++(int) const { BitSet prev_value(m_bitmask); ++m_bitmask; return prev_value; }

  // Add and subtract.
  BitSet& operator+=(mask_type n) { m_bitmask += n; return *this; }
  BitSet& operator-=(mask_type n) { m_bitmask -= n; return *this; }
  BitSet operator+(mask_type n) const { return BitSet(m_bitmask + n); }
  BitSet operator-(mask_type n) const { return BitSet(m_bitmask - n); }

  // Writing to an ostream.

  template<typename T1>
  friend std::ostream& operator<<(std::ostream& os, BitSet<T1> m1)
  {
    return os << m1.to_string();
  }

  // Iterator support.

  bitset::const_iterator<T> begin() const { return {m_bitmask}; }
  bitset::const_iterator<T> end() const { return {}; }
};

namespace bitset {

// Inline functions.

template<typename T>
void Index::next_bit_in(BitSet<T> const& m1)
{
  next_bit_in(m1());
}

template<typename T>
void Index::prev_bit_in(BitSet<T> const& m1)
{
  prev_bit_in(m1());
}

template<typename T>
BitSet<T> const_iterator<T>::operator*() const
{
  // Return just the least significant bit.
  return BitSet<T>{m_mask & -m_mask};
}

} // namespace bitset

template<typename T>
std::string BitSet<T>::to_string(char zero, char one) const
{
  std::string result(size(), zero);
  std::string::iterator p = result.begin();
  bitset::Index bit = bitset::index_end<T>;
  do
    *p++ = (test(--bit) ? one : zero);
  while (bit != bitset::index_begin);
  return result;
}

} // namespace utils
