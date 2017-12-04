#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/cppspec.hpp"
#ifndef CPPSPEC_HPP
#define CPPSPEC_HPP
#pragma once

#ifndef CPPSPEC_DEBUG
#define CPPSPEC_DEBUG false
#endif
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/runner.hpp"
#ifndef CPPSPEC_RUNNER_HPP
#define CPPSPEC_RUNNER_HPP
#pragma once

#include <list>
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/formatters/verbose.hpp"
#ifndef CPPSPEC_FORMATTERS_VERBOSE_HPP
#define CPPSPEC_FORMATTERS_VERBOSE_HPP
#pragma once

#include <string>
#include <list>
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/formatters/formatters_base.hpp"
#ifndef CPPSPEC_FORMATTERS_FORMATTERS_BASE
#define CPPSPEC_FORMATTERS_FORMATTERS_BASE
#pragma once

#include <iostream>
#include <string>
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/formatters/term_colors.hpp"
#ifndef CPPSPEC_TERM_COLORS_HPP
#define CPPSPEC_TERM_COLORS_HPP
#pragma once

#define RESET std::string("\033[0m")
#define BLACK std::string("\033[30m")              /* Black */
#define RED std::string("\033[31m")                /* Red */
#define GREEN std::string("\033[32m")              /* Green */
#define YELLOW std::string("\033[33m")             /* Yellow */
#define BLUE std::string("\033[34m")               /* Blue */
#define MAGENTA std::string("\033[35m")            /* Magenta */
#define CYAN std::string("\033[36m")               /* Cyan */
#define WHITE std::string("\033[37m")              /* White */
#define BOLDBLACK std::string("\033[1m\033[30m")   /* Bold Black */
#define BOLDRED std::string("\033[1m\033[31m")     /* Bold Red */
#define BOLDGREEN std::string("\033[1m\033[32m")   /* Bold Green */
#define BOLDYELLOW std::string("\033[1m\033[33m")  /* Bold Yellow */
#define BOLDBLUE std::string("\033[1m\033[34m")    /* Bold Blue */
#define BOLDMAGENTA std::string("\033[1m\033[35m") /* Bold Magenta */
#define BOLDCYAN std::string("\033[1m\033[36m")    /* Bold Cyan */
#define BOLDWHITE std::string("\033[1m\033[37m")   /* Bold White */

#endif //CPPSPEC_TERM_COLORS_HPP
#line 8 "/mnt/c/Users/Kate/GitHub/cppspec/include/formatters/formatters_base.hpp"

namespace CppSpec {
class Description;
class ItBase;

namespace Formatters {

class BaseFormatter {
 protected:
  std::ostream &out_stream;
  int test_counter = 1;
  bool multiple = false;
  bool color_output;

 public:
  explicit BaseFormatter(std::ostream &out_stream = std::cout,
                         bool color = true)
      : out_stream(out_stream), color_output(color) {}
  BaseFormatter(const BaseFormatter &) = default;
  BaseFormatter(const BaseFormatter &copy, std::ostream &out_stream)
      : out_stream(out_stream),
        test_counter(copy.test_counter),
        multiple(copy.multiple),
        color_output(copy.color_output) {}

  virtual ~BaseFormatter() = default;

  virtual void format(const Description &description) {}
  virtual void format(const ItBase &it) {}
  virtual void format(std::string message) {
    out_stream << message << std::endl;
  }
  virtual void format_failure(std::string message) {}
  virtual void flush() {}
  virtual void cleanup() {}

  BaseFormatter &set_multiple(bool value);
  BaseFormatter &set_color_output(bool value);

  int get_and_increment_test_counter() { return test_counter++; }
  void reset_test_counter() { test_counter = 1; }
};

inline BaseFormatter &BaseFormatter::set_multiple(bool multiple) {
  this->multiple = multiple;
  return *this;
}

inline BaseFormatter &BaseFormatter::set_color_output(bool value) {
  this->color_output = value;
  return *this;
}

}  // namespace Formatters
}  // namespace CppSpec
#endif  // CPPSPEC_FORMATTERS_FORMATTERS_BASE
#line 8 "/mnt/c/Users/Kate/GitHub/cppspec/include/formatters/verbose.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/class_description.hpp"
#ifndef CPPSPEC_CLASS_DESCRIPTION_HPP
#define CPPSPEC_CLASS_DESCRIPTION_HPP
#include <string>
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/description.hpp"
#ifndef CPPSPEC_DESCRIPTION_HPP
#define CPPSPEC_DESCRIPTION_HPP
#pragma once

#include <string>
#include <deque>
#include <queue>
#include <forward_list>
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/it.hpp"
#ifndef CPPSPEC_IT_HPP
#define CPPSPEC_IT_HPP
#pragma once

#include <string>
#include <vector>
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#ifndef CPPSPEC_EXPECTATIONS_EXPECTATION_HPP
#define CPPSPEC_EXPECTATIONS_EXPECTATION_HPP
#pragma once

#include <string>
#include <regex>
#include <vector>
#include <exception>
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/be_between.hpp"
#ifndef CPPSPEC_MATCHERS_BE_BETWEEN_HPP
#define CPPSPEC_MATCHERS_BE_BETWEEN_HPP
#pragma once

#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/matcher_base.hpp"
#ifndef CPPSPEC_MATCHERS_MATCHER_BASE_HPP
#define CPPSPEC_MATCHERS_MATCHER_BASE_HPP
#pragma once

#include <string>
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/handler.hpp"
#ifndef CPPSPEC_EXPECTATIONS_HANDLER_HPP
#define CPPSPEC_EXPECTATIONS_HANDLER_HPP
#pragma once

#include <string>
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/result.hpp"
#ifndef CPPSPEC_RESULT_HPP
#define CPPSPEC_RESULT_HPP
#pragma once

#include <string>
#include <sstream>
#include <ciso646>

namespace CppSpec {

class Result {
  const bool value;
  std::string message;
  explicit Result(bool value, std::string message = "") noexcept
      : value(value),
        message(message) {}

 public:
  virtual ~Result() = default;

  Result(const Result &) = default;
  Result &operator=(const Result &) = default;

  Result(Result &&) = default;
  Result &operator=(Result &&) = default;

  /*--------- Status helper functions --------------*/
  const bool get_status() noexcept { return value; }
  const bool get_status() const noexcept { return value; }

  operator bool() noexcept { return this->get_status(); }
  operator bool() const noexcept { return this->get_status(); }

  /*--------- Message helper functions -------------*/
  const std::string get_message() noexcept { return message; }
  const std::string get_message() const noexcept { return message; }
  Result &set_message(std::string message) noexcept;

  /*--------- Explicit constructor functions -------*/
  static Result success() noexcept;
  static Result failure() noexcept;
  static Result success_with(std::string success_message) noexcept;
  static Result failure_with(std::string failure_message) noexcept;

  /*-------------- Friend functions ----------------*/

  friend std::ostream &operator<<(std::ostream &os, const Result &res);
};

inline Result &Result::set_message(std::string message) noexcept {
  this->message = message;
  return *this;
}

inline Result Result::success() noexcept { return Result(true); }
inline Result Result::success_with(std::string success_message) noexcept {
  return Result(true, success_message);
}

inline Result Result::failure() noexcept { return Result(false); }
inline Result Result::failure_with(std::string failure_message) noexcept {
  return Result(false, failure_message);
}

inline std::ostream &operator<<(std::ostream &os, const Result &res) {
  std::stringstream ss;
  ss << (res.get_status() ? "Result::success" : "Result::failure");

  if (not res.get_message().empty()) {
    ss << "(\"" + res.get_message() + "\")";
  }

  return os << ss.str();
}

}  // namespace CppSpec
#endif  // CPPSPEC_RESULT_HPP
#line 7 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/handler.hpp"

namespace CppSpec {

struct PositiveExpectationHandler {
  template <class Matcher>
  static Result handle_matcher(Matcher &matcher);
  static std::string verb() { return "should"; }
};

struct NegativeExpectationHandler {
  template <class Matcher>
  static Result handle_matcher(Matcher &matcher);
  static std::string verb() { return "should not"; }
};

template <class Matcher>
Result PositiveExpectationHandler::handle_matcher(Matcher &matcher) {
  return !matcher.match() ? Result::failure_with(matcher.failure_message())
                          : Result::success();
}

template <class Matcher>
Result NegativeExpectationHandler::handle_matcher(Matcher &matcher) {
  return !matcher.negated_match()
             ? Result::failure_with(matcher.failure_message_when_negated())
             : Result::success();
}

}  // namespace CppSpec
#endif  // CPPSPEC_EXPECTATIONS_HANDLER_HPP
#line 7 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/matcher_base.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/it_base.hpp"
#ifndef CPPSPEC_IT_BASE_HPP
#define CPPSPEC_IT_BASE_HPP
#pragma once

#include <string>
#include <vector>
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/runnable.hpp"
#ifndef CPPSPEC_RUNNABLE_HPP
#define CPPSPEC_RUNNABLE_HPP
#pragma once

#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/child.hpp"
#ifndef CPPSPEC_CHILD_HPP
#define CPPSPEC_CHILD_HPP
#pragma once

#ifndef CPPSPEC_DEBUG
#define CPPSPEC_DEBUG false
#endif

#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

namespace CppSpec {

namespace Formatters {
class BaseFormatter;  // Forward declaration to allow reference
}

class Child {
  Child *parent = nullptr;

  bool status = true;

  Formatters::BaseFormatter *formatter = nullptr;

 public:
  Child() = default;
  virtual ~Child() = default;

  Child(Child &&) = default;
  Child &operator=(Child &&) = default;

  Child(const Child &) = default;
  Child &operator=(const Child &) = default;

  static Child of(const Child &parent) noexcept {
    return Child().set_parent(&parent);
  }

  /*--------- Parent helper functions -------------*/

  const bool has_parent() noexcept { return parent != nullptr; }
  const bool has_parent() const noexcept { return parent != nullptr; }

  Child *get_parent() const noexcept { return parent; }

  template <class C>
  C get_parent_as() const noexcept {
    return static_cast<C>(get_parent());
  }

  Child set_parent(Child *parent) noexcept {
    this->parent = parent;
    return *this;
  }
  Child &set_parent(const Child *parent) noexcept {
    this->parent = const_cast<Child *>(parent);
    return *this;
  }

  /*--------- Formatter helper functions -----------*/
  const bool has_formatter() const noexcept;

  Formatters::BaseFormatter &get_formatter() const noexcept;

  void set_formatter(const Formatters::BaseFormatter &formatter) {
    this->formatter = &const_cast<Formatters::BaseFormatter &>(formatter);
  }

  /*--------- Primary member functions -------------*/

  const bool get_status() const noexcept { return this->status; }
  void failed() noexcept;  // Report failure to the object.

  std::string padding() const noexcept;
};

/*>>>>>>>>>>>>>>>>>>>> Child <<<<<<<<<<<<<<<<<<<<<<<<<*/

inline void Child::failed() noexcept {
  this->status = false;
  if (this->has_parent()) this->get_parent()->failed();
}

inline std::string Child::padding() const noexcept {
  return this->has_parent() ? this->get_parent()->padding() + "  " : "";
}

inline const bool Child::has_formatter() const noexcept {
  if (this->formatter != nullptr) return true;
  if (!this->has_parent()) return false;  // base case;
  return parent->has_formatter();
}

inline Formatters::BaseFormatter &Child::get_formatter() const noexcept {
  if (this->formatter) return *formatter;
  if (!this->has_parent()) std::terminate();
  return parent->get_formatter();
}

}  // namespace CppSpec
#endif  // CPPSPEC_CHILD_HPP
#line 6 "/mnt/c/Users/Kate/GitHub/cppspec/include/runnable.hpp"
#line 7 "/mnt/c/Users/Kate/GitHub/cppspec/include/runnable.hpp"

namespace CppSpec {

class Runnable : public Child {
 public:
  Runnable() = default;
  virtual ~Runnable() = default;

  Runnable(Runnable &&) = default;
  Runnable &operator=(Runnable &&) = default;

  Runnable(const Runnable &parent) = default;
  Runnable &operator=(const Runnable &) = delete;

  explicit Runnable(const Child &parent) noexcept : Child(Child::of(parent)) {}

  virtual Result run(Formatters::BaseFormatter &printer) = 0;
};

}  // namespace CppSpec
#endif  // CPPSPEC_RUNNABLE_HPP
#line 8 "/mnt/c/Users/Kate/GitHub/cppspec/include/it_base.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/let.hpp"
#ifndef CPPSPEC_LET_HPP
#define CPPSPEC_LET_HPP
#pragma once

#include <functional>
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/optional/optional.hpp"

# ifndef ___OPTIONAL_HPP___
# define ___OPTIONAL_HPP___

# include <utility>
# include <type_traits>
# include <initializer_list>
# include <cassert>
# include <functional>
# include <string>
# include <stdexcept>

# define TR2_OPTIONAL_REQUIRES(...) typename enable_if<__VA_ARGS__::value, bool>::type = false

# if defined __GNUC__ // NOTE: GNUC is also defined for Clang
#   if (__GNUC__ == 4) && (__GNUC_MINOR__ >= 8)
#     define TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
#   elif (__GNUC__ > 4)
#     define TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
#   endif
#
#   if (__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)
#     define TR2_OPTIONAL_GCC_4_7_AND_HIGHER___
#   elif (__GNUC__ > 4)
#     define TR2_OPTIONAL_GCC_4_7_AND_HIGHER___
#   endif
#
#   if (__GNUC__ == 4) && (__GNUC_MINOR__ == 8) && (__GNUC_PATCHLEVEL__ >= 1)
#     define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   elif (__GNUC__ == 4) && (__GNUC_MINOR__ >= 9)
#     define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   elif (__GNUC__ > 4)
#     define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   endif
# endif
#
# if defined __clang_major__
#   if (__clang_major__ == 3 && __clang_minor__ >= 5)
#     define TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#   elif (__clang_major__ > 3)
#     define TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#   endif
#   if defined TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#     define TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
#   elif (__clang_major__ == 3 && __clang_minor__ == 4 && __clang_patchlevel__ >= 2)
#     define TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
#   endif
# endif
#
# if defined _MSC_VER
#   if (_MSC_VER >= 1900)
#     define TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
#   endif
# endif

# if defined __clang__
#   if (__clang_major__ > 2) || (__clang_major__ == 2) && (__clang_minor__ >= 9)
#     define OPTIONAL_HAS_THIS_RVALUE_REFS 1
#   else
#     define OPTIONAL_HAS_THIS_RVALUE_REFS 0
#   endif
# elif defined TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   define OPTIONAL_HAS_THIS_RVALUE_REFS 1
# elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
#   define OPTIONAL_HAS_THIS_RVALUE_REFS 1
# else
#   define OPTIONAL_HAS_THIS_RVALUE_REFS 0
# endif


# if defined TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   define OPTIONAL_HAS_CONSTEXPR_INIT_LIST 1
#   define OPTIONAL_CONSTEXPR_INIT_LIST constexpr
# else
#   define OPTIONAL_HAS_CONSTEXPR_INIT_LIST 0
#   define OPTIONAL_CONSTEXPR_INIT_LIST
# endif

# if defined TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_ && (defined __cplusplus) && (__cplusplus != 201103L)
#   define OPTIONAL_HAS_MOVE_ACCESSORS 1
# else
#   define OPTIONAL_HAS_MOVE_ACCESSORS 0
# endif

