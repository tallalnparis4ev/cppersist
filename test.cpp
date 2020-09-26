// #include <optional>
// using std::experimental::optional;
// using std::optional;
// -*- C++ -*-
//===-------------------------- optional ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_OPTIONAL
#define _LIBCPP_OPTIONAL

/*
    optional synopsis

// C++1z

namespace std {
  // 23.6.3, optional for object types
  template <class T> class optional;

  // 23.6.4, no-value state indicator
  struct nullopt_t{see below };
  inline constexpr nullopt_t nullopt(unspecified );

  // 23.6.5, class bad_optional_access
  class bad_optional_access;

  // 23.6.6, relational operators
  template <class T, class U>
  constexpr bool operator==(const optional<T>&, const optional<U>&);
  template <class T, class U>
  constexpr bool operator!=(const optional<T>&, const optional<U>&);
  template <class T, class U>
  constexpr bool operator<(const optional<T>&, const optional<U>&);
  template <class T, class U>
  constexpr bool operator>(const optional<T>&, const optional<U>&);
  template <class T, class U>
  constexpr bool operator<=(const optional<T>&, const optional<U>&);
  template <class T, class U>
  constexpr bool operator>=(const optional<T>&, const optional<U>&);

  // 23.6.7 comparison with nullopt
  template <class T> constexpr bool operator==(const optional<T>&, nullopt_t) noexcept;
  template <class T> constexpr bool operator==(nullopt_t, const optional<T>&) noexcept;
  template <class T> constexpr bool operator!=(const optional<T>&, nullopt_t) noexcept;
  template <class T> constexpr bool operator!=(nullopt_t, const optional<T>&) noexcept;
  template <class T> constexpr bool operator<(const optional<T>&, nullopt_t) noexcept;
  template <class T> constexpr bool operator<(nullopt_t, const optional<T>&) noexcept;
  template <class T> constexpr bool operator<=(const optional<T>&, nullopt_t) noexcept;
  template <class T> constexpr bool operator<=(nullopt_t, const optional<T>&) noexcept;
  template <class T> constexpr bool operator>(const optional<T>&, nullopt_t) noexcept;
  template <class T> constexpr bool operator>(nullopt_t, const optional<T>&) noexcept;
  template <class T> constexpr bool operator>=(const optional<T>&, nullopt_t) noexcept;
  template <class T> constexpr bool operator>=(nullopt_t, const optional<T>&) noexcept;

  // 23.6.8, comparison with T
  template <class T, class U> constexpr bool operator==(const optional<T>&, const U&);
  template <class T, class U> constexpr bool operator==(const T&, const optional<U>&);
  template <class T, class U> constexpr bool operator!=(const optional<T>&, const U&);
  template <class T, class U> constexpr bool operator!=(const T&, const optional<U>&);
  template <class T, class U> constexpr bool operator<(const optional<T>&, const U&);
  template <class T, class U> constexpr bool operator<(const T&, const optional<U>&);
  template <class T, class U> constexpr bool operator<=(const optional<T>&, const U&);
  template <class T, class U> constexpr bool operator<=(const T&, const optional<U>&);
  template <class T, class U> constexpr bool operator>(const optional<T>&, const U&);
  template <class T, class U> constexpr bool operator>(const T&, const optional<U>&);
  template <class T, class U> constexpr bool operator>=(const optional<T>&, const U&);
  template <class T, class U> constexpr bool operator>=(const T&, const optional<U>&);

  // 23.6.9, specialized algorithms
  template <class T> void swap(optional<T>&, optional<T>&) noexcept(see below );
  template <class T> constexpr optional<see below > make_optional(T&&);
  template <class T, class... Args>
    constexpr optional<T> make_optional(Args&&... args);
  template <class T, class U, class... Args>
    constexpr optional<T> make_optional(initializer_list<U> il, Args&&... args);

  // 23.6.10, hash support
  template <class T> struct hash;
  template <class T> struct hash<optional<T>>;

  template <class T> class optional {
  public:
    using value_type = T;

    // 23.6.3.1, constructors
    constexpr optional() noexcept;
    constexpr optional(nullopt_t) noexcept;
    optional(const optional &);
    optional(optional &&) noexcept(see below);
    template <class... Args> constexpr explicit optional(in_place_t, Args &&...);
    template <class U, class... Args>
      constexpr explicit optional(in_place_t, initializer_list<U>, Args &&...);
    template <class U = T>
      constexpr EXPLICIT optional(U &&);
    template <class U>
      constexpr EXPLICIT optional(const optional<U> &);
    template <class U>
      constexpr EXPLICIT optional(optional<U> &&);

    // 23.6.3.2, destructor
    ~optional();

    // 23.6.3.3, assignment
    optional &operator=(nullopt_t) noexcept;
    optional &operator=(const optional &);                // constexpr in C++20
    optional &operator=(optional &&) noexcept(see below); // constexpr in C++20
    template <class U = T> optional &operator=(U &&);
    template <class U> optional &operator=(const optional<U> &);
    template <class U> optional &operator=(optional<U> &&);
    template <class... Args> T& emplace(Args &&...);
    template <class U, class... Args>
      T& emplace(initializer_list<U>, Args &&...);

    // 23.6.3.4, swap
    void swap(optional &) noexcept(see below );

    // 23.6.3.5, observers
    constexpr T const *operator->() const;
    constexpr T *operator->();
    constexpr T const &operator*() const &;
    constexpr T &operator*() &;
    constexpr T &&operator*() &&;
    constexpr const T &&operator*() const &&;
    constexpr explicit operator bool() const noexcept;
    constexpr bool has_value() const noexcept;
    constexpr T const &value() const &;
    constexpr T &value() &;
    constexpr T &&value() &&;
    constexpr const T &&value() const &&;
    template <class U> constexpr T value_or(U &&) const &;
    template <class U> constexpr T value_or(U &&) &&;

    // 23.6.3.6, modifiers
    void reset() noexcept;

  private:
    T *val; // exposition only
  };

template<class T>
  optional(T) -> optional<T>;

} // namespace std

*/

