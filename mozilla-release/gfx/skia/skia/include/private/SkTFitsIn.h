/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkTFitsIn_DEFINED
#define SkTFitsIn_DEFINED

#include <limits>
#include <stdint.h>
#include <type_traits>

/**
 * std::underlying_type is only defined for enums. For integral types, we just want the type.
 */
template <typename T, class Enable = void>
struct sk_strip_enum {
    typedef T type;
};

template <typename T>
struct sk_strip_enum<T, typename std::enable_if<std::is_enum<T>::value>::type> {
    typedef typename std::underlying_type<T>::type type;
};


/**
 * In C++ an unsigned to signed cast where the source value cannot be represented in the destination
 * type results in an implementation defined destination value. Unlike C, C++ does not allow a trap.
 * This makes "(S)(D)s == s" a possibly useful test. However, there are two cases where this is
 * incorrect:
 *
 * when testing if a value of a smaller signed type can be represented in a larger unsigned type
 * (int8_t)(uint16_t)-1 == -1 => (int8_t)0xFFFF == -1 => [implementation defined] == -1
 *
 * when testing if a value of a larger unsigned type can be represented in a smaller signed type
 * (uint16_t)(int8_t)0xFFFF == 0xFFFF => (uint16_t)-1 == 0xFFFF => 0xFFFF == 0xFFFF => true.
 *
 * Consider the cases:
 *   u = unsigned, less digits
 *   U = unsigned, more digits
 *   s = signed, less digits
 *   S = signed, more digits
 *   v is the value we're considering.
 *
 * u -> U: (u)(U)v == v, trivially true
 * U -> u: (U)(u)v == v, both casts well defined, test works
 * s -> S: (s)(S)v == v, trivially true
 * S -> s: (S)(s)v == v, first cast implementation value, second cast defined, test works
 * s -> U: (s)(U)v == v, *this is bad*, the second cast results in implementation defined value
 * S -> u: (S)(u)v == v, the second cast is required to prevent promotion of rhs to unsigned
 * u -> S: (u)(S)v == v, trivially true
 * U -> s: (U)(s)v == v, *this is bad*,
 *                             first cast results in implementation defined value,
 *                             second cast is defined. However, this creates false positives
 *                             uint16_t x = 0xFFFF
 *                                (uint16_t)(int8_t)x == x
 *                             => (uint16_t)-1        == x
 *                             => 0xFFFF              == x
 *                             => true
 *
 * So for the eight cases three are trivially true, three more are valid casts, and two are special.
 * The two 'full' checks which otherwise require two comparisons are valid cast checks.
<<<<<<< HEAD
 * The two remaining checks s -> U [v >= 0] and U -> s [v <= max(s)] can be done with one op.
 */

template <typename D, typename S>
static constexpr inline
typename std::enable_if<(std::is_integral<S>::value || std::is_enum<S>::value) &&
                        (std::is_integral<D>::value || std::is_enum<D>::value), bool>::type