# // In C++11 constexpr implies const, so we need to make non-const members also non-constexpr
# if (defined __cplusplus) && (__cplusplus == 201103L)
#   define OPTIONAL_MUTABLE_CONSTEXPR
# else
#   define OPTIONAL_MUTABLE_CONSTEXPR constexpr
# endif

namespace std{

namespace experimental{

# if defined TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
# elif defined TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
# elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
# elif defined TR2_OPTIONAL_DISABLE_EMULATION_OF_TYPE_TRAITS
# else
	template <typename T>
	using is_trivially_destructible = std::has_trivial_destructor<T>;
# endif

# if (defined TR2_OPTIONAL_GCC_4_7_AND_HIGHER___)
# elif defined TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
# elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
# elif defined TR2_OPTIONAL_DISABLE_EMULATION_OF_TYPE_TRAITS
# else


template <class T>
struct is_nothrow_move_constructible
{
  constexpr static bool value = std::is_nothrow_constructible<T, T&&>::value;
};


template <class T, class U>
struct is_assignable
{
  template <class X, class Y>
  constexpr static bool has_assign(...) { return false; }

  template <class X, class Y, size_t S = sizeof((std::declval<X>() = std::declval<Y>(), true)) >
  constexpr static bool has_assign(bool) { return true; }

  constexpr static bool value = has_assign<T, U>(true);
};


template <class T>
struct is_nothrow_move_assignable
{
  template <class X, bool has_any_move_assign>
  struct has_nothrow_move_assign {
    constexpr static bool value = false;
  };

  template <class X>
  struct has_nothrow_move_assign<X, true> {
    constexpr static bool value = noexcept( std::declval<X&>() = std::declval<X&&>() );
  };

  constexpr static bool value = has_nothrow_move_assign<T, is_assignable<T&, T&&>::value>::value;
};


# endif



template <class T> class optional;

template <class T> class optional<T&>;


template <class T> inline constexpr T&& constexpr_forward(typename std::remove_reference<T>::type& t) noexcept
{
  return static_cast<T&&>(t);
}

template <class T> inline constexpr T&& constexpr_forward(typename std::remove_reference<T>::type&& t) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "!!");
    return static_cast<T&&>(t);
}

template <class T> inline constexpr typename std::remove_reference<T>::type&& constexpr_move(T&& t) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}


#if defined NDEBUG
# define TR2_OPTIONAL_ASSERTED_EXPRESSION(CHECK, EXPR) (EXPR)
#else
# define TR2_OPTIONAL_ASSERTED_EXPRESSION(CHECK, EXPR) ((CHECK) ? (EXPR) : ([]{assert(!#CHECK);}(), (EXPR)))
#endif


namespace detail_
{

template <typename T>
struct has_overloaded_addressof
{
  template <class X>
  constexpr static bool has_overload(...) { return false; }
  
  template <class X, size_t S = sizeof(std::declval<X&>().operator&()) >
  constexpr static bool has_overload(bool) { return true; }

  constexpr static bool value = has_overload<T>(true);
};

template <typename T, TR2_OPTIONAL_REQUIRES(!has_overloaded_addressof<T>)>
constexpr T* static_addressof(T& ref)
{
  return &ref;
}

template <typename T, TR2_OPTIONAL_REQUIRES(has_overloaded_addressof<T>)>
T* static_addressof(T& ref)
{
  return std::addressof(ref);
}


template <class U>
U convert(U v) { return v; }
  
} // namespace detail


constexpr struct trivial_init_t{} trivial_init{};


constexpr struct in_place_t{} in_place{};


struct nullopt_t
{
  struct init{};
  constexpr explicit nullopt_t(init){}
};
constexpr nullopt_t nullopt{nullopt_t::init()};


class bad_optional_access : public logic_error {
public:
  explicit bad_optional_access(const string& what_arg) : logic_error{what_arg} {}
  explicit bad_optional_access(const char* what_arg) : logic_error{what_arg} {}
};


template <class T>
union storage_t
{
  unsigned char dummy_;
  T value_;

  constexpr storage_t( trivial_init_t ) noexcept : dummy_() {};

  template <class... Args>
  constexpr storage_t( Args&&... args ) : value_(constexpr_forward<Args>(args)...) {}

  ~storage_t(){}
};


template <class T>
union constexpr_storage_t
{
    unsigned char dummy_;
    T value_;

    constexpr constexpr_storage_t( trivial_init_t ) noexcept : dummy_() {};

    template <class... Args>
    constexpr constexpr_storage_t( Args&&... args ) : value_(constexpr_forward<Args>(args)...) {}

    ~constexpr_storage_t() = default;
};


template <class T>
struct optional_base
{
    bool init_;
    storage_t<T> storage_;

    constexpr optional_base() noexcept : init_(false), storage_(trivial_init) {};

    explicit constexpr optional_base(const T& v) : init_(true), storage_(v) {}

    explicit constexpr optional_base(T&& v) : init_(true), storage_(constexpr_move(v)) {}

    template <class... Args> explicit optional_base(in_place_t, Args&&... args)
        : init_(true), storage_(constexpr_forward<Args>(args)...) {}

    template <class U, class... Args, TR2_OPTIONAL_REQUIRES(is_constructible<T, std::initializer_list<U>>)>
    explicit optional_base(in_place_t, std::initializer_list<U> il, Args&&... args)
        : init_(true), storage_(il, std::forward<Args>(args)...) {}

    ~optional_base() { if (init_) storage_.value_.T::~T(); }
};


template <class T>
struct constexpr_optional_base
{
    bool init_;
    constexpr_storage_t<T> storage_;

    constexpr constexpr_optional_base() noexcept : init_(false), storage_(trivial_init) {};

    explicit constexpr constexpr_optional_base(const T& v) : init_(true), storage_(v) {}

    explicit constexpr constexpr_optional_base(T&& v) : init_(true), storage_(constexpr_move(v)) {}

    template <class... Args> explicit constexpr constexpr_optional_base(in_place_t, Args&&... args)
      : init_(true), storage_(constexpr_forward<Args>(args)...) {}

    template <class U, class... Args, TR2_OPTIONAL_REQUIRES(is_constructible<T, std::initializer_list<U>>)>
    OPTIONAL_CONSTEXPR_INIT_LIST explicit constexpr_optional_base(in_place_t, std::initializer_list<U> il, Args&&... args)
      : init_(true), storage_(il, std::forward<Args>(args)...) {}

    ~constexpr_optional_base() = default;
};

template <class T>
using OptionalBase = typename std::conditional<
    is_trivially_destructible<T>::value,
    constexpr_optional_base<T>,
    optional_base<T>
>::type;



template <class T>
class optional : private OptionalBase<T>
{
  static_assert( !std::is_same<typename std::decay<T>::type, nullopt_t>::value, "bad T" );
  static_assert( !std::is_same<typename std::decay<T>::type, in_place_t>::value, "bad T" );
  

  constexpr bool initialized() const noexcept { return OptionalBase<T>::init_; }
  T* dataptr() {  return std::addressof(OptionalBase<T>::storage_.value_); }
  constexpr const T* dataptr() const { return detail_::static_addressof(OptionalBase<T>::storage_.value_); }
  
# if OPTIONAL_HAS_THIS_RVALUE_REFS == 1
  constexpr const T& contained_val() const& { return OptionalBase<T>::storage_.value_; }
#   if OPTIONAL_HAS_MOVE_ACCESSORS == 1
  OPTIONAL_MUTABLE_CONSTEXPR T&& contained_val() && { return std::move(OptionalBase<T>::storage_.value_); }
  OPTIONAL_MUTABLE_CONSTEXPR T& contained_val() & { return OptionalBase<T>::storage_.value_; }
#   else
  T& contained_val() & { return OptionalBase<T>::storage_.value_; }
  T&& contained_val() && { return std::move(OptionalBase<T>::storage_.value_); }
#   endif
# else
  constexpr const T& contained_val() const { return OptionalBase<T>::storage_.value_; }
  T& contained_val() { return OptionalBase<T>::storage_.value_; }
# endif

  void clear() noexcept {
    if (initialized()) dataptr()->T::~T();
    OptionalBase<T>::init_ = false;
  }
  
  template <class... Args>
  void initialize(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...)))
  {
    assert(!OptionalBase<T>::init_);
    ::new (static_cast<void*>(dataptr())) T(std::forward<Args>(args)...);
    OptionalBase<T>::init_ = true;
  }

  template <class U, class... Args>
  void initialize(std::initializer_list<U> il, Args&&... args) noexcept(noexcept(T(il, std::forward<Args>(args)...)))
  {
    assert(!OptionalBase<T>::init_);
    ::new (static_cast<void*>(dataptr())) T(il, std::forward<Args>(args)...);
    OptionalBase<T>::init_ = true;
  }

public:
  typedef T value_type;

  constexpr optional() noexcept : OptionalBase<T>()  {};
  constexpr optional(nullopt_t) noexcept : OptionalBase<T>() {};

  optional(const optional& rhs)
  : OptionalBase<T>()
  {
    if (rhs.initialized()) {
        ::new (static_cast<void*>(dataptr())) T(*rhs);
        OptionalBase<T>::init_ = true;
    }
  }

  optional(optional&& rhs) noexcept(is_nothrow_move_constructible<T>::value)
  : OptionalBase<T>()
  {
    if (rhs.initialized()) {
        ::new (static_cast<void*>(dataptr())) T(std::move(*rhs));
        OptionalBase<T>::init_ = true;
    }
  }

  constexpr optional(const T& v) : OptionalBase<T>(v) {}

  constexpr optional(T&& v) : OptionalBase<T>(constexpr_move(v)) {}

  template <class... Args>
  explicit constexpr optional(in_place_t, Args&&... args)
  : OptionalBase<T>(in_place_t{}, constexpr_forward<Args>(args)...) {}

  template <class U, class... Args, TR2_OPTIONAL_REQUIRES(is_constructible<T, std::initializer_list<U>>)>
  OPTIONAL_CONSTEXPR_INIT_LIST explicit optional(in_place_t, std::initializer_list<U> il, Args&&... args)
  : OptionalBase<T>(in_place_t{}, il, constexpr_forward<Args>(args)...) {}

  ~optional() = default;

  optional& operator=(nullopt_t) noexcept
  {
    clear();
    return *this;
  }
  
  optional& operator=(const optional& rhs)
  {
    if      (initialized() == true  && rhs.initialized() == false) clear();
    else if (initialized() == false && rhs.initialized() == true)  initialize(*rhs);
    else if (initialized() == true  && rhs.initialized() == true)  contained_val() = *rhs;
    return *this;
  }
  
  optional& operator=(optional&& rhs)
  noexcept(is_nothrow_move_assignable<T>::value && is_nothrow_move_constructible<T>::value)
  {
    if      (initialized() == true  && rhs.initialized() == false) clear();
    else if (initialized() == false && rhs.initialized() == true)  initialize(std::move(*rhs));
    else if (initialized() == true  && rhs.initialized() == true)  contained_val() = std::move(*rhs);
    return *this;
  }

  template <class U>
  auto operator=(U&& v)
  -> typename enable_if
  <
    is_same<typename decay<U>::type, T>::value,
    optional&
  >::type
  {
    if (initialized()) { contained_val() = std::forward<U>(v); }
    else               { initialize(std::forward<U>(v));  }
    return *this;
  }
  
  
  template <class... Args>
  void emplace(Args&&... args)
  {
    clear();
    initialize(std::forward<Args>(args)...);
  }
  
  template <class U, class... Args>
  void emplace(initializer_list<U> il, Args&&... args)
  {
    clear();
    initialize<U, Args...>(il, std::forward<Args>(args)...);
  }
  
  void swap(optional<T>& rhs) noexcept(is_nothrow_move_constructible<T>::value && noexcept(swap(declval<T&>(), declval<T&>())))
  {
    if      (initialized() == true  && rhs.initialized() == false) { rhs.initialize(std::move(**this)); clear(); }
    else if (initialized() == false && rhs.initialized() == true)  { initialize(std::move(*rhs)); rhs.clear(); }
    else if (initialized() == true  && rhs.initialized() == true)  { using std::swap; swap(**this, *rhs); }
  }

  
  explicit constexpr operator bool() const noexcept { return initialized(); }
  
  constexpr T const* operator ->() const {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), dataptr());
  }
  
# if OPTIONAL_HAS_MOVE_ACCESSORS == 1

  OPTIONAL_MUTABLE_CONSTEXPR T* operator ->() {
    assert (initialized());
    return dataptr();
  }
  
  constexpr T const& operator *() const& {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), contained_val());
  }
  
  OPTIONAL_MUTABLE_CONSTEXPR T& operator *() & {
    assert (initialized());
    return contained_val();
  }
  
  OPTIONAL_MUTABLE_CONSTEXPR T&& operator *() && {
    assert (initialized());
    return constexpr_move(contained_val());
  }

  constexpr T const& value() const& {
    return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
  }
  
  OPTIONAL_MUTABLE_CONSTEXPR T& value() & {
    return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
  }
  
  OPTIONAL_MUTABLE_CONSTEXPR T&& value() && {
    if (!initialized()) throw bad_optional_access("bad optional access");
	return std::move(contained_val());
  }
  
# else

  T* operator ->() {
    assert (initialized());
    return dataptr();
  }
  
  constexpr T const& operator *() const {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), contained_val());
  }
  
  T& operator *() {
    assert (initialized());
    return contained_val();
  }
  
  constexpr T const& value() const {
    return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
  }
  
  T& value() {
    return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
  }
  
# endif
  
# if OPTIONAL_HAS_THIS_RVALUE_REFS == 1

  template <class V>
  constexpr T value_or(V&& v) const&
  {
    return *this ? **this : detail_::convert<T>(constexpr_forward<V>(v));
  }
  
#   if OPTIONAL_HAS_MOVE_ACCESSORS == 1

  template <class V>
  OPTIONAL_MUTABLE_CONSTEXPR T value_or(V&& v) &&
  {
    return *this ? constexpr_move(const_cast<optional<T>&>(*this).contained_val()) : detail_::convert<T>(constexpr_forward<V>(v));
  }

#   else
 
  template <class V>
  T value_or(V&& v) &&
  {
    return *this ? constexpr_move(const_cast<optional<T>&>(*this).contained_val()) : detail_::convert<T>(constexpr_forward<V>(v));
  }
  
#   endif
  
# else
  
  template <class V>
  constexpr T value_or(V&& v) const
  {
    return *this ? **this : detail_::convert<T>(constexpr_forward<V>(v));
  }

# endif

};


template <class T>
class optional<T&>
{
  static_assert( !std::is_same<T, nullopt_t>::value, "bad T" );
  static_assert( !std::is_same<T, in_place_t>::value, "bad T" );
  T* ref;
  
public:

  constexpr optional() noexcept : ref(nullptr) {}
  
  constexpr optional(nullopt_t) noexcept : ref(nullptr) {}
   
  constexpr optional(T& v) noexcept : ref(detail_::static_addressof(v)) {}
  
  optional(T&&) = delete;
  
  constexpr optional(const optional& rhs) noexcept : ref(rhs.ref) {}
  
  explicit constexpr optional(in_place_t, T& v) noexcept : ref(detail_::static_addressof(v)) {}
  
  explicit optional(in_place_t, T&&) = delete;
  
  ~optional() = default;
  
  optional& operator=(nullopt_t) noexcept {
    ref = nullptr;
    return *this;
  }
  
  
  