#include <__config>
#include <__debug>
#include <__functional_base>
#include <functional>
#include <initializer_list>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <__cxx_version>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>


namespace std  // purposefully not using versioning namespace
{

class _LIBCPP_EXCEPTION_ABI _LIBCPP_AVAILABILITY_BAD_OPTIONAL_ACCESS bad_optional_access
    : public exception
{
public:
    // Get the key function ~bad_optional_access() into the dylib
    virtual ~bad_optional_access() _NOEXCEPT;
    virtual const char* what() const _NOEXCEPT;
};

}  // std

#if _LIBCPP_STD_VER > 14

_LIBCPP_BEGIN_NAMESPACE_STD

_LIBCPP_NORETURN
inline _LIBCPP_INLINE_VISIBILITY
_LIBCPP_AVAILABILITY_THROW_BAD_OPTIONAL_ACCESS
void __throw_bad_optional_access() {
#ifndef _LIBCPP_NO_EXCEPTIONS
        throw bad_optional_access();
#else
        _VSTD::abort();
#endif
}

struct nullopt_t
{
    struct __secret_tag { _LIBCPP_INLINE_VISIBILITY explicit __secret_tag() = default; };
    _LIBCPP_INLINE_VISIBILITY constexpr explicit nullopt_t(__secret_tag, __secret_tag) noexcept {}
};

_LIBCPP_INLINE_VAR constexpr nullopt_t nullopt{nullopt_t::__secret_tag{}, nullopt_t::__secret_tag{}};

template <class _Tp, bool = is_trivially_destructible<_Tp>::value>
struct __optional_destruct_base;

template <class _Tp>
struct __optional_destruct_base<_Tp, false>
{
    typedef _Tp value_type;
    static_assert(is_object_v<value_type>,
        "instantiation of optional with a non-object type is undefined behavior");
    union
    {
        char __null_state_;
        value_type __val_;
    };
    bool __engaged_;

    _LIBCPP_INLINE_VISIBILITY
    ~__optional_destruct_base()
    {
        if (__engaged_)
            __val_.~value_type();
    }

    _LIBCPP_INLINE_VISIBILITY
    constexpr __optional_destruct_base() noexcept
        :  __null_state_(),
           __engaged_(false) {}

    template <class... _Args>
    _LIBCPP_INLINE_VISIBILITY
    constexpr explicit __optional_destruct_base(in_place_t, _Args&&... __args)
        :  __val_(_VSTD::forward<_Args>(__args)...),
           __engaged_(true) {}

    _LIBCPP_INLINE_VISIBILITY
    void reset() noexcept
    {
        if (__engaged_)
        {
            __val_.~value_type();
            __engaged_ = false;
        }
    }
};

template <class _Tp>
struct __optional_destruct_base<_Tp, true>
{
    typedef _Tp value_type;
    static_assert(is_object_v<value_type>,
        "instantiation of optional with a non-object type is undefined behavior");
    union
    {
        char __null_state_;
        value_type __val_;
    };
    bool __engaged_;

    _LIBCPP_INLINE_VISIBILITY
    constexpr __optional_destruct_base() noexcept
        :  __null_state_(),
           __engaged_(false) {}

    template <class... _Args>
    _LIBCPP_INLINE_VISIBILITY
    constexpr explicit __optional_destruct_base(in_place_t, _Args&&... __args)
        :  __val_(_VSTD::forward<_Args>(__args)...),
           __engaged_(true) {}

    _LIBCPP_INLINE_VISIBILITY
    void reset() noexcept
    {
        if (__engaged_)
        {
            __engaged_ = false;
        }
    }
};

template <class _Tp, bool = is_reference<_Tp>::value>
struct __optional_storage_base : __optional_destruct_base<_Tp>
{
    using __base = __optional_destruct_base<_Tp>;
    using value_type = _Tp;
    using __base::__base;

    _LIBCPP_INLINE_VISIBILITY
    constexpr bool has_value() const noexcept
    {
        return this->__engaged_;
    }

    _LIBCPP_INLINE_VISIBILITY
    constexpr value_type& __get() & noexcept
    {
        return this->__val_;
    }
    _LIBCPP_INLINE_VISIBILITY
    constexpr const value_type& __get() const& noexcept
    {
        return this->__val_;
    }
    _LIBCPP_INLINE_VISIBILITY
    constexpr value_type&& __get() && noexcept
    {
        return _VSTD::move(this->__val_);
    }
    _LIBCPP_INLINE_VISIBILITY
    constexpr const value_type&& __get() const&& noexcept
    {
        return _VSTD::move(this->__val_);
    }