/*bool*/ SkTFitsIn(S src) {
    // SkTFitsIn() is used in public headers, so needs to be written targeting at most C++11.
    return

    // E.g. (int8_t)(uint8_t) int8_t(-1) == -1, but the uint8_t == 255, not -1.
    (std::is_signed<S>::value && std::is_unsigned<D>::value && sizeof(S) <= sizeof(D)) ?
        (S)0 <= src :

    // E.g. (uint8_t)(int8_t) uint8_t(255) == 255, but the int8_t == -1.
    (std::is_signed<D>::value && std::is_unsigned<S>::value && sizeof(D) <= sizeof(S)) ?
        src <= (S)std::numeric_limits<D>::max() :

    // else
        (S)(D)src == src;
||||||| merged common ancestors
 * The two remaining checks uX -> sx [uX < max(sx)] and sx -> uX [sx > 0] can be done with one op.
 */

namespace sktfitsin {
namespace Private {

/** SkTMux::type = (a && b) ? Both : (a) ? A : (b) ? B : Neither; */
template <bool a, bool b, typename Both, typename A, typename B, typename Neither>
struct SkTMux {
    using type = skstd::conditional_t<a, skstd::conditional_t<b, Both, A>,
                                         skstd::conditional_t<b, B, Neither>>;
};

/** SkTHasMoreDigits = (digits(A) >= digits(B)) ? true_type : false_type. */
template <typename A, typename B> struct SkTHasMoreDigits
    : skstd::bool_constant<std::numeric_limits<A>::digits >= std::numeric_limits<B>::digits>
{ };

/** Returns true.
 *  Used when it is statically known that source values are in the range of the Destination.
 */
template <typename S> struct SkTInRange_True {
    static constexpr bool fits(S) {
        return true;
    }
};

/** Tests that (S)(D)s == s.
 *  This is not valid for uX -> sx and sx -> uX conversions.
 */
template <typename D, typename S> struct SkTInRange_Cast {
    static constexpr bool fits(S s) {
        using S_is_bigger = SkTHasMoreDigits<S, D>;
        using D_is_bigger = SkTHasMoreDigits<D, S>;

        using S_is_signed = skstd::bool_constant<std::numeric_limits<S>::is_signed>;
        using D_is_signed = skstd::bool_constant<std::numeric_limits<D>::is_signed>;

        using precondition = skstd::bool_constant<
            !((!S_is_signed::value &&  D_is_signed::value && S_is_bigger::value) ||
              ( S_is_signed::value && !D_is_signed::value && D_is_bigger::value)   )>;
        static_assert(precondition::value, "not valid for uX -> sx and sx -> uX conversions");

        return static_cast<S>(static_cast<D>(s)) == s;
    }
};

/** Tests if the source value <= Max(D).
 *  Assumes that Max(S) >= Max(D).
 */
template <typename D, typename S> struct SkTInRange_LE_MaxD {
    static constexpr bool fits(S s) {
        using precondition = SkTHasMoreDigits<S, D>;
        static_assert(precondition::value, "maxS < maxD");

        return s <= static_cast<S>((std::numeric_limits<D>::max)());

    }
};

/** Tests if the source value >= 0. */
template <typename D, typename S> struct SkTInRange_GE_Zero {
    static constexpr bool fits(S s) {
        return static_cast<S>(0) <= s;
    }
};

/** SkTFitsIn_Unsigned2Unsiged::type is an SkTInRange with an fits(S s) method
 *  the implementation of which is tailored for the source and destination types.
 *  Assumes that S and D are unsigned integer types.
 */
template <typename D, typename S> struct SkTFitsIn_Unsigned2Unsiged {
    using CastCheck = SkTInRange_Cast<D, S>;
    using NoCheck = SkTInRange_True<S>;

    // If std::numeric_limits<D>::digits >= std::numeric_limits<S>::digits, nothing to check.
    using sourceFitsInDesitination = SkTHasMoreDigits<D, S>;
    using type = skstd::conditional_t<sourceFitsInDesitination::value, NoCheck, CastCheck>;
};

/** SkTFitsIn_Signed2Signed::type is an SkTInRange with an fits(S s) method
 *  the implementation of which is tailored for the source and destination types.
 *  Assumes that S and D are signed integer types.
 */
template <typename D, typename S> struct SkTFitsIn_Signed2Signed {
    using CastCheck = SkTInRange_Cast<D, S>;
    using NoCheck = SkTInRange_True<S>;

    // If std::numeric_limits<D>::digits >= std::numeric_limits<S>::digits, nothing to check.
    using sourceFitsInDesitination = SkTHasMoreDigits<D, S>;
    using type = skstd::conditional_t<sourceFitsInDesitination::value, NoCheck, CastCheck>;
};

/** SkTFitsIn_Signed2Unsigned::type is an SkTInRange with an fits(S s) method
 *  the implementation of which is tailored for the source and destination types.
 *  Assumes that S is a signed integer type and D is an unsigned integer type.
 */
template <typename D, typename S> struct SkTFitsIn_Signed2Unsigned {
    using CastCheck = SkTInRange_Cast<D, S>;
    using LowSideOnlyCheck = SkTInRange_GE_Zero<D, S>;

    // If std::numeric_limits<D>::max() >= std::numeric_limits<S>::max(),
    // no need to check the high side. (Until C++11, assume more digits means greater max.)
    // This also protects the precondition of SkTInRange_Cast.
    using sourceCannotExceedDest = SkTHasMoreDigits<D, S>;
    using type = skstd::conditional_t<sourceCannotExceedDest::value, LowSideOnlyCheck, CastCheck>;
};

/** SkTFitsIn_Unsigned2Signed::type is an SkTInRange with an fits(S s) method
 *  the implementation of which is tailored for the source and destination types.
 *  Assumes that S is an usigned integer type and D is a signed integer type.
 */
template <typename D, typename S> struct SkTFitsIn_Unsigned2Signed {
    using HighSideCheck = SkTInRange_LE_MaxD<D, S>;
    using NoCheck = SkTInRange_True<S>;

    // If std::numeric_limits<D>::max() >= std::numeric_limits<S>::max(), nothing to check.
    // (Until C++11, assume more digits means greater max.)
    using sourceCannotExceedDest = SkTHasMoreDigits<D, S>;
    using type = skstd::conditional_t<sourceCannotExceedDest::value, NoCheck, HighSideCheck>;
};

/** SkTFitsIn::type is an SkTInRange with an fits(S s) method
 *  the implementation of which is tailored for the source and destination types.
 *  Assumes that S and D are integer types.
 */
template <typename D, typename S> struct SkTFitsIn {
    // One of the following will be the 'selector' type.
    using S2S = SkTFitsIn_Signed2Signed<D, S>;
    using S2U = SkTFitsIn_Signed2Unsigned<D, S>;
    using U2S = SkTFitsIn_Unsigned2Signed<D, S>;
    using U2U = SkTFitsIn_Unsigned2Unsiged<D, S>;

    using S_is_signed = skstd::bool_constant<std::numeric_limits<S>::is_signed>;
    using D_is_signed = skstd::bool_constant<std::numeric_limits<D>::is_signed>;

    using selector = typename SkTMux<S_is_signed::value, D_is_signed::value,
                                     S2S, S2U, U2S, U2U>::type;
    // This type is an SkTInRange.
    using type = typename selector::type;
};

template <typename T, bool = std::is_enum<T>::value> struct underlying_type {
    using type = skstd::underlying_type_t<T>;
};
template <typename T> struct underlying_type<T, false> {
    using type = T;
};

} // namespace Private
} // namespace sktfitsin

/** Returns true if the integer source value 's' will fit in the integer destination type 'D'. */
template <typename D, typename S> constexpr inline bool SkTFitsIn(S s) {
    static_assert(std::is_integral<S>::value || std::is_enum<S>::value, "S must be integral.");
    static_assert(std::is_integral<D>::value || std::is_enum<D>::value, "D must be integral.");

    using RealS = typename sktfitsin::Private::underlying_type<S>::type;
    using RealD = typename sktfitsin::Private::underlying_type<D>::type;

    return sktfitsin::Private::SkTFitsIn<RealD, RealS>::type::fits(s);
=======
 * The two remaining checks s -> U [v >= 0] and U -> s [v <= max(s)] can be done with one op.
 */

template <typename D, typename S>
static constexpr inline
typename std::enable_if<(std::is_integral<S>::value || std::is_enum<S>::value) &&
                        (std::is_integral<D>::value || std::is_enum<D>::value), bool>::type
/*bool*/ SkTFitsIn(S src) {
    // SkTFitsIn() is used in public headers, so needs to be written targeting at most C++11.
    return

    // E.g. (int8_t)(uint8_t) int8_t(-1) == -1, but the uint8_t == 255, not -1.
    (std::is_signed<S>::value && std::is_unsigned<D>::value && sizeof(S) <= sizeof(D)) ?
        (S)0 <= src :

    // E.g. (uint8_t)(int8_t) uint8_t(255) == 255, but the int8_t == -1.
    (std::is_signed<D>::value && std::is_unsigned<S>::value && sizeof(D) <= sizeof(S)) ?
        src <= (S)std::numeric_limits<typename sk_strip_enum<D>::type>::max() :

#if !defined(SK_DEBUG) && !defined(__MSVC_RUNTIME_CHECKS )
    // Correct (simple) version. This trips up MSVC's /RTCc run-time checking.
    (S)(D)src == src;
#else
    // More complex version that's safe with /RTCc. Used in all debug builds, for coverage.
    (std::is_signed<S>::value) ?
        (intmax_t)src >= (intmax_t)std::numeric_limits<typename sk_strip_enum<D>::type>::min() &&
        (intmax_t)src <= (intmax_t)std::numeric_limits<typename sk_strip_enum<D>::type>::max() :

    // std::is_unsigned<S> ?
        (uintmax_t)src <= (uintmax_t)std::numeric_limits<typename sk_strip_enum<D>::type>::max();
#endif
>>>>>>> upstream-releases
}

#endif