  template <typename U>
  auto operator=(U&& rhs) noexcept
  -> typename enable_if
  <
    is_same<typename decay<U>::type, optional<T&>>::value,
    optional&
  >::type
  {
    ref = rhs.ref;
    return *this;
  }
  
  template <typename U>
  auto operator=(U&& rhs) noexcept
  -> typename enable_if
  <
    !is_same<typename decay<U>::type, optional<T&>>::value,
    optional&
  >::type
  = delete;
  
  void emplace(T& v) noexcept {
    ref = detail_::static_addressof(v);
  }
  
  void emplace(T&&) = delete;
  
  
  void swap(optional<T&>& rhs) noexcept
  {
    std::swap(ref, rhs.ref);
  }
    
  constexpr T* operator->() const {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(ref, ref);
  }
  
  constexpr T& operator*() const {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(ref, *ref);
  }
  
  constexpr T& value() const {
    return ref ? *ref : (throw bad_optional_access("bad optional access"), *ref);
  }
  
  explicit constexpr operator bool() const noexcept {
    return ref != nullptr;
  }
  
  template <class V>
  constexpr typename decay<T>::type value_or(V&& v) const
  {
    return *this ? **this : detail_::convert<typename decay<T>::type>(constexpr_forward<V>(v));
  }
};


template <class T>
class optional<T&&>
{
  static_assert( sizeof(T) == 0, "optional rvalue references disallowed" );
};


template <class T> constexpr bool operator==(const optional<T>& x, const optional<T>& y)
{
  return bool(x) != bool(y) ? false : bool(x) == false ? true : *x == *y;
}

template <class T> constexpr bool operator!=(const optional<T>& x, const optional<T>& y)
{
  return !(x == y);
}

template <class T> constexpr bool operator<(const optional<T>& x, const optional<T>& y)
{
  return (!y) ? false : (!x) ? true : *x < *y;
}

template <class T> constexpr bool operator>(const optional<T>& x, const optional<T>& y)
{
  return (y < x);
}

template <class T> constexpr bool operator<=(const optional<T>& x, const optional<T>& y)
{
  return !(y < x);
}

template <class T> constexpr bool operator>=(const optional<T>& x, const optional<T>& y)
{
  return !(x < y);
}


template <class T> constexpr bool operator==(const optional<T>& x, nullopt_t) noexcept
{
  return (!x);
}

template <class T> constexpr bool operator==(nullopt_t, const optional<T>& x) noexcept
{
  return (!x);
}

template <class T> constexpr bool operator!=(const optional<T>& x, nullopt_t) noexcept
{
  return bool(x);
}

template <class T> constexpr bool operator!=(nullopt_t, const optional<T>& x) noexcept
{
  return bool(x);
}

template <class T> constexpr bool operator<(const optional<T>&, nullopt_t) noexcept
{
  return false;
}

template <class T> constexpr bool operator<(nullopt_t, const optional<T>& x) noexcept
{
  return bool(x);
}

template <class T> constexpr bool operator<=(const optional<T>& x, nullopt_t) noexcept
{
  return (!x);
}

template <class T> constexpr bool operator<=(nullopt_t, const optional<T>&) noexcept
{
  return true;
}

template <class T> constexpr bool operator>(const optional<T>& x, nullopt_t) noexcept
{
  return bool(x);
}

template <class T> constexpr bool operator>(nullopt_t, const optional<T>&) noexcept
{
  return false;
}

template <class T> constexpr bool operator>=(const optional<T>&, nullopt_t) noexcept
{
  return true;
}

template <class T> constexpr bool operator>=(nullopt_t, const optional<T>& x) noexcept
{
  return (!x);
}



template <class T> constexpr bool operator==(const optional<T>& x, const T& v)
{
  return bool(x) ? *x == v : false;
}

template <class T> constexpr bool operator==(const T& v, const optional<T>& x)
{
  return bool(x) ? v == *x : false;
}

template <class T> constexpr bool operator!=(const optional<T>& x, const T& v)
{
  return bool(x) ? *x != v : true;
}

template <class T> constexpr bool operator!=(const T& v, const optional<T>& x)
{
  return bool(x) ? v != *x : true;
}

template <class T> constexpr bool operator<(const optional<T>& x, const T& v)
{
  return bool(x) ? *x < v : true;
}

template <class T> constexpr bool operator>(const T& v, const optional<T>& x)
{
  return bool(x) ? v > *x : true;
}

template <class T> constexpr bool operator>(const optional<T>& x, const T& v)
{
  return bool(x) ? *x > v : false;
}

template <class T> constexpr bool operator<(const T& v, const optional<T>& x)
{
  return bool(x) ? v < *x : false;
}

template <class T> constexpr bool operator>=(const optional<T>& x, const T& v)
{
  return bool(x) ? *x >= v : false;
}

template <class T> constexpr bool operator<=(const T& v, const optional<T>& x)
{
  return bool(x) ? v <= *x : false;
}

template <class T> constexpr bool operator<=(const optional<T>& x, const T& v)
{
  return bool(x) ? *x <= v : true;
}

template <class T> constexpr bool operator>=(const T& v, const optional<T>& x)
{
  return bool(x) ? v >= *x : true;
}


template <class T> constexpr bool operator==(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x == v : false;
}

template <class T> constexpr bool operator==(const T& v, const optional<T&>& x)
{
  return bool(x) ? v == *x : false;
}

template <class T> constexpr bool operator!=(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x != v : true;
}

template <class T> constexpr bool operator!=(const T& v, const optional<T&>& x)
{
  return bool(x) ? v != *x : true;
}

template <class T> constexpr bool operator<(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x < v : true;
}

template <class T> constexpr bool operator>(const T& v, const optional<T&>& x)
{
  return bool(x) ? v > *x : true;
}

template <class T> constexpr bool operator>(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x > v : false;
}

template <class T> constexpr bool operator<(const T& v, const optional<T&>& x)
{
  return bool(x) ? v < *x : false;
}

template <class T> constexpr bool operator>=(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x >= v : false;
}

template <class T> constexpr bool operator<=(const T& v, const optional<T&>& x)
{
  return bool(x) ? v <= *x : false;
}

template <class T> constexpr bool operator<=(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x <= v : true;
}

template <class T> constexpr bool operator>=(const T& v, const optional<T&>& x)
{
  return bool(x) ? v >= *x : true;
}

template <class T> constexpr bool operator==(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x == v : false;
}

template <class T> constexpr bool operator==(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v == *x : false;
}

template <class T> constexpr bool operator!=(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x != v : true;
}

template <class T> constexpr bool operator!=(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v != *x : true;
}

template <class T> constexpr bool operator<(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x < v : true;
}

template <class T> constexpr bool operator>(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v > *x : true;
}

template <class T> constexpr bool operator>(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x > v : false;
}

template <class T> constexpr bool operator<(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v < *x : false;
}

template <class T> constexpr bool operator>=(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x >= v : false;
}

template <class T> constexpr bool operator<=(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v <= *x : false;
}

template <class T> constexpr bool operator<=(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x <= v : true;
}

template <class T> constexpr bool operator>=(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v >= *x : true;
}


template <class T>
void swap(optional<T>& x, optional<T>& y) noexcept(noexcept(x.swap(y)))
{
  x.swap(y);
}


template <class T>
constexpr optional<typename decay<T>::type> make_optional(T&& v)
{
  return optional<typename decay<T>::type>(constexpr_forward<T>(v));
}

template <class X>
constexpr optional<X&> make_optional(reference_wrapper<X> v)
{
  return optional<X&>(v.get());
}


} // namespace experimental
} // namespace std

namespace std
{
  template <typename T>
  struct hash<std::experimental::optional<T>>
  {
    typedef typename hash<T>::result_type result_type;
    typedef std::experimental::optional<T> argument_type;
    
    constexpr result_type operator()(argument_type const& arg) const {
      return arg ? std::hash<T>{}(*arg) : result_type{};
    }
  };
  
  template <typename T>
  struct hash<std::experimental::optional<T&>>
  {
    typedef typename hash<T>::result_type result_type;
    typedef std::experimental::optional<T&> argument_type;
    
    constexpr result_type operator()(argument_type const& arg) const {
      return arg ? std::hash<T>{}(*arg) : result_type{};
    }
  };
}

# undef TR2_OPTIONAL_REQUIRES
# undef TR2_OPTIONAL_ASSERTED_EXPRESSION

# endif //___OPTIONAL_HPP___
#line 7 "/mnt/c/Users/Kate/GitHub/cppspec/include/let.hpp"

namespace CppSpec {

class LetBase {
 protected:
  bool delivered;

 public:
  constexpr LetBase() noexcept : delivered(false) {}
  LetBase(const LetBase &copy) = default;
  void reset() noexcept { delivered = false; }
  constexpr const bool has_result() noexcept { return this->delivered; }
};

template <typename T>
class Let : public LetBase {
  typedef std::function<T()> block_t;
  std::experimental::optional<T> result;

  block_t body;

  void exec();

 public:
  explicit Let(block_t body) noexcept : LetBase(), body(body) {}

  T *operator->() {
    value();
    return result.operator->();
  }

  T &operator*() & { return value(); }
  T &value()&;
};

template <typename T>
void Let<T>::exec() {
  if (!delivered) {
    result = body();
    delivered = true;
  }
}

template <typename T>
T &Let<T>::value() & {
  exec();
  return result.value();
}

}  // namespace CppSpec
#endif  // CPPSPEC_LET_HPP
#line 9 "/mnt/c/Users/Kate/GitHub/cppspec/include/it_base.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/util.hpp"
#ifndef CPPSPEC_UTIL_HPP
#define CPPSPEC_UTIL_HPP
#pragma once

#include <string>
#include <sstream>
#include <functional>

#ifdef __GNUG__
#include <cxxabi.h>
#include <memory>
#endif

namespace CppSpec {
namespace Util {

#ifdef __GNUG__
inline std::string demangle(const char *mangled) {
  int status;
  std::unique_ptr<char[], void (*)(void *)> result{
      abi::__cxa_demangle(mangled, NULL, NULL, &status), std::free};

  return (status == 0) ? result.get() : mangled;
}
#else
inline std::string demangle(const char *name) { return name; }
#endif

template <class A>
struct verbose_assert {
  static_assert(A::value, "Assertion failed (see below for more information)");
  static bool const value = A::value;
};

template <typename T>
struct is_true : public std::is_same<std::true_type, T> {};

template <typename T>
struct is_false : public std::is_same<std::false_type, T> {};

namespace is_iterable_imp {
template <class C>
auto test(void *) -> decltype(std::declval<C>().begin(),
                              std::declval<C>().end(), std::true_type{});

template <class C>
auto test(int) -> decltype(std::declval<C>().cbegin(), std::declval<C>().cend(),
                           std::true_type{});

template <class>
auto test(...) -> std::false_type;
}  // namespace is_iterable_imp

template <class T>
class is_iterable : public decltype(is_iterable_imp::test<T>(0)) {};

#ifdef HAS_VARIABLE_TEMPLATES
template <class T>
constexpr bool is_iterable_v = is_iterable<T>::value;
#endif

namespace is_container_imp {
template <class C>
auto test(void *) -> decltype(std::declval<C>().max_size(),
                              std::declval<C>().empty(), is_iterable<C>{});

template <class>
auto test(...) -> std::false_type;
}  // namespace is_container_imp

template <class T>
struct is_container : public decltype(is_container_imp::test<T>(0)) {};

#ifdef HAS_VARIABLE_TEMPLATES
template <class T>
constexpr bool is_container_v = is_container<T>::value;
#endif

namespace is_streamable_imp {
template <typename C>
auto test(void *) -> std::is_same<
    decltype(std::declval<std::ostream>().operator<<(std::declval<C>())),
    std::ostream &>;

template <class C>
auto test(int) -> std::is_same<
    decltype(operator<<(std::declval<std::ostream &>(),
                        std::declval<C const &>())),

    std::ostream &  // make sure the return type is ostream&
    >;

template <class>
auto test(...) -> std::false_type;  // fallthrough
}  // namespace is_streamable_imp

template <class T>
struct is_streamable : public decltype(is_streamable_imp::test<T>(0)) {};

#ifdef HAS_VARIABLE_TEMPLATES
template <class T>
constexpr bool is_streamable_v = is_streamable<T>::value;
#endif

namespace is_functional_imp {

template <typename C>
auto test(void *) -> std::is_function<C>;


template <class C>
auto test(int) -> std::is_convertible<C, std::function<void()>>;

template <class C>
auto test(...) -> std::false_type;  // fallthrough
}  // namespace is_functional_imp

template <class T>
struct is_functional : public decltype(is_functional_imp::test<T>(0)) {};

#ifdef HAS_VARIABLE_TEMPLATES
template <class T>
constexpr bool is_functional_v = is_functional<T>::value;
#endif

template <typename Range>
inline std::string join(Range &iterable, const std::string &separator = "") {
  std::ostringstream oss;
  typename Range::const_iterator it;
  for (it = iterable.cbegin(); it != iterable.cend();) {
    oss << *it;  // use operator<< to convert and append

    if (++it != iterable.cend()) oss << separator;
  }
  return oss.str();
}

}  // namespace Util
}  // namespace CppSpec
#endif  // CPPSPEC_UTIL_HPP
#line 10 "/mnt/c/Users/Kate/GitHub/cppspec/include/it_base.hpp"

namespace CppSpec {

template <typename T>
class ExpectationValue;
template <typename T>
class ExpectationFunc;

class ItBase : public Runnable {
  std::string description = "";

 public:
  ItBase() = delete;  // Don't allow a default constructor

  ItBase(const ItBase &copy) noexcept : Runnable(copy),
                                        description(copy.description) {}

  explicit ItBase(const Child &parent) noexcept : Runnable(parent) {}

  explicit ItBase(const Child &parent, std::string description) noexcept
      : Runnable(parent),
        description(description) {}

  const bool needs_description() noexcept { return description.empty(); }

  std::string get_description() noexcept { return description; }
  const std::string get_description() const noexcept { return description; }

  ItBase &set_description(std::string description) noexcept {
    this->description = description;
    return *this;
  }

  template <typename T>
  typename std::enable_if<not Util::is_functional<T>::value,
                          ExpectationValue<T>>::type
  expect(T value);

  template <typename T>
  typename std::enable_if<Util::is_functional<T>::value,
                          ExpectationFunc<T>>::type
  expect(T block);

  template <typename T>
  ExpectationValue<std::initializer_list<T>> expect(
      std::initializer_list<T> init_list);

  template <typename T>
  ExpectationValue<T> expect(Let<T> &let);

  ExpectationValue<std::string> expect(const char *string);
};

}  // namespace CppSpec
#endif  // CPPSPEC_IT_BASE_HPP
#line 8 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/matcher_base.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/pretty_matchers.hpp"
#ifndef CPPSPEC_MATCHERS_PRETTY_MATCHERS_HPP
#define CPPSPEC_MATCHERS_PRETTY_MATCHERS_HPP
#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/cxx-prettyprint/prettyprint.hpp"

#ifndef H_PRETTY_PRINT
#define H_PRETTY_PRINT

#include <cstddef>
#include <iterator>
#include <memory>
#include <ostream>
#include <set>
#include <tuple>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <sstream>

namespace pretty_print
{
    namespace detail
    {

        struct sfinae_base
        {
            using yes = char;
            using no  = yes[2];
        };