    template <class... _Args>
    _LIBCPP_INLINE_VISIBILITY
    void __construct(_Args&&... __args)
    {
        _LIBCPP_ASSERT(!has_value(), "__construct called for engaged __optional_storage");
        ::new((void*)_VSTD::addressof(this->__val_)) value_type(_VSTD::forward<_Args>(__args)...);
        this->__engaged_ = true;
    }

    template <class _That>
    _LIBCPP_INLINE_VISIBILITY
    void __construct_from(_That&& __opt)
    {
        if (__opt.has_value())
            __construct(_VSTD::forward<_That>(__opt).__get());
    }

    template <class _That>
    _LIBCPP_INLINE_VISIBILITY
    void __assign_from(_That&& __opt)
    {
        if (this->__engaged_ == __opt.has_value())
        {
            if (this->__engaged_)
                this->__val_ = _VSTD::forward<_That>(__opt).__get();
        }
        else
        {
            if (this->__engaged_)
                this->reset();
            else
                __construct(_VSTD::forward<_That>(__opt).__get());
        }
    }
};

// optional<T&> is currently required ill-formed, however it may to be in the
// future. For this reason it has already been implemented to ensure we can
// make the change in an ABI compatible manner.
template <class _Tp>
struct __optional_storage_base<_Tp, true>
{
    using value_type = _Tp;
    using __raw_type = remove_reference_t<_Tp>;
    __raw_type* __value_;

    template <class _Up>
    static constexpr bool __can_bind_reference() {
        using _RawUp = typename remove_reference<_Up>::type;
        using _UpPtr = _RawUp*;
        using _RawTp = typename remove_reference<_Tp>::type;
        using _TpPtr = _RawTp*;
        using _CheckLValueArg = integral_constant<bool,
            (is_lvalue_reference<_Up>::value && is_convertible<_UpPtr, _TpPtr>::value)
        ||  is_same<_RawUp, reference_wrapper<_RawTp>>::value
        ||  is_same<_RawUp, reference_wrapper<typename remove_const<_RawTp>::type>>::value
        >;
        return (is_lvalue_reference<_Tp>::value && _CheckLValueArg::value)
            || (is_rvalue_reference<_Tp>::value && !is_lvalue_reference<_Up>::value &&
                is_convertible<_UpPtr, _TpPtr>::value);
    }

    _LIBCPP_INLINE_VISIBILITY
    constexpr __optional_storage_base() noexcept
        :  __value_(nullptr) {}

    template <class _UArg>
    _LIBCPP_INLINE_VISIBILITY
    constexpr explicit __optional_storage_base(in_place_t, _UArg&& __uarg)
        :  __value_(_VSTD::addressof(__uarg))
    {
      static_assert(__can_bind_reference<_UArg>(),
        "Attempted to construct a reference element in tuple from a "
        "possible temporary");
    }

    _LIBCPP_INLINE_VISIBILITY
    void reset() noexcept { __value_ = nullptr; }

    _LIBCPP_INLINE_VISIBILITY
    constexpr bool has_value() const noexcept
      { return __value_ != nullptr; }

    _LIBCPP_INLINE_VISIBILITY
    constexpr value_type& __get() const& noexcept
      { return *__value_; }

    _LIBCPP_INLINE_VISIBILITY
    constexpr value_type&& __get() const&& noexcept
      { return _VSTD::forward<value_type>(*__value_); }

    template <class _UArg>
    _LIBCPP_INLINE_VISIBILITY
    void __construct(_UArg&& __val)
    {
        _LIBCPP_ASSERT(!has_value(), "__construct called for engaged __optional_storage");
        static_assert(__can_bind_reference<_UArg>(),
            "Attempted to construct a reference element in tuple from a "
            "possible temporary");
        __value_ = _VSTD::addressof(__val);
    }

    template <class _That>
    _LIBCPP_INLINE_VISIBILITY
    void __construct_from(_That&& __opt)
    {
        if (__opt.has_value())
            __construct(_VSTD::forward<_That>(__opt).__get());
    }

    template <class _That>
    _LIBCPP_INLINE_VISIBILITY
    void __assign_from(_That&& __opt)
    {
        if (has_value() == __opt.has_value())
        {
            if (has_value())
                *__value_ = _VSTD::forward<_That>(__opt).__get();
        }
        else
        {
            if (has_value())
                reset();
            else
                __construct(_VSTD::forward<_That>(__opt).__get());
        }
    }
};

template <class _Tp, bool = is_trivially_copy_constructible<_Tp>::value>
struct __optional_copy_base : __optional_storage_base<_Tp>
{
    using __optional_storage_base<_Tp>::__optional_storage_base;
};

template <class _Tp>
struct __optional_copy_base<_Tp, false> : __optional_storage_base<_Tp>
{
    using __optional_storage_base<_Tp>::__optional_storage_base;

    _LIBCPP_INLINE_VISIBILITY
    __optional_copy_base() = default;

    _LIBCPP_INLINE_VISIBILITY
    __optional_copy_base(const __optional_copy_base& __opt)
    {
        this->__construct_from(__opt);
    }

    _LIBCPP_INLINE_VISIBILITY
    __optional_copy_base(__optional_copy_base&&) = default;
    _LIBCPP_INLINE_VISIBILITY
    __optional_copy_base& operator=(const __optional_copy_base&) = default;
    _LIBCPP_INLINE_VISIBILITY
    __optional_copy_base& operator=(__optional_copy_base&&) = default;
};

