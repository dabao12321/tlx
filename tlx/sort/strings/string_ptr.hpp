/*******************************************************************************
 * tlx/sort/strings/string_ptr.hpp
 *
 * StringPtr, StringLcpPtr, StringShadowPtr, and StringShadowLcpPtr:
 * Encapsulation of string, shadow and LCP array pointers.
 *
 * StringPtr          -> (string,size)
 * StringLcpPtr       -> (string,lcp,size)
 * StringShadowPtr    -> (string,shadow,size,flip)
 * StringShadowLcpPtr -> (string,shadow,lcp,size,flip)
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2013-2018 Timo Bingmann <tb@panthema.net>
 * Copyright (C) 2013-2014 Andreas Eberle <email@andreas-eberle.com>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_SORT_STRINGS_STRING_PTR_HEADER
#define TLX_SORT_STRINGS_STRING_PTR_HEADER

#include <tlx/sort/strings/string_set.hpp>

#include <algorithm>
#include <cassert>
#include <stdint.h>

namespace tlx {
namespace sort_strings_detail {

using LcpType = size_t;

template <typename StringSet_>
class StringShadowPtr;

template <typename StringSet_>
class StringShadowLcpPtr;

/******************************************************************************/
// StringPtr

//! Objectified string array pointer array.
template <typename StringSet_>
class StringPtr
{
public:
    typedef StringSet_ StringSet;
    typedef typename StringSet::String String;

protected:
    //! strings (front) array
    StringSet active_;

public:
    //! constructor specifying all attributes
    StringPtr(const StringSet& ss)
        : active_(ss) { }

    //! return currently active array
    const StringSet& active() const { return active_; }

    //! return valid length
    size_t size() const { return active_.size(); }

    //! Advance (both) pointers by given offset, return sub-array
    StringPtr sub(size_t offset, size_t sub_size) const {
        assert(offset + sub_size <= size());
        return StringPtr(active_.subi(offset, offset + sub_size));
    }

    //! if we want to save the LCPs
    static const bool with_lcp = false;

    //! set the i-th lcp to v and check its value
    void set_lcp(size_t /* i */, const LcpType& /* v */) const { }

    //! objectified string and shadow pointer class
    typedef StringShadowPtr<StringSet_> WithShadow;

    //! construct objectified string and shadow pointer class
    StringShadowPtr<StringSet_> add_shadow(const StringSet& shadow) const;
};

/******************************************************************************/
// StringLcpPtr

//! Objectified string and LCP array pointer arrays.
template <typename StringSet_>
class StringLcpPtr
{
public:
    typedef StringSet_ StringSet;
    typedef typename StringSet::String String;

protected:
    //! strings (front) array
    StringSet active_;

    //! lcp array
    LcpType* lcp_;

public:
    //! constructor specifying all attributes
    StringLcpPtr(const StringSet& ss, LcpType* lcp)
        : active_(ss), lcp_(lcp) { }

    //! return currently active array
    const StringSet& active() const { return active_; }

    //! return valid length
    size_t size() const { return active_.size(); }

    //! Advance (both) pointers by given offset, return sub-array
    StringLcpPtr sub(size_t offset, size_t sub_size) const {
        assert(offset + sub_size <= size());
        return StringLcpPtr(active_.subi(offset, offset + sub_size),
                            lcp_ + offset);
    }

    //! if we want to save the LCPs
    static const bool with_lcp = true;

    //! set the i-th lcp to v and check its value
    void set_lcp(size_t i, const LcpType& v) const {
        assert(i < size());
        lcp_[i] = v;
    }

    //! objectified string and shadow pointer class
    typedef StringShadowLcpPtr<StringSet_> WithShadow;

    //! construct objectified string and shadow pointer class
    StringShadowLcpPtr<StringSet_> add_shadow(const StringSet& shadow) const;
};

/******************************************************************************/
// StringShadowPtr

//! Objectified string array pointer and shadow pointer array for out-of-place
//! swapping of pointers.
template <typename StringSet_>
class StringShadowPtr
{
public:
    typedef StringSet_ StringSet;
    typedef typename StringSet::String String;
    typedef typename StringSet::Iterator Iterator;

protected:
    //! strings (front) and temporary shadow (back) array
    StringSet active_, shadow_;

    //! false if active_ is original, true if shadow_ is original
    bool flipped_;

public:
    //! constructor specifying all attributes
    StringShadowPtr(const StringSet& original, const StringSet& shadow,
                    bool flipped = false)
        : active_(original), shadow_(shadow), flipped_(flipped) { }

    //! return currently active array
    const StringSet& active() const { return active_; }