        template <typename T>
        struct has_const_iterator : private sfinae_base
        {
        private:
            template <typename C> static yes & test(typename C::const_iterator*);
            template <typename C> static no  & test(...);
        public:
            static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
            using type =  T;
        };

        template <typename T>
        struct has_begin_end : private sfinae_base
        {
        private:
            template <typename C>
            static yes & f(typename std::enable_if<
                std::is_same<decltype(static_cast<typename C::const_iterator(C::*)() const>(&C::begin)),
                             typename C::const_iterator(C::*)() const>::value>::type *);

            template <typename C> static no & f(...);

            template <typename C>
            static yes & g(typename std::enable_if<
                std::is_same<decltype(static_cast<typename C::const_iterator(C::*)() const>(&C::end)),
                             typename C::const_iterator(C::*)() const>::value, void>::type*);

            template <typename C> static no & g(...);

        public:
            static bool const beg_value = sizeof(f<T>(nullptr)) == sizeof(yes);
            static bool const end_value = sizeof(g<T>(nullptr)) == sizeof(yes);
        };

    }  // namespace detail



    template <typename TChar>
    struct delimiters_values
    {
        using char_type = TChar;
        const char_type * prefix;
        const char_type * delimiter;
        const char_type * postfix;
    };



    template <typename T, typename TChar>
    struct delimiters
    {
        using type = delimiters_values<TChar>;
        static const type values; 
    };

    template <typename T>
    struct quoting_handler
    {
        static std::string toString(const T& x)
        {
            std::ostringstream out;
            out << x;
            return out.str();
        }
    };

    template <>
    struct quoting_handler<std::string>
    {
        static std::string toString(const std::string& x)
        {
            return '\"' + x + '\"';
        }
    };

    template <class T>
    std::string quote_if_necessary(const T& x)
    {
        return quoting_handler<T>::toString(x);
    }


    template <typename T,
              typename TChar = char,
              typename TCharTraits = ::std::char_traits<TChar>,
              typename TDelimiters = delimiters<T, TChar>>
    struct print_container_helper
    {
        using delimiters_type = TDelimiters;
        using ostream_type = std::basic_ostream<TChar, TCharTraits>;

        template <typename U>
        struct printer
        {
            static void print_body(const U & c, ostream_type & stream)
            {
                using std::begin;
                using std::end;

                auto it = begin(c);
                const auto the_end = end(c);

                if (it != the_end)
                {
                    for ( ; ; )
                    {
		        stream << quote_if_necessary(*it);

                        if (++it == the_end)
                            break;

                        if (delimiters_type::values.delimiter != NULL)
                            stream << delimiters_type::values.delimiter;
                    }
                }
            }
        };

        print_container_helper(const T & container)
        : container_(container)
        { }

        inline void operator()(ostream_type & stream) const
        {
            if (delimiters_type::values.prefix != NULL)
                stream << delimiters_type::values.prefix;

            printer<T>::print_body(container_, stream);

            if (delimiters_type::values.postfix != NULL)
                stream << delimiters_type::values.postfix;
        }

    private:
        const T & container_;
    };


    template <typename T, typename TChar, typename TCharTraits, typename TDelimiters>
    template <typename T1, typename T2>
    struct print_container_helper<T, TChar, TCharTraits, TDelimiters>::printer<std::pair<T1, T2>>
    {
        using ostream_type = typename print_container_helper<T, TChar, TCharTraits, TDelimiters>::ostream_type;

        static void print_body(const std::pair<T1, T2> & c, ostream_type & stream)
        {
            stream << quote_if_necessary(c.first);
            if (print_container_helper<T, TChar, TCharTraits, TDelimiters>::delimiters_type::values.delimiter != NULL)
                stream << print_container_helper<T, TChar, TCharTraits, TDelimiters>::delimiters_type::values.delimiter;
            stream << quote_if_necessary(c.second);
        }
    };


    template <typename T, typename TChar, typename TCharTraits, typename TDelimiters>
    template <typename ...Args>
    struct print_container_helper<T, TChar, TCharTraits, TDelimiters>::printer<std::tuple<Args...>>
    {
        using ostream_type = typename print_container_helper<T, TChar, TCharTraits, TDelimiters>::ostream_type;
        using element_type = std::tuple<Args...>;

        template <std::size_t I> struct Int { };

        static void print_body(const element_type & c, ostream_type & stream)
        {
            tuple_print(c, stream, Int<0>());
        }

        static void tuple_print(const element_type &, ostream_type &, Int<sizeof...(Args)>)
        {
        }

        static void tuple_print(const element_type & c, ostream_type & stream,
                                typename std::conditional<sizeof...(Args) != 0, Int<0>, std::nullptr_t>::type)
        {
            stream << quote_if_necessary(std::get<0>(c));
            tuple_print(c, stream, Int<1>());
        }

        template <std::size_t N>
        static void tuple_print(const element_type & c, ostream_type & stream, Int<N>)
        {
            if (print_container_helper<T, TChar, TCharTraits, TDelimiters>::delimiters_type::values.delimiter != NULL)
                stream << print_container_helper<T, TChar, TCharTraits, TDelimiters>::delimiters_type::values.delimiter;

            stream << quote_if_necessary(std::get<N>(c));

            tuple_print(c, stream, Int<N + 1>());
        }
    };


    template<typename T, typename TChar, typename TCharTraits, typename TDelimiters>
    inline std::basic_ostream<TChar, TCharTraits> & operator<<(
        std::basic_ostream<TChar, TCharTraits> & stream,
        const print_container_helper<T, TChar, TCharTraits, TDelimiters> & helper)
    {
        helper(stream);
        return stream;
    }



    template <typename T>
    struct is_container : public std::integral_constant<bool,
                                                        detail::has_const_iterator<T>::value &&
                                                        detail::has_begin_end<T>::beg_value  &&
                                                        detail::has_begin_end<T>::end_value> { };

    template <typename T, std::size_t N>
    struct is_container<T[N]> : std::true_type { };

    template <std::size_t N>
    struct is_container<char[N]> : std::false_type { };

    template <typename T>
    struct is_container<std::valarray<T>> : std::true_type { };

    template <typename T1, typename T2>
    struct is_container<std::pair<T1, T2>> : std::true_type { };

    template <typename ...Args>
    struct is_container<std::tuple<Args...>> : std::true_type { };



    template <typename T> struct delimiters<T, char> { static const delimiters_values<char> values; };
    template <typename T> const delimiters_values<char> delimiters<T, char>::values = { "[", ", ", "]" };
    template <typename T> struct delimiters<T, wchar_t> { static const delimiters_values<wchar_t> values; };
    template <typename T> const delimiters_values<wchar_t> delimiters<T, wchar_t>::values = { L"[", L", ", L"]" };



    template <typename T, typename TComp, typename TAllocator>
    struct delimiters< ::std::set<T, TComp, TAllocator>, char> { static const delimiters_values<char> values; };

    template <typename T, typename TComp, typename TAllocator>
    const delimiters_values<char> delimiters< ::std::set<T, TComp, TAllocator>, char>::values = { "{", ", ", "}" };

    template <typename T, typename TComp, typename TAllocator>
    struct delimiters< ::std::set<T, TComp, TAllocator>, wchar_t> { static const delimiters_values<wchar_t> values; };

    template <typename T, typename TComp, typename TAllocator>
    const delimiters_values<wchar_t> delimiters< ::std::set<T, TComp, TAllocator>, wchar_t>::values = { L"{", L", ", L"}" };

    template <typename T, typename TComp, typename TAllocator>
    struct delimiters< ::std::multiset<T, TComp, TAllocator>, char> { static const delimiters_values<char> values; };

    template <typename T, typename TComp, typename TAllocator>
    const delimiters_values<char> delimiters< ::std::multiset<T, TComp, TAllocator>, char>::values = { "{", ", ", "}" };

    template <typename T, typename TComp, typename TAllocator>
    struct delimiters< ::std::multiset<T, TComp, TAllocator>, wchar_t> { static const delimiters_values<wchar_t> values; };

    template <typename T, typename TComp, typename TAllocator>
    const delimiters_values<wchar_t> delimiters< ::std::multiset<T, TComp, TAllocator>, wchar_t>::values = { L"{", L", ", L"}" };

    template <typename T, typename THash, typename TEqual, typename TAllocator>
    struct delimiters< ::std::unordered_set<T, THash, TEqual, TAllocator>, char> { static const delimiters_values<char> values; };

    template <typename T, typename THash, typename TEqual, typename TAllocator>
    const delimiters_values<char> delimiters< ::std::unordered_set<T, THash, TEqual, TAllocator>, char>::values = { "{", ", ", "}" };

    template <typename T, typename THash, typename TEqual, typename TAllocator>
    struct delimiters< ::std::unordered_set<T, THash, TEqual, TAllocator>, wchar_t> { static const delimiters_values<wchar_t> values; };

    template <typename T, typename THash, typename TEqual, typename TAllocator>
    const delimiters_values<wchar_t> delimiters< ::std::unordered_set<T, THash, TEqual, TAllocator>, wchar_t>::values = { L"{", L", ", L"}" };

    template <typename T, typename THash, typename TEqual, typename TAllocator>
    struct delimiters< ::std::unordered_multiset<T, THash, TEqual, TAllocator>, char> { static const delimiters_values<char> values; };

    template <typename T, typename THash, typename TEqual, typename TAllocator>
    const delimiters_values<char> delimiters< ::std::unordered_multiset<T, THash, TEqual, TAllocator>, char>::values = { "{", ", ", "}" };

    template <typename T, typename THash, typename TEqual, typename TAllocator>
    struct delimiters< ::std::unordered_multiset<T, THash, TEqual, TAllocator>, wchar_t> { static const delimiters_values<wchar_t> values; };

    template <typename T, typename THash, typename TEqual, typename TAllocator>
    const delimiters_values<wchar_t> delimiters< ::std::unordered_multiset<T, THash, TEqual, TAllocator>, wchar_t>::values = { L"{", L", ", L"}" };



    template <typename T1, typename T2> struct delimiters<std::pair<T1, T2>, char> { static const delimiters_values<char> values; };
    template <typename T1, typename T2> const delimiters_values<char> delimiters<std::pair<T1, T2>, char>::values = { "(", ", ", ")" };
    template <typename T1, typename T2> struct delimiters< ::std::pair<T1, T2>, wchar_t> { static const delimiters_values<wchar_t> values; };
    template <typename T1, typename T2> const delimiters_values<wchar_t> delimiters< ::std::pair<T1, T2>, wchar_t>::values = { L"(", L", ", L")" };

    template <typename ...Args> struct delimiters<std::tuple<Args...>, char> { static const delimiters_values<char> values; };
    template <typename ...Args> const delimiters_values<char> delimiters<std::tuple<Args...>, char>::values = { "(", ", ", ")" };
    template <typename ...Args> struct delimiters< ::std::tuple<Args...>, wchar_t> { static const delimiters_values<wchar_t> values; };
    template <typename ...Args> const delimiters_values<wchar_t> delimiters< ::std::tuple<Args...>, wchar_t>::values = { L"(", L", ", L")" };



    struct custom_delims_base
    {
        virtual ~custom_delims_base() { }
        virtual std::ostream & stream(::std::ostream &) = 0;
        virtual std::wostream & stream(::std::wostream &) = 0;
    };

    template <typename T, typename Delims>
    struct custom_delims_wrapper : custom_delims_base
    {
        custom_delims_wrapper(const T & t_) : t(t_) { }

        std::ostream & stream(std::ostream & s)
        {
            return s << print_container_helper<T, char, std::char_traits<char>, Delims>(t);
        }

        std::wostream & stream(std::wostream & s)
        {
            return s << print_container_helper<T, wchar_t, std::char_traits<wchar_t>, Delims>(t);
        }

    private:
        const T & t;
    };

    template <typename Delims>
    struct custom_delims
    {
        template <typename Container>
        custom_delims(const Container & c) : base(new custom_delims_wrapper<Container, Delims>(c)) { }

        std::unique_ptr<custom_delims_base> base;
    };

    template <typename TChar, typename TCharTraits, typename Delims>
    inline std::basic_ostream<TChar, TCharTraits> & operator<<(std::basic_ostream<TChar, TCharTraits> & s, const custom_delims<Delims> & p)
    {
        return p.base->stream(s);
    }



    template<typename T>
    struct array_wrapper_n
    {
        typedef const T * const_iterator;
        typedef T value_type;

        array_wrapper_n(const T * const a, size_t n) : _array(a), _n(n) { }
        inline const_iterator begin() const { return _array; }
        inline const_iterator end() const { return _array + _n; }

    private:
        const T * const _array;
        size_t _n;
    };



    template <typename T>
    struct bucket_print_wrapper
    {
        typedef typename T::const_local_iterator const_iterator;
        typedef typename T::size_type size_type;

        const_iterator begin() const
        {
            return m_map.cbegin(n);
        }

        const_iterator end() const
        {
            return m_map.cend(n);
        }

        bucket_print_wrapper(const T & m, size_type bucket) : m_map(m), n(bucket) { }

    private:
        const T & m_map;
        const size_type n;
    };

}   // namespace pretty_print



template<typename T>
inline pretty_print::array_wrapper_n<T> pretty_print_array(const T * const a, size_t n)
{
    return pretty_print::array_wrapper_n<T>(a, n);
}

template <typename T> pretty_print::bucket_print_wrapper<T>
bucket_print(const T & m, typename T::size_type n)
{
    return pretty_print::bucket_print_wrapper<T>(m, n);
}



namespace std
{

    template<typename T, typename TChar, typename TCharTraits>
    inline typename enable_if< ::pretty_print::is_container<T>::value,
                              basic_ostream<TChar, TCharTraits> &>::type
    operator<<(basic_ostream<TChar, TCharTraits> & stream, const T & container)
    {
        return stream << ::pretty_print::print_container_helper<T, TChar, TCharTraits>(container);
    }
}



#endif  // H_PRETTY_PRINT
#line 10 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/pretty_matchers.hpp"
#line 11 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/pretty_matchers.hpp"

namespace CppSpec {

namespace Matchers {
template <typename A, typename E>
class MatcherBase;
}

struct Pretty {
  std::string _name = "";
  std::string name(std::string name);
  std::string name_to_sentance(std::string name);
  static std::string split_words(std::string sym);
  static std::string underscore(std::string camel_cased_word);
  static std::string last(const std::string &s, const char delim);
  static std::string improve_hash_formatting(std::string inspect_string);

  template <typename T>
  static
      typename std::enable_if<Util::is_streamable<T>::value, std::string>::type
      to_word(T &item);

  template <typename T>
  static
      typename std::enable_if<!Util::is_streamable<T>::value, std::string>::type
      to_word(T &item);

  template <typename T>
  static std::string to_word_type(T &item);

  template <typename A, typename E>
  static std::string to_word(Matchers::MatcherBase<A, E> &matcher);

  template <class T>
  static std::string to_sentance(T &item);

  template <class T>
  static std::string to_sentance(std::vector<T> &words);