template <class _Tp, bool = is_trivially_move_constructible<_Tp>::value>
struct __optional_move_base : __optional_copy_base<_Tp>
{
    using __optional_copy_base<_Tp>::__optional_copy_base;
};

template <class _Tp>
struct __optional_move_base<_Tp, false> : __optional_copy_base<_Tp>
{
    using value_type = _Tp;
    using __optional_copy_base<_Tp>::__optional_copy_base;

    _LIBCPP_INLINE_VISIBILITY
    __optional_move_base() = default;
    _LIBCPP_INLINE_VISIBILITY
    __optional_move_base(const __optional_move_base&) = default;

    _LIBCPP_INLINE_VISIBILITY
    __optional_move_base(__optional_move_base&& __opt)
        noexcept(is_nothrow_move_constructible_v<value_type>)
    {
        this->__construct_from(_VSTD::move(__opt));
    }

    _LIBCPP_INLINE_VISIBILITY
    __optional_move_base& operator=(const __optional_move_base&) = default;
    _LIBCPP_INLINE_VISIBILITY
    __optional_move_base& operator=(__optional_move_base&&) = default;
};

template <class _Tp, bool =
    is_trivially_destructible<_Tp>::value &&
    is_trivially_copy_constructible<_Tp>::value &&
    is_trivially_copy_assignable<_Tp>::value>
struct __optional_copy_assign_base : __optional_move_base<_Tp>
{
    using __optional_move_base<_Tp>::__optional_move_base;
};

template <class _Tp>
struct __optional_copy_assign_base<_Tp, false> : __optional_move_base<_Tp>
{
    using __optional_move_base<_Tp>::__optional_move_base;

    _LIBCPP_INLINE_VISIBILITY
    __optional_copy_assign_base() = default;
    _LIBCPP_INLINE_VISIBILITY
    __optional_copy_assign_base(const __optional_copy_assign_base&) = default;
    _LIBCPP_INLINE_VISIBILITY
    __optional_copy_assign_base(__optional_copy_assign_base&&) = default;

    _LIBCPP_INLINE_VISIBILITY
    __optional_copy_assign_base& operator=(const __optional_copy_assign_base& __opt)
    {
        this->__assign_from(__opt);
        return *this;
    }

    _LIBCPP_INLINE_VISIBILITY
    __optional_copy_assign_base& operator=(__optional_copy_assign_base&&) = default;
};

template <class _Tp, bool =
    is_trivially_destructible<_Tp>::value &&
    is_trivially_move_constructible<_Tp>::value &&
    is_trivially_move_assignable<_Tp>::value>
struct __optional_move_assign_base : __optional_copy_assign_base<_Tp>
{
    using __optional_copy_assign_base<_Tp>::__optional_copy_assign_base;
};

template <class _Tp>
struct __optional_move_assign_base<_Tp, false> : __optional_copy_assign_base<_Tp>
{
    using value_type = _Tp;
    using __optional_copy_assign_base<_Tp>::__optional_copy_assign_base;

    _LIBCPP_INLINE_VISIBILITY
    __optional_move_assign_base() = default;
    _LIBCPP_INLINE_VISIBILITY
    __optional_move_assign_base(const __optional_move_assign_base& __opt) = default;
    _LIBCPP_INLINE_VISIBILITY
    __optional_move_assign_base(__optional_move_assign_base&&) = default;
    _LIBCPP_INLINE_VISIBILITY
    __optional_move_assign_base& operator=(const __optional_move_assign_base&) = default;

    _LIBCPP_INLINE_VISIBILITY
    __optional_move_assign_base& operator=(__optional_move_assign_base&& __opt)
        noexcept(is_nothrow_move_assignable_v<value_type> &&
                 is_nothrow_move_constructible_v<value_type>)
    {
        this->__assign_from(_VSTD::move(__opt));
        return *this;
    }
};

template <class _Tp>
using __optional_sfinae_ctor_base_t = __sfinae_ctor_base<
    is_copy_constructible<_Tp>::value,
    is_move_constructible<_Tp>::value
>;

template <class _Tp>
using __optional_sfinae_assign_base_t = __sfinae_assign_base<
    (is_copy_constructible<_Tp>::value && is_copy_assignable<_Tp>::value),
    (is_move_constructible<_Tp>::value && is_move_assignable<_Tp>::value)
>;