    //! return current shadow array
    const StringSet& shadow() const { return shadow_; }

    //! true if flipped to back array
    bool flipped() const { return flipped_; }

    //! return valid length
    size_t size() const { return active_.size(); }

    //! Advance (both) pointers by given offset, return sub-array without flip
    StringShadowPtr sub(size_t offset, size_t sub_size) const {
        assert(offset + sub_size <= size());
        return StringShadowPtr(active_.subi(offset, offset + sub_size),
                               shadow_.subi(offset, offset + sub_size),
                               flipped_);
    }

    //! construct a StringShadowPtr object specifying a sub-array with flipping
    //! to other array.
    StringShadowPtr flip(size_t offset, size_t sub_size) const {
        assert(offset + sub_size <= size());
        return StringShadowPtr(shadow_.subi(offset, offset + sub_size),
                               active_.subi(offset, offset + sub_size),
                               !flipped_);
    }

    //! return subarray pointer to n strings in original array, might copy from
    //! shadow before returning.
    StringShadowPtr copy_back() {
        if (!flipped_) {
            return *this;
        }
        else {
            std::move(active_.begin(), active_.end(), shadow_.begin());
            return StringShadowPtr(shadow_, active_, !flipped_);
        }
    }

    //! if we want to save the LCPs
    static const bool with_lcp = false;

    //! set the i-th lcp to v and check its value
    void set_lcp(size_t /* i */, const LcpType& /* v */) const { }
};

/******************************************************************************/
// StringShadowLcpPtr

//! Objectified string array pointer and shadow pointer array for out-of-place
//! swapping of pointers.
template <typename StringSet_>
class StringShadowLcpPtr
{
public:
    typedef StringSet_ StringSet;
    typedef typename StringSet::String String;
    typedef typename StringSet::Iterator Iterator;

protected:
    //! strings (front) and temporary shadow (back) array
    StringSet active_, shadow_;

    //! lcp array
    LcpType* lcp_;

    //! false if active_ is original, true if shadow_ is original
    bool flipped_;

public:
    //! constructor specifying all attributes
    StringShadowLcpPtr(const StringSet& original, const StringSet& shadow,
                       LcpType* lcp, bool flipped = false)
        : active_(original), shadow_(shadow), lcp_(lcp), flipped_(flipped) { }

    //! return currently active array
    const StringSet& active() const { return active_; }

    //! return current shadow array
    const StringSet& shadow() const { return shadow_; }

    //! true if flipped to back array
    bool flipped() const { return flipped_; }

    //! return valid length
    size_t size() const { return active_.size(); }

    //! Advance (both) pointers by given offset, return sub-array without flip
    StringShadowLcpPtr sub(size_t offset, size_t sub_size) const {
        assert(offset + sub_size <= size());
        return StringShadowLcpPtr(active_.subi(offset, offset + sub_size),
                                  shadow_.subi(offset, offset + sub_size),
                                  lcp_ + offset, flipped_);
    }

    //! construct a StringShadowLcpPtr object specifying a sub-array with
    //! flipping to other array.
    StringShadowLcpPtr flip(size_t offset, size_t sub_size) const {
        assert(offset + sub_size <= size());
        return StringShadowLcpPtr(shadow_.subi(offset, offset + sub_size),
                                  active_.subi(offset, offset + sub_size),
                                  lcp_ + offset, !flipped_);
    }

    //! return subarray pointer to n strings in original array, might copy from
    //! shadow before returning.
    StringShadowLcpPtr copy_back() {
        if (!flipped_) {
            return *this;
        }
        else {
            std::move(active_.begin(), active_.end(), shadow_.begin());
            return StringShadowLcpPtr(shadow_, active_, lcp_, !flipped_);
        }
    }

    //! if we want to save the LCPs
    static const bool with_lcp = true;

    //! set the i-th lcp to v and check its value
    void set_lcp(size_t i, const LcpType& v) const {
        assert(i < size());
        lcp_[i] = v;
    }
};

/******************************************************************************/

template <typename StringSet_>
StringShadowPtr<StringSet_>
StringPtr<StringSet_>::add_shadow(const StringSet_& shadow) const {
    return StringShadowPtr<StringSet_>(active_, shadow);
}

template <typename StringSet_>
StringShadowLcpPtr<StringSet_>
StringLcpPtr<StringSet_>::add_shadow(const StringSet_& shadow) const {
    return StringShadowLcpPtr<StringSet_>(active_, shadow, lcp_);
}

/******************************************************************************/

} // namespace sort_strings_detail
} // namespace tlx

#endif // !TLX_SORT_STRINGS_STRING_PTR_HEADER

/******************************************************************************/