  template <typename T>
  static std::string inspect_object(T &object);
};

template <typename T>
inline std::string Pretty::to_sentance(std::vector<T> &words) {
  std::vector<std::string> my_words;
  for (T word : words) {
    my_words.push_back(to_word<T>(word));
  }
  std::stringstream ss;
  switch (my_words.size()) {
    case 0:
      return "";
    case 1:
      ss << " " << my_words[0];
      break;
    case 2:
      ss << " " << my_words[0] << " and " << my_words[1];
      break;
    default:
      ss << " ";
      for (size_t i = 0; i < my_words.size() - 1; ++i) {
        if (i != 0) ss << ", ";
        ss << my_words[i];
      }
      ss << ", and " << my_words.back();
      break;
  }

  return ss.str();
}

template <typename T>
inline std::string Pretty::to_sentance(T &item) {
  std::vector<T> v = {item};
  return to_sentance<T>(v);
}

template <typename T>
typename std::enable_if<Util::is_streamable<T>::value,
                        std::string>::type inline Pretty::to_word(T &item) {
  std::stringstream ss;
  ss << item;
  return ss.str();
}

template <>
inline std::string Pretty::to_word<bool>(bool &item) {
  return item ? "true" : "false";
}

template <>
inline std::string Pretty::to_word<std::true_type>(std::true_type &item) {
  return "true";
}

template <>
inline std::string Pretty::to_word<std::false_type>(std::false_type &item) {
  return "false";
}

template <typename T>
typename std::enable_if<!Util::is_streamable<T>::value,
                        std::string>::type inline Pretty::to_word(T &item) {
  std::stringstream ss;
  ss << "#<" << Util::demangle(typeid(item).name()) << ":" << &item << ">";
  return ss.str();
}

template <typename A, typename E>
inline std::string Pretty::to_word(Matchers::MatcherBase<A, E> &matcher) {
  std::string description = matcher.description();
  if (description.empty()) {
    return "[No description]";
  } else {
    return description;
  }
}

template <typename T>
inline std::string Pretty::to_word_type(T &item) {
  std::string word = to_word(item);
  if (Util::is_streamable<T>::value) {
    word += " : " + Util::demangle(typeid(T).name());
  }
  return word;
}

inline std::string Pretty::name_to_sentance(std::string n) {
  return split_words(name(n));
}

inline std::string Pretty::name(std::string name) {
  if (_name.empty()) {
    return last(name, ':');
  } else {
    return _name;
  }
}

inline std::string Pretty::split_words(std::string sym) {
  std::stringstream ss;
  ss << sym;
  sym = ss.str();
  std::regex_replace(sym, std::regex("_"), " ");
  return sym;
}

inline std::string Pretty::underscore(std::string word) {
  std::regex_replace(word, std::regex("([A-Z]+)([A-Z][a-z])"), "$1_$2");
  std::regex_replace(word, std::regex("([a-z\\d])([A-Z])"), "$1_$2");
  std::regex_replace(word, std::regex("-"), "_");
  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  return word;
}

inline std::string Pretty::last(const std::string &s, const char delim) {
  std::vector<std::string> elems;
  std::stringstream ss(s);
  std::string item;
  while (getline(ss, item, delim)) {
    if (!item.empty()) {
      elems.push_back(item);
    }
  }
  return elems.back();
}

inline std::string Pretty::improve_hash_formatting(std::string inspect_string) {
  std::regex_replace(inspect_string, std::regex("(\\S)=>(\\S)"), "$1 => $2");
  return inspect_string;
}


template <typename O>
inline std::string Pretty::inspect_object(O &o) {
  std::stringstream ss;
  ss << "(" << Util::demangle(typeid(o).name()) << ") =>" << to_sentance(o);
  return ss.str();
}

template <>
inline std::string Pretty::inspect_object<const char *>(const char *&o) {
  std::stringstream ss;
  ss << "(const char *) => " << '"' << o << '"';
  return ss.str();
}

template <>
inline std::string Pretty::inspect_object<std::string>(std::string &o) {
  std::stringstream ss;
  ss << "(std::string) => " << '"' << o << '"';
  return ss.str();
}

}  // namespace CppSpec
#endif  // CPPSPEC_MATCHERS_PRETTY_MATCHERS_HPP
#line 9 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/matcher_base.hpp"

namespace CppSpec {

template <class T>
class Expectation;  // Forward declaration of Expectation

namespace Matchers {

template <typename Actual, typename Expected>
class MatcherBase : public Runnable, public Pretty {
  std::string custom_failure_message = "";

 protected:
  Expected expected;  // The expected object contained by the matcher

  Expectation<Actual> &expectation;

 public:
  MatcherBase(MatcherBase<Actual, Expected> const &copy) = default;

  explicit MatcherBase(Expectation<Actual> &expectation)
      : Runnable(*expectation.get_parent()),  // We want the parent of the
        expectation(expectation) {}

  MatcherBase(Expectation<Actual> &expectation, Expected expected)
      : Runnable(*expectation.get_parent()),
        expected(expected),
        expectation(expectation) {}

  /*--------- Helper functions -------------*/

  virtual std::string failure_message();
  virtual std::string failure_message_when_negated();
  virtual std::string description();

  Actual &get_actual() { return expectation.get_target(); }

  Expected &get_expected() { return expected; }

  Expectation<Actual> &get_expectation() { return expectation; }

  virtual MatcherBase &set_message(std::string message);

  /*--------- Primary functions -------------*/

  Result run(Formatters::BaseFormatter &printer) override;

  virtual bool match() = 0;
  virtual bool negated_match() { return !match(); }

  typedef Expected expected_t;
};

template <typename A, typename E>
MatcherBase<A, E> &MatcherBase<A, E>::set_message(std::string message) {
  this->custom_failure_message = message;
  return *this;
}

template <typename A, typename E>
std::string MatcherBase<A, E>::failure_message() {
  if (not custom_failure_message.empty()) {
    return this->custom_failure_message;
  } else {
    std::stringstream ss;
    ss << "expected " << Pretty::to_word(get_actual()) << " to "
       << description();
    return ss.str();
  }
}

template <typename A, typename E>
std::string MatcherBase<A, E>::failure_message_when_negated() {
  if (not custom_failure_message.empty()) {
    return this->custom_failure_message;
  } else {
    std::stringstream ss;
    ss << "expected " << Pretty::to_word(get_actual()) << " to not "
       << description();
    return ss.str();
  }
}

template <typename A, typename E>
std::string MatcherBase<A, E>::description() {
  std::stringstream ss;
  std::string pretty_expected = this->to_sentance(expected);
  ss << "match" << Pretty::to_sentance(expected);
  return ss.str();
}

template <typename A, typename E>
Result MatcherBase<A, E>::run(Formatters::BaseFormatter &printer) {
  ItBase *par = static_cast<ItBase *>(this->get_parent());
  if (par->needs_description() && !expectation.get_ignore_failure()) {
    std::stringstream ss;
    ss << (expectation.get_sign() ? PositiveExpectationHandler::verb()
                                  : NegativeExpectationHandler::verb())
       << " " << this->description();
    std::string ss_str = ss.str();
    par->set_description(ss_str);
  }

  Result matched = expectation.get_sign()
                       ? PositiveExpectationHandler::handle_matcher(*this)
                       : NegativeExpectationHandler::handle_matcher(*this);

  if (!matched && !expectation.get_ignore_failure()) {
    this->failed();
    std::string message = matched.get_message();
    if (message.empty()) {
      printer.format_failure(
          "Failure message is empty. Does your matcher define the "
          "appropriate failure_message[_when_negated] method to "
          "return a string?");
    } else {
      printer.format_failure(matched.get_message());
    }
  }
  return matched;
}

}  // namespace Matchers
}  // namespace CppSpec
#endif  // CPPSPEC_MATCHERS_MATCHER_BASE_HPP
#line 6 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/be_between.hpp"

namespace CppSpec {
namespace Matchers {

enum class RangeMode { exclusive, inclusive };

template <typename A, typename E>
class BeBetween : public MatcherBase<A, E> {
  E min;
  E max;
  RangeMode mode;
  enum class LtOp { lt, lt_eq } lt_op;
  enum class GtOp { gt, gt_eq } gt_op;

 public:

  BeBetween(Expectation<A> &expectation, E min, E max,
            RangeMode mode = RangeMode::inclusive)
      : MatcherBase<A, E>(expectation), min(min), max(max), mode(mode) {
    switch (mode) {
      case RangeMode::inclusive:
        lt_op = LtOp::lt_eq;
        gt_op = GtOp::gt_eq;
        break;
      case RangeMode::exclusive:
        lt_op = LtOp::lt;
        gt_op = GtOp::gt;
        break;
    }
  }

  bool match() override;
  std::string description() override;
};

template <typename A, typename E>
bool BeBetween<A, E>::match() {
  auto actual = this->get_actual();
  bool result1;
  switch (gt_op) {
    case GtOp::gt:
      result1 = actual > min;
      break;
    case GtOp::gt_eq:
      result1 = actual >= min;
      break;
  }
  bool result2;
  switch (lt_op) {
    case LtOp::lt:
      result2 = actual < max;
      break;
    case LtOp::lt_eq:
      result2 = actual <= max;
      break;
  }
  return result1 && result2;
}

template <typename A, typename E>
std::string BeBetween<A, E>::description() {
  std::stringstream ss;
  ss << "be between " << min << " and " << max << " ("
     << (mode == RangeMode::exclusive ? "exclusive" : "inclusive") << ")";
  return ss.str();
}

}  // namespace Matchers
}  // namespace CppSpec
#endif // CPPSPEC_MATCHERS_BE_BETWEEN_HPP
#line 10 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/be_greater_than.hpp"
#ifndef CPPSPEC_MATCHERS_BE_GREATER_THAN_HPP
#define CPPSPEC_MATCHERS_BE_GREATER_THAN_HPP
#pragma once
#include <string>
#line 6 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/be_greater_than.hpp"
namespace CppSpec {
namespace Matchers {

template <typename A, typename E>
class BeGreaterThan : public MatcherBase<A, E> {
 public:
  BeGreaterThan(Expectation<A> &expectation, E expected)
      : MatcherBase<A, E>(expectation, expected) {}
  bool match() override { return this->get_actual() > this->get_expected(); }
  std::string description() override {
    return "be greater than" + Pretty::to_word(this->get_expected());
  }
};

}  // namespace Matchers
}  // namespace CppSpec

#endif  // CPPSPEC_MATCHERS_BE_GREATER_THAN_HPP
#line 11 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/be_less_than.hpp"
#ifndef CPPSPEC_MATCHERS_BE_LESS_THAN_HPP
#define CPPSPEC_MATCHERS_BE_LESS_THAN_HPP
#pragma once
#include <string>
#line 6 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/be_less_than.hpp"
namespace CppSpec {
namespace Matchers {

template <typename A, typename E>
class BeLessThan : public MatcherBase<A, E> {
 public:
  BeLessThan(Expectation<A> &expectation, E expected)
      : MatcherBase<A, E>(expectation, expected) {}
  bool match() override { return this->get_actual() < this->get_expected(); }
  std::string description() override {
    return "be less than" + Pretty::to_word(this->get_expected());
  }
};

}  // namespace Matchers
}  // namespace CppSpec

#endif  // CPPSPEC_MATCHERS_BE_LESS_THAN_HPP
#line 12 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/be_nullptr.hpp"
#ifndef CPPSPEC_MATCHERS_BE_NULLPTR_HPP
#define CPPSPEC_MATCHERS_BE_NULLPTR_HPP
#pragma once
#include <string>
#line 6 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/be_nullptr.hpp"
namespace CppSpec {
namespace Matchers {

template <class A>
class BeNullptr : MatcherBase<A, std::nullptr_t> {
 public:
  explicit BeNullptr(Expectation<A> &expectation)
      : MatcherBase<A, std::nullptr_t>(expectation) {}

  std::string description() override { return "be nullptr"; }
  bool match() override { return this->get_actual() == nullptr; }
};

}  // namespace Matchers
}  // namespace CppSpec

#endif  // CPPSPEC_MATCHERS_BE_NULLPTR_HPP
#line 13 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/be_within.hpp"
#ifndef CPPSPEC_MATCHERS_BE_WITHIN_HPP
#define CPPSPEC_MATCHERS_BE_WITHIN_HPP
#pragma once

#line 6 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/be_within.hpp"

namespace CppSpec {
namespace Matchers {

template <typename A, typename E>
class BeWithin : public MatcherBase<A, E> {
  E delta;
  std::string unit;
  E tolerance;

 public:
  BeWithin(Expectation<A> &expectation, E delta)
      : MatcherBase<A, E>(expectation, 0), delta(delta) {}

  bool of(E expected);
  bool percent_of(E expected);

  bool match() override;

  std::string failure_message() override;
  std::string failure_message_when_negated() override;
  std::string description() override;
};

template <typename A, typename E>
bool BeWithin<A, E>::of(E expected) {
  this->expected = expected;
  this->tolerance = this->delta;
  this->unit = "";
  return this->run(this->get_formatter());
}

template <typename A, typename E>
bool BeWithin<A, E>::percent_of(E expected) {
  this->expected = expected;
  this->tolerance = this->delta;
  this->unit = "%";
  return this->run(this->get_formatter());
}

template <typename A, typename E>
bool BeWithin<A, E>::match() {
  if (!this->expected) {
    std::stringstream ss;
    ss << "You must set an expected value using #of: be_within(" << this->delta
       << ").of(expected_value)";

    return false;
  }
  return std::abs(this->get_actual() - this->expected) <= this->tolerance;
}

template <typename A, typename E>
std::string BeWithin<A, E>::failure_message() {
  std::stringstream ss;
  ss << "expected " << this->get_actual() << " to " << description();
  return ss.str();
}

template <typename A, typename E>
std::string BeWithin<A, E>::failure_message_when_negated() {
  std::stringstream ss;
  ss << "expected " << this->get_actual() << " not to " << description();
  return ss.str();
}

template <typename A, typename E>
std::string BeWithin<A, E>::description() {
  std::stringstream ss;
  ss << "be within " << this->delta << this->unit << " of " << this->expected;
  return ss.str();
}

}  // namespace Matchers
}  // namespace CppSpec
#endif // CPPSPEC_MATCHERS_BE_WITHIN_HPP
#line 14 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/contain.hpp"
#ifndef CPPSPEC_MATCHERS_INCLUDE_HPP
#define CPPSPEC_MATCHERS_INCLUDE_HPP
#line 4 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/contain.hpp"

namespace CppSpec {
namespace Matchers {

template <typename A, typename E, typename U>
class ContainBase : public MatcherBase<A, E> {
  A actual;

 public:
  virtual std::string description() override;
  virtual std::string failure_message() override;
  virtual std::string failure_message_when_negated() override;
  virtual bool diffable() { return true; }

 protected:
  bool actual_collection_includes(U expected_item);
  ContainBase(Expectation<A> &expectation,
              std::initializer_list<U> expected)
      : MatcherBase<A, std::vector<U>>(expectation, std::vector<U>(expected)),
        actual(this->get_actual()){};
  ContainBase(Expectation<A> &expectation, U expected)
      : MatcherBase<A, U>(expectation, expected), actual(this->get_actual()){};
};

template <typename A, typename E, typename U>
std::string ContainBase<A, E, U>::description() {
  return Pretty::improve_hash_formatting(
      "contain" + Pretty::to_sentance(this->get_expected()));
}

template <typename A, typename E, typename U>
std::string ContainBase<A, E, U>::failure_message() {
  return Pretty::improve_hash_formatting(MatcherBase<A, E>::failure_message());
}

template <typename A, typename E, typename U>
std::string ContainBase<A, E, U>::failure_message_when_negated() {
  return Pretty::improve_hash_formatting(
      MatcherBase<A, E>::failure_message_when_negated());
}

template <typename A, typename E, typename U>
bool ContainBase<A, E, U>::actual_collection_includes(U expected_item) {
  auto actual = this->get_actual();
  auto last = *(actual.begin());
  static_assert(
      Util::verbose_assert<std::is_same<decltype(last), U>>::value,
      "Expected item is not the same type as what is inside container.");
  return std::find(actual.begin(), actual.end(), expected_item) != actual.end();
}

template <typename A, typename E, typename U>
class Contain : public ContainBase<A, E, U> {
  enum class Predicate { any, all, none };