template <class _Tp>
class optional
    : private __optional_move_assign_base<_Tp>
    , private __optional_sfinae_ctor_base_t<_Tp>
    , private __optional_sfinae_assign_base_t<_Tp>
{
    using __base = __optional_move_assign_base<_Tp>;
public:
    using value_type = _Tp;

private:
     // Disable the reference extension using this static assert.
    static_assert(!is_same_v<__uncvref_t<value_type>, in_place_t>,
        "instantiation of optional with in_place_t is ill-formed");
    static_assert(!is_same_v<__uncvref_t<value_type>, nullopt_t>,
        "instantiation of optional with nullopt_t is ill-formed");
    static_assert(!is_reference_v<value_type>,
        "instantiation of optional with a reference type is ill-formed");
    static_assert(is_destructible_v<value_type>,
        "instantiation of optional with a non-destructible type is ill-formed");
    static_assert(!is_array_v<value_type>,
        "instantiation of optional with an array type is ill-formed");

    // LWG2756: conditionally explicit conversion from _Up
    struct _CheckOptionalArgsConstructor {
      template <class _Up>
      static constexpr bool __enable_implicit() {
          return is_constructible_v<_Tp, _Up&&> &&
                 is_convertible_v<_Up&&, _Tp>;
      }

      template <class _Up>
      static constexpr bool __enable_explicit() {
          return is_constructible_v<_Tp, _Up&&> &&
                 !is_convertible_v<_Up&&, _Tp>;
      }
    };
    template <class _Up>
    using _CheckOptionalArgsCtor = _If<
        _IsNotSame<__uncvref_t<_Up>, in_place_t>::value &&
        _IsNotSame<__uncvref_t<_Up>, optional>::value,
        _CheckOptionalArgsConstructor,
        __check_tuple_constructor_fail
    >;
    template <class _QualUp>
    struct _CheckOptionalLikeConstructor {
      template <class _Up, class _Opt = optional<_Up>>
      using __check_constructible_from_opt = _Or<
          is_constructible<_Tp, _Opt&>,
          is_constructible<_Tp, _Opt const&>,
          is_constructible<_Tp, _Opt&&>,
          is_constructible<_Tp, _Opt const&&>,
          is_convertible<_Opt&, _Tp>,
          is_convertible<_Opt const&, _Tp>,
          is_convertible<_Opt&&, _Tp>,
          is_convertible<_Opt const&&, _Tp>
      >;
      template <class _Up, class _Opt = optional<_Up>>
      using __check_assignable_from_opt = _Or<
          is_assignable<_Tp&, _Opt&>,
          is_assignable<_Tp&, _Opt const&>,
          is_assignable<_Tp&, _Opt&&>,
          is_assignable<_Tp&, _Opt const&&>
      >;
      template <class _Up, class _QUp = _QualUp>
      static constexpr bool __enable_implicit() {
          return is_convertible<_QUp, _Tp>::value &&
              !__check_constructible_from_opt<_Up>::value;
      }
      template <class _Up, class _QUp = _QualUp>
      static constexpr bool __enable_explicit() {
          return !is_convertible<_QUp, _Tp>::value &&
              !__check_constructible_from_opt<_Up>::value;
      }
      template <class _Up, class _QUp = _QualUp>
      static constexpr bool __enable_assign() {
          // Construction and assignability of _Qup to _Tp has already been
          // checked.
          return !__check_constructible_from_opt<_Up>::value &&
              !__check_assignable_from_opt<_Up>::value;
      }
    };

    template <class _Up, class _QualUp>
    using _CheckOptionalLikeCtor = _If<
      _And<
         _IsNotSame<_Up, _Tp>,
          is_constructible<_Tp, _QualUp>
      >::value,
      _CheckOptionalLikeConstructor<_QualUp>,
      __check_tuple_constructor_fail
    >;
    template <class _Up, class _QualUp>
    using _CheckOptionalLikeAssign = _If<
      _And<
          _IsNotSame<_Up, _Tp>,
          is_constructible<_Tp, _QualUp>,
          is_assignable<_Tp&, _QualUp>
      >::value,
      _CheckOptionalLikeConstructor<_QualUp>,
      __check_tuple_constructor_fail
    >;
public:

    _LIBCPP_INLINE_VISIBILITY constexpr optional() noexcept {}
    _LIBCPP_INLINE_VISIBILITY constexpr optional(const optional&) = default;
    _LIBCPP_INLINE_VISIBILITY constexpr optional(optional&&) = default;
    _LIBCPP_INLINE_VISIBILITY constexpr optional(nullopt_t) noexcept {}

    template <class _InPlaceT, class... _Args, class = _EnableIf<
          _And<
              _IsSame<_InPlaceT, in_place_t>,
              is_constructible<value_type, _Args...>
            >::value
        >
    >
    _LIBCPP_INLINE_VISIBILITY
    constexpr explicit optional(_InPlaceT, _Args&&... __args)
        : __base(in_place, _VSTD::forward<_Args>(__args)...) {}

    template <class _Up, class... _Args, class = _EnableIf<
        is_constructible_v<value_type, initializer_list<_Up>&, _Args...>>
    >
    _LIBCPP_INLINE_VISIBILITY
    constexpr explicit optional(in_place_t, initializer_list<_Up> __il, _Args&&... __args)
        : __base(in_place, __il, _VSTD::forward<_Args>(__args)...) {}

    template <class _Up = value_type, _EnableIf<
        _CheckOptionalArgsCtor<_Up>::template __enable_implicit<_Up>()
    , int> = 0>
    _LIBCPP_INLINE_VISIBILITY
    constexpr optional(_Up&& __v)
        : __base(in_place, _VSTD::forward<_Up>(__v)) {}

    template <class _Up, _EnableIf<
        _CheckOptionalArgsCtor<_Up>::template __enable_explicit<_Up>()
    , int> = 0>
    _LIBCPP_INLINE_VISIBILITY
    constexpr explicit optional(_Up&& __v)
        : __base(in_place, _VSTD::forward<_Up>(__v)) {}

    // LWG2756: conditionally explicit conversion from const optional<_Up>&
    template <class _Up, _EnableIf<
        _CheckOptionalLikeCtor<_Up, _Up const&>::template __enable_implicit<_Up>()
    , int> = 0>
    _LIBCPP_INLINE_VISIBILITY
    optional(const optional<_Up>& __v)
    {
        this->__construct_from(__v);
    }
    template <class _Up, _EnableIf<
        _CheckOptionalLikeCtor<_Up, _Up const&>::template __enable_explicit<_Up>()
    , int> = 0>
    _LIBCPP_INLINE_VISIBILITY
    explicit optional(const optional<_Up>& __v)
    {
        this->__construct_from(__v);
    }

    // LWG2756: conditionally explicit conversion from optional<_Up>&&
    template <class _Up, _EnableIf<
        _CheckOptionalLikeCtor<_Up, _Up &&>::template __enable_implicit<_Up>()
    , int> = 0>
    _LIBCPP_INLINE_VISIBILITY
    optional(optional<_Up>&& __v)
    {
        this->__construct_from(_VSTD::move(__v));
    }
    template <class _Up, _EnableIf<
        _CheckOptionalLikeCtor<_Up, _Up &&>::template __enable_explicit<_Up>()
    , int> = 0>
    _LIBCPP_INLINE_VISIBILITY
    explicit optional(optional<_Up>&& __v)
    {
        this->__construct_from(_VSTD::move(__v));
    }

    _LIBCPP_INLINE_VISIBILITY
    optional& operator=(nullopt_t) noexcept
    {
        reset();
        return *this;
    }

    _LIBCPP_INLINE_VISIBILITY optional& operator=(const optional&) = default;
    _LIBCPP_INLINE_VISIBILITY optional& operator=(optional&&) = default;

    // LWG2756
    template <class _Up = value_type,
              class = _EnableIf<
                      _And<
                          _IsNotSame<__uncvref_t<_Up>, optional>,
                          _Or<
                              _IsNotSame<__uncvref_t<_Up>, value_type>,
                              _Not<is_scalar<value_type>>
                          >,
                          is_constructible<value_type, _Up>,
                          is_assignable<value_type&, _Up>
                      >::value>
             >
    _LIBCPP_INLINE_VISIBILITY
    optional&
    operator=(_Up&& __v)
    {
        if (this->has_value())
            this->__get() = _VSTD::forward<_Up>(__v);
        else
            this->__construct(_VSTD::forward<_Up>(__v));
        return *this;
    }

    // LWG2756
    template <class _Up, _EnableIf<
        _CheckOptionalLikeAssign<_Up, _Up const&>::template __enable_assign<_Up>()
    , int> = 0>
    _LIBCPP_INLINE_VISIBILITY
    optional&
    operator=(const optional<_Up>& __v)
    {
        this->__assign_from(__v);
        return *this;
    }

    // LWG2756
    template <class _Up, _EnableIf<
        _CheckOptionalLikeCtor<_Up, _Up &&>::template __enable_assign<_Up>()
    , int> = 0>
    _LIBCPP_INLINE_VISIBILITY
    optional&
    operator=(optional<_Up>&& __v)
    {
        this->__assign_from(_VSTD::move(__v));
        return *this;
    }

    template <class... _Args,
              class = _EnableIf
                      <
                          is_constructible_v<value_type, _Args...>
                      >
             >
    _LIBCPP_INLINE_VISIBILITY
    _Tp &
    emplace(_Args&&... __args)
    {
        reset();
        this->__construct(_VSTD::forward<_Args>(__args)...);
        return this->__get();
    }

    template <class _Up, class... _Args,
              class = _EnableIf
                      <
                          is_constructible_v<value_type, initializer_list<_Up>&, _Args...>
                      >
             >
    _LIBCPP_INLINE_VISIBILITY
    _Tp &
    emplace(initializer_list<_Up> __il, _Args&&... __args)
    {
        reset();
        this->__construct(__il, _VSTD::forward<_Args>(__args)...);
        return this->__get();
    }

    _LIBCPP_INLINE_VISIBILITY
    void swap(optional& __opt)
        noexcept(is_nothrow_move_constructible_v<value_type> &&
                 is_nothrow_swappable_v<value_type>)
    {
        if (this->has_value() == __opt.has_value())
        {
            using _VSTD::swap;
            if (this->has_value())
                swap(this->__get(), __opt.__get());
        }
        else
        {
            if (this->has_value())
            {
                __opt.__construct(_VSTD::move(this->__get()));
                reset();
            }
            else
            {
                this->__construct(_VSTD::move(__opt.__get()));
                __opt.reset();
            }
        }
    }

    _LIBCPP_INLINE_VISIBILITY
    constexpr
    add_pointer_t<value_type const>
    operator->() const
    {
        _LIBCPP_ASSERT(this->has_value(), "optional operator-> called for disengaged value");
#ifndef _LIBCPP_HAS_NO_BUILTIN_ADDRESSOF
        return _VSTD::addressof(this->__get());
#else
        return __operator_arrow(__has_operator_addressof<value_type>{}, this->__get());
#endif
    }

    _LIBCPP_INLINE_VISIBILITY
    constexpr
    add_pointer_t<value_type>
    operator->()
    {
        _LIBCPP_ASSERT(this->has_value(), "optional operator-> called for disengaged value");
#ifndef _LIBCPP_HAS_NO_BUILTIN_ADDRESSOF
        return _VSTD::addressof(this->__get());
#else
        return __operator_arrow(__has_operator_addressof<value_type>{}, this->__get());
#endif
    }

    _LIBCPP_INLINE_VISIBILITY
    constexpr
    const value_type&
    operator*() const&
    {
        _LIBCPP_ASSERT(this->has_value(), "optional operator* called for disengaged value");
        return this->__get();
    }

    _LIBCPP_INLINE_VISIBILITY
    constexpr
    value_type&
    operator*() &
    {
        _LIBCPP_ASSERT(this->has_value(), "optional operator* called for disengaged value");
        return this->__get();
    }

    _LIBCPP_INLINE_VISIBILITY
    constexpr
    value_type&&
    operator*() &&
    {
        _LIBCPP_ASSERT(this->has_value(), "optional operator* called for disengaged value");
        return _VSTD::move(this->__get());
    }

    _LIBCPP_INLINE_VISIBILITY
    constexpr
    const value_type&&
    operator*() const&&
    {
        _LIBCPP_ASSERT(this->has_value(), "optional operator* called for disengaged value");
        return _VSTD::move(this->__get());
    }

    _LIBCPP_INLINE_VISIBILITY
    constexpr explicit operator bool() const noexcept { return has_value(); }

    using __base::has_value;
    using __base::__get;

    _LIBCPP_INLINE_VISIBILITY
    _LIBCPP_AVAILABILITY_THROW_BAD_OPTIONAL_ACCESS
    constexpr value_type const& value() const&
    {
        if (!this->has_value())
            __throw_bad_optional_access();
        return this->__get();
    }

    _LIBCPP_INLINE_VISIBILITY
    _LIBCPP_AVAILABILITY_THROW_BAD_OPTIONAL_ACCESS
    constexpr value_type& value() &
    {
        if (!this->has_value())
            __throw_bad_optional_access();
        return this->__get();
    }

    _LIBCPP_INLINE_VISIBILITY
    _LIBCPP_AVAILABILITY_THROW_BAD_OPTIONAL_ACCESS
    constexpr value_type&& value() &&
    {
        if (!this->has_value())
            __throw_bad_optional_access();
        return _VSTD::move(this->__get());
    }

    _LIBCPP_INLINE_VISIBILITY
    _LIBCPP_AVAILABILITY_THROW_BAD_OPTIONAL_ACCESS
    constexpr value_type const&& value() const&&
    {
        if (!this->has_value())
            __throw_bad_optional_access();
        return _VSTD::move(this->__get());
    }

    template <class _Up>
    _LIBCPP_INLINE_VISIBILITY
    constexpr value_type value_or(_Up&& __v) const&
    {
        static_assert(is_copy_constructible_v<value_type>,
                      "optional<T>::value_or: T must be copy constructible");
        static_assert(is_convertible_v<_Up, value_type>,
                      "optional<T>::value_or: U must be convertible to T");
        return this->has_value() ? this->__get() :
                                  static_cast<value_type>(_VSTD::forward<_Up>(__v));
    }

    template <class _Up>
    _LIBCPP_INLINE_VISIBILITY
    constexpr value_type value_or(_Up&& __v) &&
    {
        static_assert(is_move_constructible_v<value_type>,
                      "optional<T>::value_or: T must be move constructible");
        static_assert(is_convertible_v<_Up, value_type>,
                      "optional<T>::value_or: U must be convertible to T");
        return this->has_value() ? _VSTD::move(this->__get()) :
                                  static_cast<value_type>(_VSTD::forward<_Up>(__v));
    }

    using __base::reset;

private:
    template <class _Up>
    _LIBCPP_INLINE_VISIBILITY
    static _Up*
    __operator_arrow(true_type, _Up& __x)
    {
        return _VSTD::addressof(__x);
    }

    template <class _Up>
    _LIBCPP_INLINE_VISIBILITY
    static constexpr _Up*
    __operator_arrow(false_type, _Up& __x)
    {
        return &__x;
    }
};

#ifndef _LIBCPP_HAS_NO_DEDUCTION_GUIDES
template<class T>
    optional(T) -> optional<T>;
#endif

// Comparisons between optionals
template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() ==
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator==(const optional<_Tp>& __x, const optional<_Up>& __y)
{
    if (static_cast<bool>(__x) != static_cast<bool>(__y))
        return false;
    if (!static_cast<bool>(__x))
        return true;
    return *__x == *__y;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() !=
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator!=(const optional<_Tp>& __x, const optional<_Up>& __y)
{
    if (static_cast<bool>(__x) != static_cast<bool>(__y))
        return true;
    if (!static_cast<bool>(__x))
        return false;
    return *__x != *__y;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() <
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator<(const optional<_Tp>& __x, const optional<_Up>& __y)
{
    if (!static_cast<bool>(__y))
        return false;
    if (!static_cast<bool>(__x))
        return true;
    return *__x < *__y;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() >
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator>(const optional<_Tp>& __x, const optional<_Up>& __y)
{
    if (!static_cast<bool>(__x))
        return false;
    if (!static_cast<bool>(__y))
        return true;
    return *__x > *__y;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() <=
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator<=(const optional<_Tp>& __x, const optional<_Up>& __y)
{
    if (!static_cast<bool>(__x))
        return true;
    if (!static_cast<bool>(__y))
        return false;
    return *__x <= *__y;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() >=
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator>=(const optional<_Tp>& __x, const optional<_Up>& __y)
{
    if (!static_cast<bool>(__y))
        return true;
    if (!static_cast<bool>(__x))
        return false;
    return *__x >= *__y;
}

// Comparisons with nullopt
template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
bool
operator==(const optional<_Tp>& __x, nullopt_t) noexcept
{
    return !static_cast<bool>(__x);
}

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
bool
operator==(nullopt_t, const optional<_Tp>& __x) noexcept
{
    return !static_cast<bool>(__x);
}

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
bool
operator!=(const optional<_Tp>& __x, nullopt_t) noexcept
{
    return static_cast<bool>(__x);
}

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
bool
operator!=(nullopt_t, const optional<_Tp>& __x) noexcept
{
    return static_cast<bool>(__x);
}

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
bool
operator<(const optional<_Tp>&, nullopt_t) noexcept
{
    return false;
}

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
bool
operator<(nullopt_t, const optional<_Tp>& __x) noexcept
{
    return static_cast<bool>(__x);
}

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
bool
operator<=(const optional<_Tp>& __x, nullopt_t) noexcept
{
    return !static_cast<bool>(__x);
}

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
bool
operator<=(nullopt_t, const optional<_Tp>&) noexcept
{
    return true;
}

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
bool
operator>(const optional<_Tp>& __x, nullopt_t) noexcept
{
    return static_cast<bool>(__x);
}

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
bool
operator>(nullopt_t, const optional<_Tp>&) noexcept
{
    return false;
}

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
bool
operator>=(const optional<_Tp>&, nullopt_t) noexcept
{
    return true;
}

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
bool
operator>=(nullopt_t, const optional<_Tp>& __x) noexcept
{
    return !static_cast<bool>(__x);
}

// Comparisons with T
template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() ==
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator==(const optional<_Tp>& __x, const _Up& __v)
{
    return static_cast<bool>(__x) ? *__x == __v : false;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() ==
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator==(const _Tp& __v, const optional<_Up>& __x)
{
    return static_cast<bool>(__x) ? __v == *__x : false;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() !=
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator!=(const optional<_Tp>& __x, const _Up& __v)
{
    return static_cast<bool>(__x) ? *__x != __v : true;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() !=
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator!=(const _Tp& __v, const optional<_Up>& __x)
{
    return static_cast<bool>(__x) ? __v != *__x : true;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() <
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator<(const optional<_Tp>& __x, const _Up& __v)
{
    return static_cast<bool>(__x) ? *__x < __v : true;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() <
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator<(const _Tp& __v, const optional<_Up>& __x)
{
    return static_cast<bool>(__x) ? __v < *__x : false;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() <=
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator<=(const optional<_Tp>& __x, const _Up& __v)
{
    return static_cast<bool>(__x) ? *__x <= __v : true;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() <=
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator<=(const _Tp& __v, const optional<_Up>& __x)
{
    return static_cast<bool>(__x) ? __v <= *__x : false;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() >
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator>(const optional<_Tp>& __x, const _Up& __v)
{
    return static_cast<bool>(__x) ? *__x > __v : false;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() >
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator>(const _Tp& __v, const optional<_Up>& __x)
{
    return static_cast<bool>(__x) ? __v > *__x : true;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() >=
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator>=(const optional<_Tp>& __x, const _Up& __v)
{
    return static_cast<bool>(__x) ? *__x >= __v : false;
}

template <class _Tp, class _Up>
_LIBCPP_INLINE_VISIBILITY constexpr
_EnableIf<
    is_convertible_v<decltype(_VSTD::declval<const _Tp&>() >=
        _VSTD::declval<const _Up&>()), bool>,
    bool
>
operator>=(const _Tp& __v, const optional<_Up>& __x)
{
    return static_cast<bool>(__x) ? __v >= *__x : true;
}


template <class _Tp>
inline _LIBCPP_INLINE_VISIBILITY
_EnableIf<
    is_move_constructible_v<_Tp> && is_swappable_v<_Tp>,
    void
>
swap(optional<_Tp>& __x, optional<_Tp>& __y) noexcept(noexcept(__x.swap(__y)))
{
    __x.swap(__y);
}

template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
optional<decay_t<_Tp>> make_optional(_Tp&& __v)
{
    return optional<decay_t<_Tp>>(_VSTD::forward<_Tp>(__v));
}

template <class _Tp, class... _Args>
_LIBCPP_INLINE_VISIBILITY constexpr
optional<_Tp> make_optional(_Args&&... __args)
{
    return optional<_Tp>(in_place, _VSTD::forward<_Args>(__args)...);
}

template <class _Tp, class _Up, class... _Args>
_LIBCPP_INLINE_VISIBILITY constexpr
optional<_Tp> make_optional(initializer_list<_Up> __il,  _Args&&... __args)
{
    return optional<_Tp>(in_place, __il, _VSTD::forward<_Args>(__args)...);
}

template <class _Tp>
struct _LIBCPP_TEMPLATE_VIS hash<
    __enable_hash_helper<optional<_Tp>, remove_const_t<_Tp>>
>
{
    typedef optional<_Tp> argument_type;
    typedef size_t        result_type;

    _LIBCPP_INLINE_VISIBILITY
    result_type operator()(const argument_type& __opt) const
    {
        return static_cast<bool>(__opt) ? hash<remove_const_t<_Tp>>()(*__opt) : 0;
    }
};

_LIBCPP_END_NAMESPACE_STD

#endif  // _LIBCPP_STD_VER > 14

_LIBCPP_POP_MACROS

#endif  // _LIBCPP_OPTIONAL

optional<int> lol(){
  return 2;
}
int main(){

}