 public:
  bool match() override;
  bool negated_match() override;
  Contain(Expectation<A> &expectation,
          std::initializer_list<U> expected)
      : ContainBase<A, E, U>(expectation, expected){};

 protected:
  bool perform_match(Predicate predicate, Predicate hash_subset_predicate);
};

template <typename A, typename E, typename U>
bool Contain<A, E, U>::match() {
  return perform_match(Predicate::all, Predicate::all);
}

template <typename A, typename E, typename U>
bool Contain<A, E, U>::negated_match() {
  return perform_match(Predicate::none, Predicate::any);
}

template <typename A, typename E, typename U>
bool Contain<A, E, U>::perform_match(Predicate predicate,
                                     Predicate /*hash_subset_predicate*/) {
  bool retval = true;  // start off true
  for (U expected_item : this->get_expected()) {
    retval = retval && this->actual_collection_includes(expected_item);

    switch (predicate) {
      case Predicate::all:
        if (retval) {    // if the collection includes the item,
          continue;      // continue the loop
        } else {         // otherwise
          return false;  // immediately return false
        }
      case Predicate::none:
        if (retval) {    // if the collection includes the item
          return false;  // immediately return false
        } else {         // otherwise
          continue;      // continue the loop
        }
      case Predicate::any:
        if (retval) {   // if the collection includes the item
          return true;  // immediately return true
        } else {        // otherwise
          continue;     // continue the loop
        }
    }
  }
  return true;
}

template <typename A, typename U>
class Contain<A, U, U> : public ContainBase<A, U, U> {
 public:
  Contain(Expectation<A> &expectation, U expected)
      : ContainBase<A, U, U>(expectation, expected){};

  bool match() override;
};

template <typename A, typename U>
bool Contain<A, U, U>::match() {
  return this->actual_collection_includes(this->get_expected());
}

}  // ::Matchers
}  // namespace CppSpec
#endif  // CPPSPEC_MATCHERS_INCLUDE_HPP
#line 15 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/equal.hpp"
#ifndef CPPSPEC_MATCHERS_EQUAL_HPP
#define CPPSPEC_MATCHERS_EQUAL_HPP
#pragma once

#include <string>
#line 7 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/equal.hpp"

namespace CppSpec {
namespace Matchers {

template <typename A, typename E>
class Equal : public MatcherBase<A, E> {
 public:
  Equal(Expectation<A> &expectation, E expected)
      : MatcherBase<A, E>(expectation, expected) {}

  std::string description() override;
  std::string failure_message() override;
  std::string failure_message_when_negated() override;
  bool diffable();

 protected:
  bool match() override;
  bool expected_is_a_literal();
  std::string actual_inspected();
  std::string simple_failure_message();
};

template <typename A, typename E>
std::string Equal<A, E>::description() {
  std::stringstream ss;
  ss << "equal" << Pretty::to_sentance<E>(this->expected);
  return ss.str();
}

template <typename A, typename E>
std::string Equal<A, E>::failure_message() {
  return simple_failure_message();
}

template <typename A, typename E>
std::string Equal<A, E>::failure_message_when_negated() {
  std::stringstream ss;
  ss << "expected not "
     << Pretty::inspect_object(MatcherBase<A, E>::get_expected()) << "\n"
     << "         got " << actual_inspected() << "\n"
     << "Compared using `==`" << std::endl;
  return ss.str();
}

template <typename A, typename E>
std::string Equal<A, E>::simple_failure_message() {
  std::stringstream ss;
  ss << "expected " << Pretty::inspect_object(MatcherBase<A, E>::get_expected())
     << "\n"
     << "     got " << actual_inspected() << "\n"
     << "Compared using `==`" << std::endl;
  return ss.str();
}

template <typename A, typename E>
bool Equal<A, E>::diffable() {
  return !expected_is_a_literal();
}

template <typename A, typename E>
bool Equal<A, E>::match() {
  return this->get_expected() == this->get_actual();
}

template <typename A, typename E>
bool Equal<A, E>::expected_is_a_literal() {
  return (typeid(E) == typeid(bool));
}


template <typename A, typename E>
std::string Equal<A, E>::actual_inspected() {
  return Pretty::inspect_object(MatcherBase<A, E>::get_actual());
}



}  // namespace Matchers
}  // namespace CppSpec
#endif  // CPPSPEC_MATCHERS_EQUAL_HPP
#line 16 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/fail.hpp"
#ifndef CPPSPEC_MATCHERS_FAIL_HPP
#define CPPSPEC_MATCHERS_FAIL_HPP
#pragma once

#include <string>
#line 7 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/fail.hpp"

namespace CppSpec {
namespace Matchers {

template <typename A>
class Fail : public MatcherBase<A, void *> {
 public:
  static_assert(std::is_same<A, Result>::value,
                ".fail must be matched against a Result.");
  explicit Fail(Expectation<A> &expectation)
      : MatcherBase<A, void *>(expectation, nullptr) {}

  typename std::enable_if<std::is_same<A, Result>::value, bool>::type match() {
    return not this->get_actual().get_status();
  }
};

template <typename A>
class FailWith : public Matchers::MatcherBase<A, std::string> {
 public:
  FailWith(Expectation<A> &expectation, std::string expected)
      : Matchers::MatcherBase<A, std::string>(expectation, expected) {}

  typename std::enable_if<std::is_same<A, Result>::value, bool>::type match();
};

template <typename A>
typename std::enable_if<std::is_same<A, Result>::value, bool>::type
FailWith<A>::match() {
  static_assert(std::is_same<A, Result>::value,
                ".fail_with must be matched against a Result.");
  return (not this->get_actual().get_status()) &&
         this->get_actual().get_message() == this->get_expected();
}

}  // namespace Matchers
}  // namespace CppSpec
#endif  // CPPSPEC_FAIL_HPP
#line 17 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/match.hpp"
#ifndef CPPSPEC_MATCHERS_MATCH_HPP
#define CPPSPEC_MATCHERS_MATCH_HPP
#pragma once
#include <string>
#include <regex>
#line 7 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/match.hpp"
namespace CppSpec {
namespace Matchers {

template <typename A>
class Match : MatcherBase<A, std::regex> {
 public:
  explicit Match(Expectation<A> &expectation,
                 std::string expected)
      : MatcherBase<A, std::regex>(expectation, std::regex(expected)) {}

  explicit Match(Expectation<A> &expectation, std::regex expected)
      : MatcherBase<A, std::regex>(expectation, expected) {}

  std::string description() override {
    return "match " + Pretty::to_word(this->get_expected());
  }

  virtual bool match() override {
    std::smatch temp_match;
    return std::regex_match(this->get_actual(), temp_match,
                            this->get_expected());
  }
};

template <typename A>
class MatchPartial : public MatcherBase<A, std::regex> {
 public:
  explicit MatchPartial(Expectation<A> &expectation,
                        std::string expected)
      : MatcherBase<A, std::regex>(expectation, std::regex(expected)) {}

  explicit MatchPartial(Expectation<A> &expectation,
                        std::regex expected)
      : MatcherBase<A, std::regex>(expectation, expected) {}

  std::string description() override {
    return "partially match " + Pretty::to_word(this->get_expected());
  }

  bool match() override {
    return std::regex_match(this->get_actual(), this->get_expected());
  }
};

}  // namespace Matchers
}  // namespace CppSpec

#endif  // CPPSPEC_MATCHERS_BE_NULLPTR_HPP
#line 18 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/satisfy.hpp"
#ifndef CPPSPEC_MATCHERS_SATISFY_HPP
#define CPPSPEC_MATCHERS_SATISFY_HPP
#pragma once

#include <string>
#line 7 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/satisfy.hpp"

namespace CppSpec {
namespace Matchers {

template <typename A>
class Satisfy : public MatcherBase<A, bool> //, BeHelpers<Satisfy<A>>
{
  std::function<bool(A)> test;

 public:
  Satisfy(Expectation<A> &expectation, std::function<bool(A)> test)
      : MatcherBase<A, bool>(expectation), test(test) {}

  std::string description() override;
  std::string failure_message() override;
  std::string failure_message_when_negated() override;

  bool match() override;
};

template <typename A>
std::string Satisfy<A>::description() {
  std::stringstream ss;
  ss << "be" << Pretty::to_sentance(this->expected);
  return ss.str();
}

template <typename A>
std::string Satisfy<A>::failure_message() {
  std::stringstream ss;
  ss << "expected " << MatcherBase<A, bool>::get_actual()
     << " to evaluate to true";
  return ss.str();
}

template <typename A>
std::string Satisfy<A>::failure_message_when_negated() {
  std::stringstream ss;
  ss << "expected " << MatcherBase<A, bool>::get_actual()
     << " to evaluate to false";
  return ss.str();
}

template <typename A>
bool Satisfy<A>::match() {
  return test(this->get_actual());
}

}  // namespace Matchers
}  // namespace CppSpec
#endif  // CPPSPEC_MATCHERS_SATISFY_HPP
#line 19 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/start_with.hpp"
#ifndef CPPSPEC_MATCHERS_START_WITH_HPP
#define CPPSPEC_MATCHERS_START_WITH_HPP
#pragma once

#include <string>
#line 7 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/start_with.hpp"

namespace CppSpec {
namespace Matchers {

template <typename A, typename E>
class StartWith : public MatcherBase<A, E> {
  static_assert(Util::is_iterable<A>::value && Util::is_iterable<E>::value,
                "Error! Trying to match starting items with a non-iterable "
                    "container");

 public:
  StartWith(Expectation <A> &expectation, E start)
      : MatcherBase<A, E>(expectation, start) { }

  std::string description() override {
    return "start with " + Pretty::to_word(this->expected);
  }

  bool match() override {
    A &actual = this->get_actual();
    E &expected = this->get_expected();
    return std::equal(expected.begin(), expected.end(), actual.begin());
  }
};

}  // namespace Matchers
}  // namespace CppSpec
#endif  // CPPSPEC_MATCHERS_START_WITH_HPP
#line 20 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/end_with.hpp"
#ifndef CPPSPEC_MATCHERS_END_WITH_HPP
#define CPPSPEC_MATCHERS_END_WITH_HPP
#pragma once

#include <string>
#line 7 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/end_with.hpp"

namespace CppSpec {
namespace Matchers {

template <typename A, typename E>
class EndWith : public MatcherBase<A, E> {
  static_assert(Util::is_iterable<A>::value && Util::is_iterable<E>::value,
                "Error! Trying to match starting items with a non-iterable "
                    "container");

 public:
 EndWith(Expectation <A> &expectation, E start)
     : MatcherBase<A, E>(expectation, start) { }

  std::string description() override {
    return "end with " + Pretty::to_word(this->expected);
  }

  bool match() override {
    A &actual = this->get_actual();
    E &expected = this->get_expected();
    return std::equal(expected.rbegin(), expected.rend(), actual.rbegin());
  }
};

}  // namespace Matchers
}  // namespace CppSpec
#endif  // CPPSPEC_MATCHERS_END_WITH_HPP
#line 21 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/throw.hpp"
#ifndef C_SPEC_THROW_HPP
#define C_SPEC_THROW_HPP
#pragma once

#include <type_traits>
#include <functional>
#line 8 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/throw.hpp"
#line 9 "/mnt/c/Users/Kate/GitHub/cppspec/include/matchers/throw.hpp"

namespace CppSpec {

namespace Matchers {

template <class A, class Ex>
class Throw : public MatcherBase<A, void *> {
 public:
  explicit Throw(Expectation<A> &expectation)
      : MatcherBase<A, void *>(expectation, nullptr) {}
  bool match() override;
  std::string description() override;
  std::string failure_message() override;
  std::string failure_message_when_negated() override;
};

template <class A, class Ex>
bool Throw<A, Ex>::match() {
  bool caught = false;
  try {
    this->get_actual();
  } catch (Ex &ex) {
    caught = true;
  }
  return caught;
}

template <typename A, typename Ex>
std::string Throw<
    A, Ex>::description() {
  std::stringstream ss;
  ss << "throw " << Util::demangle(typeid(Ex).name());
  return ss.str();
}

template <typename A, typename Ex>
std::string
Throw<A, Ex>::failure_message() {
  std::stringstream ss;
  ss << "expected the given function ([] -> " << Util::demangle(typeid(A).name()) << " {...}) to "
     << this->description();
  return ss.str();
}

template <typename A, typename Ex>
std::string Throw<A, Ex>::
    failure_message_when_negated() {
  std::stringstream ss;
  ss << "expected the given function ([] -> " << Util::demangle(typeid(A).name()) << " {...}) not to "
     << this->description();
  return ss.str();
}

}  // namespace Matchers
}  // namespace CppSpec

#endif  // C_SPEC_THROW_HPP
#line 22 "/mnt/c/Users/Kate/GitHub/cppspec/include/expectations/expectation.hpp"

namespace CppSpec {

template <class A>
class Expectation : public Child {
 protected:
  bool is_positive = true;
  bool ignore_failure = false;

 public:
  Expectation(const Expectation &) = default;

  explicit Expectation(ItBase &it) : Child(Child::of(it)) {}

  virtual A &get_target()& = 0;

  const bool get_sign() const { return is_positive; }
  bool get_sign() { return is_positive; }
  const bool get_ignore_failure() const { return ignore_failure; }
  bool get_ignore_failure() { return ignore_failure; }

  virtual Expectation &not_() = 0;
  virtual Expectation &ignore() = 0;

  template <class M>
  Result to(M matcher, std::string msg = "");

  /*-------- to be... ----------*/

  Result to_be_false(std::string msg = "");
  Result to_be_falsy(std::string msg = "");
  Result to_be_null(std::string msg = "");
  Result to_be_true(std::string msg = "");
  Result to_be_truthy(std::string msg = "");

  template <typename E>
  Result to_be_between(
      E min, E max, Matchers::RangeMode mode = Matchers::RangeMode::inclusive,
      std::string msg = "");

  template <typename E>
  Result to_be_greater_than(E rhs, std::string msg = "");

  template <typename E>
  Result to_be_less_than(E rhs, std::string msg = "");

  template <typename E>
  Matchers::BeWithin<A, E> to_be_within(E expected, std::string msg = "");

  /*-------- to... ----------*/

  Result to_end_with(std::string ending, std::string msg = "");
  Result to_fail(std::string msg = "");
  Result to_fail_with(std::string failure_message, std::string msg = "");
  Result to_match(std::regex regex, std::string msg = "");
  Result to_match(std::string str, std::string msg = "");
  Result to_partially_match(std::regex regex, std::string msg = "");
  Result to_partially_match(std::string str, std::string msg = "");
  Result to_satisfy(std::function<bool(A)>, std::string msg = "");
  Result to_start_with(std::string start, std::string msg = "");

  template <typename U>
  Result to_contain(std::initializer_list<U> expected, std::string msg = "");

  template <typename E>
  Result to_contain(E expected, std::string msg = "");

  template <typename U>
  Result to_end_with(std::initializer_list<U> start, std::string msg = "");

  template <typename E>
  Result to_equal(E expected, std::string msg = "");

  template <typename U>
  Result to_start_with(std::initializer_list<U> start, std::string msg = "");
};

template <typename A>
template <class M>
Result Expectation<A>::to(M matcher, std::string msg) {
  static_assert(
      std::is_base_of<Matchers::MatcherBase<A, typename M::expected_t>,
                      M>::value,
      "Matcher is not a subclass of BaseMatcher.");
  return matcher.set_message(msg).run(this->get_formatter());
}

template <typename A>
Result Expectation<A>::to_be_false(std::string msg) {
  static_assert(std::is_same<A, bool>::value,
                "Error! to_be_false can only be used on booleans or functions "
                "that return booleans");
  return to_equal(false, msg);
}

template <typename A>
Result Expectation<A>::to_be_falsy(std::string msg) {
  return to_satisfy([](const A &t) { return !static_cast<bool>(t); }, msg);
}

template <typename A>
Result Expectation<A>::to_be_null(std::string msg) {
  return Matchers::BeNullptr<A>(*this).set_message(msg).run(
      this->get_formatter());
}

template <typename A>
Result Expectation<A>::to_be_true(std::string msg) {
  static_assert(std::is_same<A, bool>::value,
                "Error! to_be_true can only be used on booleans or functions "
                "that return booleans");
  return to_equal(true, msg);
}

template <typename A>
Result Expectation<A>::to_be_truthy(std::string msg) {
  return to_satisfy([](const A &t) { return static_cast<bool>(t); }, msg);
}

template <typename A>
template <typename E>
Result Expectation<A>::to_be_between(E min, E max, Matchers::RangeMode mode,
                                     std::string msg) {
  return Matchers::BeBetween<A, E>(*this, min, max, mode)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
template <typename E>
Result Expectation<A>::to_be_less_than(E rhs, std::string msg) {
  return Matchers::BeLessThan<A, E>(*this, rhs)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
template <typename E>
Result Expectation<A>::to_be_greater_than(E rhs, std::string msg) {
  return Matchers::BeGreaterThan<A, E>(*this, rhs)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
template <typename U>
Result Expectation<A>::to_contain(std::initializer_list<U> expected,
                                  std::string msg) {
  return Matchers::Contain<A, std::vector<U>, U>(*this, expected)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
template <typename E>
Result Expectation<A>::to_contain(E expected, std::string msg) {
  return Matchers::Contain<A, E, E>(*this, expected)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
template <typename E>
Result Expectation<A>::to_equal(E expected, std::string msg) {
  return Matchers::Equal<A, E>(*this, expected)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
template <typename E>
Matchers::BeWithin<A, E> Expectation<A>::to_be_within(E expected,
                                                      std::string msg) {
  Matchers::BeWithin<A, E> matcher(*this, expected);
  matcher.set_message(msg);
  return matcher;
}

template <typename A>
Result Expectation<A>::to_fail(std::string msg) {
  static_assert(std::is_same<A, Result>::value,
                "Error: to_fail must be used on an expression that "
                "returns a Result.");
  return Matchers::Fail<Result>(*this).set_message(msg).run(
      this->get_formatter());
}


template <typename A>
Result Expectation<A>::to_fail_with(std::string failure_message,
                                    std::string msg) {
  static_assert(std::is_same<A, Result>::value,
                "Error: to_fail_with must be used on an expression that "
                "returns a Result.");
  return Matchers::FailWith<A>(*this, failure_message)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
Result Expectation<A>::to_match(std::string str, std::string msg) {
  return Matchers::Match<A>(*this, str)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
Result Expectation<A>::to_match(std::regex regex, std::string msg) {
  return Matchers::Match<A>(*this, regex)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
Result Expectation<A>::to_partially_match(std::string str, std::string msg) {
  return Matchers::MatchPartial<A>(*this, str)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
Result Expectation<A>::to_partially_match(std::regex regex, std::string msg) {
  return Matchers::MatchPartial<A>(*this, regex)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
Result Expectation<A>::to_satisfy(std::function<bool(A)> test,
                                  std::string msg) {
  return Matchers::Satisfy<A>(*this, test)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
Result Expectation<A>::to_start_with(std::string start, std::string msg) {
  return Matchers::StartWith<std::string, std::string>(*this, start)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
template <typename U>
Result Expectation<A>::to_start_with(std::initializer_list<U> start_sequence,
                                     std::string msg) {
  return Matchers::StartWith<A, std::initializer_list<U>>(*this, start_sequence)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
Result Expectation<A>::to_end_with(std::string ending, std::string msg) {
  return Matchers::EndWith<std::string, std::string>(*this, ending)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
template <typename U>
Result Expectation<A>::to_end_with(std::initializer_list<U> start_sequence,
                                   std::string msg) {
  return Matchers::StartWith<A, std::initializer_list<U>>(*this, start_sequence)
      .set_message(msg)
      .run(this->get_formatter());
}

template <typename A>
class ExpectationValue : public Expectation<A> {
  A value;

 public:
  ExpectationValue(ExpectationValue const &copy)
      : Expectation<A>(copy), value(copy.value) {}

  ExpectationValue(ItBase &it, A value) : Expectation<A>(it), value(value) {}

  template <typename U>
  ExpectationValue(ItBase &it, std::initializer_list<U> init_list)
      : Expectation<A>(it), value(std::vector<U>(init_list)) {}

  A &get_target() & override { return value; }

  ExpectationValue &not_() override {
    this->is_positive = not this->is_positive;
    return *this;
  }

  ExpectationValue &ignore() override {
    this->ignore_failure = true;
    return *this;
  }
};

template <typename F>
class ExpectationFunc : public Expectation<decltype(std::declval<F>()())> {
  static_assert(Util::is_functional<F>::value,
                "Error! ExpectationFunc can only contaion lambdas.");

  typedef decltype(std::declval<F>()()) block_ret_t;
  F block;
  std::shared_ptr<block_ret_t> computed = nullptr;

 public:
  ExpectationFunc(ExpectationFunc<F> const &copy)
      : Expectation<block_ret_t>(copy), block(copy.block) {}

  ExpectationFunc(ItBase &it, F block)
      : Expectation<block_ret_t>(it), block(block) {}


  block_ret_t &get_target() & override {
    if (computed == nullptr) {
      computed = std::make_shared<block_ret_t>(block());
    }
    return *computed;
  }


  ExpectationFunc &not_() override {
    this->is_positive = !this->is_positive;
    return *this;
  }

  ExpectationFunc &ignore() override {
    this->ignore_failure = true;
    return *this;
  }

  Expectation<decltype(block())> &casted() {
    return static_cast<decltype(block())>(*this);
  }

  template <typename Ex = std::exception>
  Result to_throw(std::string msg = "");
};

template <typename F>
template <typename Ex>
Result ExpectationFunc<F>::to_throw(std::string msg) {
  Matchers::Throw<decltype(this->block.operator()()), Ex> matcher(*this);
  return matcher.set_message(msg).run(this->get_formatter());
}

}  // namespace CppSpec
#endif  // CPPSPEC_EXPECTATIONS_EXPECTATION_HPP
#line 8 "/mnt/c/Users/Kate/GitHub/cppspec/include/it.hpp"

namespace CppSpec {

class ItD : public ItBase {
 public:
  using Block = std::function<void(ItD &)>;

 private:
  const Block block;

 public:
  ItD(const Child &parent, std::string description, Block block)
      : ItBase(parent, description), block(block) {}

  ItD(const Child &parent, Block block) : ItBase(parent), block(block) {}

  Result run(Formatters::BaseFormatter &printer) override;
};

template <typename T>
class ItCD : public ItBase {
 public:
  using Block = std::function<void(ItCD<T> &)>;

 private:
  const Block block;

 public:
  T &subject;

  ItCD(const Child &parent, T &subject, std::string description, Block block)
      : ItBase(parent, description), block(block), subject(subject) {}

  ItCD(const Child &parent, T &subject, Block block)
      : ItBase(parent), block(block), subject(subject) {}

  ExpectationValue<T> is_expected();
  Result run(Formatters::BaseFormatter &printer) override;
};

template <class T>
typename std::enable_if<not Util::is_functional<T>::value,
                        ExpectationValue<T>>::type
ItBase::expect(T value) {
  return ExpectationValue<T>(*this, value);
}

template <typename T>
auto ItBase::expect(T block) ->
    typename std::enable_if<Util::is_functional<T>::value,
                            ExpectationFunc<T>>::type {
  return ExpectationFunc<T>(*this, block);
}

template <typename T>
ExpectationValue<T> ItBase::expect(Let<T> &let) {
  return ExpectationValue<T>(*this, let.value());
}

template <class T>
ExpectationValue<std::initializer_list<T>> ItBase::expect(
    std::initializer_list<T> init_list) {
  return ExpectationValue<std::initializer_list<T>>(*this, init_list);
}

inline ExpectationValue<std::string> ItBase::expect(const char *str) {
  return ExpectationValue<std::string>(*this, std::string(str));
}

}  // namespace CppSpec
#endif  // CPPSPEC_IT_HPP
#line 10 "/mnt/c/Users/Kate/GitHub/cppspec/include/description.hpp"
#line 11 "/mnt/c/Users/Kate/GitHub/cppspec/include/description.hpp"

namespace CppSpec {

template <typename T>
class ClassDescription;  // forward-declaration for ClassDescription

class Description : public Runnable {
  using VoidBlock = std::function<void()>;

 public:
  using Block = std::function<void(Description &)>;

  const bool has_subject = false;
  std::forward_list<LetBase *> lets;
  std::deque<VoidBlock> after_alls;
  std::deque<VoidBlock> before_eaches;
  std::deque<VoidBlock> after_eaches;

 private:
  Block block;

 protected:
  std::string description = "";

  Description() = default;
  explicit Description(std::string description) noexcept
      : description(description) {}

  Description(const Child &parent, std::string description,
              Block block) noexcept : Runnable(parent),
                                      block(block),
                                      description(description) {}

  void exec_before_eaches();
  void exec_after_eaches();

 public:
  Description(const Description &copy) = default;

  Description(std::string description, Block block) noexcept
      : block(block),
        description(description) {}

  Result it(std::string description, ItD::Block body);
  Result it(ItD::Block body);

  Result context(std::string name, Block body);

  template <class T>
  Result context(T subject, std::function<void(ClassDescription<T> &)> block);

  template <class T>
  Result context(std::string description, T subject,
                 std::function<void(ClassDescription<T> &)> block);

  template <class T, typename U>
  Result context(std::initializer_list<U> init_list,
                 std::function<void(ClassDescription<T> &)> block);

  void before_each(VoidBlock block);
  void before_all(VoidBlock block);
  void after_each(VoidBlock block);
  void after_all(VoidBlock block);

  template <typename T>
  auto let(T body) -> Let<decltype(body())>;
  void reset_lets() noexcept;

  virtual std::string get_description() const noexcept { return description; }
  virtual std::string get_subject_type() const noexcept { return ""; }

  Result run(Formatters::BaseFormatter &printer) override;
};

using Context = Description;

/*>>>>>>>>>>>>>>>>>>>> Description <<<<<<<<<<<<<<<<<<<<<<<<<*/

/*========= Description::it =========*/

inline Result Description::it(std::string description, ItD::Block block) {
  ItD it(*this, description, block);
  Result result = it.run(this->get_formatter());
  exec_after_eaches();
  exec_before_eaches();
  return result;
}

inline Result Description::it(ItD::Block block) {
  ItD it(*this, block);
  Result result = it.run(this->get_formatter());
  exec_after_eaches();
  exec_before_eaches();
  return result;
}

/*========= Description::context =========*/

inline Result Description::context(std::string description, Block body) {
  Context context(*this, description, body);
  context.before_eaches = this->before_eaches;
  context.after_eaches = this->after_eaches;
  return context.run(this->get_formatter());
}

/*========= Description:: each/alls =========*/

inline void Description::before_each(VoidBlock b) {
  before_eaches.push_back(b);

  b();
}

inline void Description::before_all(VoidBlock b) { b(); }

inline void Description::after_each(VoidBlock b) { after_eaches.push_back(b); }

inline void Description::after_all(VoidBlock b) { after_alls.push_back(b); }

/*----------- private -------------*/

inline void Description::exec_before_eaches() {
  for (VoidBlock &b : before_eaches) b();
}

inline void Description::exec_after_eaches() {
  for (VoidBlock &b : after_eaches) b();
}

/*========= Description::let =========*/

template <typename T>
auto Description::let(T block) -> Let<decltype(block())> {

  Let<decltype(block())> let(block);  // Create a Let
  lets.push_front(&let);              // Add it to our list
  return let;                         // Hand it object off
}

inline void Description::reset_lets() noexcept {
  for (auto &let : lets) let->reset();

  if (this->has_parent()) {
    this->get_parent_as<Description *>()->reset_lets();
  }
}

/*========= Description::run =========*/

inline Result Description::run(Formatters::BaseFormatter &formatter) {
  if (!this->has_formatter()) this->set_formatter(formatter);

  formatter.format(*this);              // Format our description in some way
  block(*this);                         // Run the block
  for (VoidBlock &a : after_alls) a();  // Run all our after_alls
  if (!this->has_parent()) formatter.flush();  // Inform the printer we're done

  return this->get_status() ? Result::success() : Result::failure();
}

/*>>>>>>>>>>>>>>>>>>>> ItD <<<<<<<<<<<<<<<<<<<<<<<<<*/

/*========= ItD::run =========*/

inline Result ItD::run(Formatters::BaseFormatter &printer) {
  block(*this);
  printer.format(*this);
  this->get_parent_as<Description *>()->reset_lets();
  return this->get_status() ? Result::success() : Result::failure();
}

}  // namespace CppSpec
#endif  // CPPSPEC_DESCRIPTION_HPP
#line 5 "/mnt/c/Users/Kate/GitHub/cppspec/include/class_description.hpp"

namespace CppSpec {

template <class T>
class ClassDescription : public Description {
  using Block = std::function<void(ClassDescription<T> &)>;

  Block block;
  std::string type = "";

 public:
  const bool has_subject = true;
  T subject;  // subject field public for usage in `expect([self.]subject)`

  ClassDescription<T>(Block block)
      : Description(),
        block(block),
        type(" : " + Util::demangle(typeid(T).name())),
        subject(T()) {
    this->description = Pretty::to_word(subject);
  }

  ClassDescription<T>(std::string description, Block block)
      : Description(description), block(block), subject(T()) {}

  ClassDescription(T subject, Block block)
      : Description(Pretty::to_word(subject)),
        block(block),
        type(" : " + Util::demangle(typeid(T).name())),
        subject(subject) {}

  ClassDescription(std::string description, T subject, Block block)
      : Description(description), block(block), subject(subject) {}

  ClassDescription(T &subject, Block block)
      : Description(Pretty::to_word(subject)),
        block(block),
        type(" : " + Util::demangle(typeid(T).name())),
        subject(subject) {}

  template <typename U>
  ClassDescription(std::initializer_list<U> init_list, Block block)
      : block(block),
        type(" : " + Util::demangle(typeid(T).name())),
        subject(T(init_list)) {
    this->description = Pretty::to_word(subject);
  }

  template <typename U>
  ClassDescription(std::string description, std::initializer_list<U> init_list,
                   Block block)
      : Description(description), block(block), subject(T(init_list)) {}

  Result it(std::string description, std::function<void(ItCD<T> &)> block);
  Result it(std::function<void(ItCD<T> &)> block);
  Result specify(std::string description,
                 std::function<void(ItCD<T> &)> block) {
    return it(description, block);
  }
  Result specify(std::function<void(ItCD<T> &)> block) { return it(block); }

  template <class U>
  Result context(std::string description, U subject,
                 std::function<void(ClassDescription<U> &)> block);
  template <class U>
  Result context(std::string description, U &subject,
                 std::function<void(ClassDescription<U> &)> block);
  template <class U>
  Result context(U subject, std::function<void(ClassDescription<U> &)> block);
  template <class U>
  Result context(U &subject, std::function<void(ClassDescription<U> &)> block);

  Result context(std::string description,
                 std::function<void(ClassDescription<T> &)> block);

  Result run(Formatters::BaseFormatter &printer) override;

  std::string get_subject_type() const noexcept override { return type; }
};

template <class T>
using ClassContext = ClassDescription<T>;

template <class T>
template <class U>
Result ClassDescription<T>::context(
    std::string description, U subject,
    std::function<void(ClassDescription<U> &)> block) {
  ClassContext<U> context(description, subject, block);
  context.set_parent(this);
  context.ClassContext<U>::before_eaches = this->before_eaches;
  context.ClassContext<U>::after_eaches = this->after_eaches;
  return context.run(this->get_formatter());
}

template <class T>
template <class U>
Result ClassDescription<T>::context(
    U subject, std::function<void(ClassDescription<U> &)> block) {
  return this->context("", std::forward<U>(subject), block);
}

template <class T>
template <class U>
Result ClassDescription<T>::context(
    std::string description, U &subject,
    std::function<void(ClassDescription<U> &)> block) {
  ClassContext<U> context(description, subject, block);
  context.set_parent(this);
  context.ClassContext<U>::before_eaches = this->before_eaches;
  context.ClassContext<U>::after_eaches = this->after_eaches;
  return context.run(this->get_formatter());
}

template <class T>
template <class U>
Result ClassDescription<T>::context(
    U &subject, std::function<void(ClassDescription<U> &)> block) {
  return this->context("", std::forward<U>(subject), block);
}

template <class T>
Result ClassDescription<T>::context(
    std::string description, std::function<void(ClassDescription<T> &)> block) {
  ClassContext<T> context(description, this->subject, block);
  context.set_parent(this);
  context.before_eaches = this->before_eaches;
  context.after_eaches = this->after_eaches;
  return context.run(this->get_formatter());
}

template <class T>
Result Description::context(T subject,
                            std::function<void(ClassDescription<T> &)> block) {
  return this->context("", subject, block);
}

template <class T>
Result Description::context(std::string description, T subject,
                            std::function<void(ClassDescription<T> &)> block) {
  ClassContext<T> context(description, subject, block);
  context.set_parent(this);
  context.before_eaches = this->before_eaches;
  context.after_eaches = this->after_eaches;
  return context.run(this->get_formatter());
}

template <class T, typename U>
Result Description::context(std::initializer_list<U> init_list,
                            std::function<void(ClassDescription<T> &)> block) {
  ClassContext<T> context(T(init_list), block);
  context.set_parent(this);
  context.before_eaches = this->before_eaches;
  context.after_eaches = this->after_eaches;
  return context.run(this->get_formatter());
}

template <class T>
Result ClassDescription<T>::it(std::string name,
                               std::function<void(ItCD<T> &)> block) {
  ItCD<T> it(*this, this->subject, name, block);
  Result result = it.run(this->get_formatter());
  exec_after_eaches();
  exec_before_eaches();
  return result;
}

template <class T>
Result ClassDescription<T>::it(std::function<void(ItCD<T> &)> block) {
  ItCD<T> it(*this, this->subject, block);
  Result result = it.run(this->get_formatter());
  exec_after_eaches();
  exec_before_eaches();
  return result;
}

template <class T>
Result ClassDescription<T>::run(Formatters::BaseFormatter &printer) {
  if (not this->has_formatter()) this->set_formatter(printer);
  printer.format(*this);
  this->block(*this);
  for (const auto &a : after_alls) a();
  if (this->get_parent() == nullptr) printer.flush();
  return this->get_status() ? Result::success() : Result::failure();
}

template <class T>
ExpectationValue<T> ItCD<T>::is_expected() {
  auto cd = static_cast<ClassDescription<T> *>(this->get_parent());
  ExpectationValue<T> expectation(*this, cd->subject);
  return expectation;
}

template <class T>
Result ItCD<T>::run(Formatters::BaseFormatter &printer) {
  this->block(*this);
  printer.format(*this);
  auto cd = static_cast<ClassDescription<T> *>(this->get_parent());
  cd->reset_lets();
  return this->get_status() ? Result::success() : Result::failure();
}

}  // namespace CppSpec
#endif  // CPPSPEC_CLASS_DESCRIPTION_HPP
#line 9 "/mnt/c/Users/Kate/GitHub/cppspec/include/formatters/verbose.hpp"

namespace CppSpec {
namespace Formatters {

class Verbose : public BaseFormatter {
  bool first = true;
  std::list<std::string> failure_messages;

 public:
  Verbose() = default;
  explicit Verbose(std::ostream &out_stream) : BaseFormatter(out_stream) {}
  Verbose(const BaseFormatter &base, std::ostream &out_stream)
      : BaseFormatter(base, out_stream) {}
  explicit Verbose(const BaseFormatter &base) : BaseFormatter(base) {}

  void format(const Description &description) override;
  void format(const ItBase &description) override;
  void format_failure(std::string message) override;
  void format_failure_messages();
};

inline void Verbose::format(const Description &description) {
  if (!first && !description.has_parent()) out_stream << std::endl;
  out_stream << description.padding() << description.get_description()
             << description.get_subject_type() << std::endl;
  if (first) this->first = false;
}

inline void Verbose::format(const ItBase &it) {
  if (color_output) out_stream << (it.get_status() ? GREEN : RED);
  out_stream << it.padding() << it.get_description() << std::endl;
  if (color_output) out_stream << RESET;

  format_failure_messages();
  get_and_increment_test_counter();
}

inline void Verbose::format_failure(std::string message) {
  failure_messages.push_back(message);
}

inline void Verbose::format_failure_messages() {
  if (not failure_messages.empty()) {     // If we have any failures to format
    if (color_output) out_stream << RED;  // make them red
    out_stream << Util::join(failure_messages,
                             "\n")  // separated by a blank line
               << std::endl;        // newline
    if (color_output) out_stream << RESET;
    failure_messages.clear();  // Finally, clear the failures list.
  }
}

static Verbose verbose;

}  // namespace Formatters
}  // namespace CppSpec
#endif  // CPPSPEC_FORMATTERS_VERBOSE_HPP
#line 7 "/mnt/c/Users/Kate/GitHub/cppspec/include/runner.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/formatters/progress.hpp"
#ifndef CPPSPEC_FORMATTERS_PROGRESS_HPP
#define CPPSPEC_FORMATTERS_PROGRESS_HPP
#pragma once

#include <string>
#include <list>
#include <forward_list>
#line 9 "/mnt/c/Users/Kate/GitHub/cppspec/include/formatters/progress.hpp"

namespace CppSpec {
namespace Formatters {

class Progress : public BaseFormatter {
  std::list<std::string> baked_failure_messages;
  std::list<std::string> raw_failure_messages;

  std::string prep_failure_helper(const ItBase &it);

 public:
  void format(const ItBase &it) override;
  void format_failure(std::string message) override;
  void flush() override;
  void cleanup() override;

  void format_failure_messages();
  void prep_failure(const ItBase &it);
};

inline std::string Progress::prep_failure_helper(const ItBase &it) {
  std::forward_list<std::string> list;

  std::ostringstream temp_stream;

  auto push_and_clear = [&]() {
    list.push_front(temp_stream.str());  // push the stream's contents onto the
    temp_stream.clear();                 // clear the stream's flags
    temp_stream.str("");                 // reset it to empty
  };

  temp_stream << it.padding() << it.get_description() << std::endl;
  push_and_clear();

  Verbose helper_formatter = Verbose(*this, temp_stream);

  const Description *parent = it.get_parent_as<const Description *>();

  do {
    helper_formatter.format(*parent);  // Format the node
    push_and_clear();
  } while ((parent = dynamic_cast<const Description *>(parent->get_parent())));

  return Util::join(list);  // squash the list of strings and return it.
}

inline void Progress::prep_failure(const ItBase &it) {
  std::ostringstream string_builder;  // oss is used as the local string builder
  if (color_output) string_builder << RED;  // if we're doing color, make it red
  string_builder << "Test number " << test_counter
                 << " failed:";  // Tell us what test # failed
  if (color_output)
    string_builder << RESET;  // if we're doing color, reset the terminal
  string_builder << prep_failure_helper(it);
  if (color_output) string_builder << RED;
  string_builder << Util::join(raw_failure_messages, "\n");
  if (color_output) string_builder << RESET;
  raw_failure_messages.clear();
  baked_failure_messages.push_back(string_builder.str());
}

inline void Progress::format(const ItBase &it) {
  if (it.get_status()) {
    if (color_output) out_stream << GREEN;
    out_stream << ".";
  } else {
    if (color_output) out_stream << RED;
    out_stream << "F";
    prep_failure(it);
  }
  if (color_output) out_stream << RESET;
  out_stream << std::flush;
  get_and_increment_test_counter();
}

inline void Progress::format_failure(std::string message) {
  raw_failure_messages.push_back(message);
}

inline void Progress::flush() {
  if (not multiple) {         // If we aren't executing through a runner
    out_stream << std::endl;  // always newline
    format_failure_messages();
    test_counter = 1;  // and reset the test counter
  }
}

inline void Progress::cleanup() {
  if (multiple) {
    out_stream << std::endl;
    format_failure_messages();
  }
}

inline void Progress::format_failure_messages() {
  if (not baked_failure_messages
              .empty()) {  // If we have any failures to format
    out_stream << Util::join(baked_failure_messages,
                             "\n\n")  // separated by a blank line
               << std::endl;          // newline
    baked_failure_messages.clear();  // Finally, clear the failures list.
  }
}

static Progress progress;

}  // namespace Formatters
}  // namespace CppSpec
#endif  // CPPSPEC_FORMATTERS_PROGRESS_HPP
#line 8 "/mnt/c/Users/Kate/GitHub/cppspec/include/runner.hpp"
#line 1 "/mnt/c/Users/Kate/GitHub/cppspec/include/formatters/tap.hpp"
#ifndef CPPSPEC_FORMATTERS_TAP_HPP
#define CPPSPEC_FORMATTERS_TAP_HPP
#pragma once

#include <string>
#include <list>
#line 8 "/mnt/c/Users/Kate/GitHub/cppspec/include/formatters/tap.hpp"
#line 9 "/mnt/c/Users/Kate/GitHub/cppspec/include/formatters/tap.hpp"

namespace CppSpec {
namespace Formatters {

class TAP : public BaseFormatter {
  bool first = true;
  std::ostringstream buffer;
  std::list<std::string> failure_messages;

 public:
  void format(const Description &description) override;
  void format(const ItBase &it) override;
  void format(std::string message) override;
  void format_failure(std::string message) override;
  void flush() override;
};

inline void TAP::format(const Description &description) {
  if (!first && description.get_parent() == nullptr) out_stream << std::endl;
  if (first) this->first = false;
}

inline void TAP::format(const ItBase &it) {
  std::string description = it.get_description();

  const Description *parent = it.get_parent_as<const Description *>();
  while (parent != nullptr) {
    description = parent->get_description() + " " + description;
    parent = it.get_parent_as<const Description *>();
  }

  if (color_output) buffer << (it.get_status() ? GREEN : RED);
  buffer << (it.get_status() ? "ok" : "not ok");
  if (color_output) buffer << RESET;
  buffer << " " << get_and_increment_test_counter() << " - " << description
         << std::endl;
  if (not failure_messages.empty()) {
    for (auto failure : failure_messages) buffer << failure;
    failure_messages.clear();
  }
}

inline void TAP::format(std::string message) { buffer << message << std::endl; }

inline void TAP::format_failure(std::string message) {
  std::ostringstream oss;
  std::istringstream iss(message);
  std::string line;
  while (std::getline(iss, line)) {
    oss << "    " << line << std::endl;
  }
  failure_messages.push_back(oss.str());
}

inline void TAP::flush() {
  std::string str = buffer.str();
  std::ostringstream oss;
  if (str[0] == '\n') {
    oss << str.substr(0, 1) << GREEN << "1.." << test_counter - 1 << RESET
        << std::endl
        << str.substr(1);
  } else {
    oss << GREEN << "1.." << test_counter - 1 << RESET << std::endl << str;
  }
  std::cout << oss.str() << std::flush;
  first = false;
  test_counter = 1;
  buffer = std::ostringstream();
}

static TAP tap;

}  // namespace Formatters
}  // namespace CppSpec
#endif  // CPPSPEC_FORMATTERS_TAP_HPP
#line 9 "/mnt/c/Users/Kate/GitHub/cppspec/include/runner.hpp"

namespace CppSpec {

class Runner {
  std::list<Description *> specs;
  Formatters::BaseFormatter &formatter;

 public:
  typedef std::function<void(Runner &)> spec_group;
  explicit Runner(Formatters::BaseFormatter &formatter =
                      Formatters::progress) noexcept : formatter(formatter) {}

  template <typename T>
  Runner &add_spec(ClassDescription<T> &spec);
  Runner &add_spec(Description &spec);
  Result run(Formatters::BaseFormatter &printer);
  Result exec();
};

template <typename T>
Runner &Runner::add_spec(ClassDescription<T> &spec) {
  specs.push_back(&spec);
  return *this;
}

inline Runner &Runner::add_spec(Description &spec) {
  specs.push_back(&spec);
  return *this;
}

inline Result Runner::run(Formatters::BaseFormatter &formatter) {
  bool success = true;
  for (auto spec : specs) {
    success &= static_cast<bool>(spec->run(formatter));
  }
  return success ? Result::success() : Result::failure();
}

inline Result Runner::exec() {
  formatter.set_multiple(true);
  Result result = this->run(formatter);
  formatter.cleanup();
  return result;
}

}  // namespace CppSpec
#endif  // CPPSPEC_RUNNER_HPP
#line 9 "/mnt/c/Users/Kate/GitHub/cppspec/include/cppspec.hpp"

/*>>>>>>>>>>>>>>>>>>>> MACROS <<<<<<<<<<<<<<<<<<<<<<*/

#define $ [](auto &self) -> void
#define _ [=](auto &self) mutable -> void

#define it self.it
#define specify it
#ifdef _MSC_VER  // Apparently MSVC++ doesn't conform to C++14 14.2/4. Annoying.
#define context self.context
#define expect self.expect
#else
#define context self.template context
#define expect self.template expect
#endif
#define explain context  // Piggybacks off of the `context` macro

#define is_expected self.is_expected
#define subject self.subject

#define before_all self.before_all
#define before_each self.before_each
#define after_all self.after_all
#define after_each self.after_each
#define let(name, body) auto name = self.let(body);

/*>>>>>>>>>>>>>>>>>>> TYPEDEFS <<<<<<<<<<<<<<<<<<<<<*/

typedef CppSpec::Description describe;

template <class T>
using describe_a = CppSpec::ClassDescription<T>;

template <class T>
using describe_an = CppSpec::ClassDescription<T>;

#endif  // CPPSPEC_HPP
