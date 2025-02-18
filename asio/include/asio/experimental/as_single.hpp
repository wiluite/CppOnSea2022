//
// experimental/as_single.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_EXPERIMENTAL_AS_SINGLE_HPP
#define ASIO_EXPERIMENTAL_AS_SINGLE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/detail/type_traits.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace experimental {

/// A @ref completion_token adapter used to specify that the completion handler
/// arguments should be combined into a single argument.
/**
 * The as_single_t class is used to indicate that any arguments to the
 * completion handler should be combined and passed as a single argument.
 * If there is already one argument, that argument is passed as-is. If
 * there is more than argument, the arguments are first moved into a
 * @c std::tuple and that tuple is then passed to the completion handler.
 */
template <typename CompletionToken>
class as_single_t
{
public:
  /// Tag type used to prevent the "default" constructor from being used for
  /// conversions.
  struct default_constructor_tag {};

  /// Default constructor.
  /**
   * This constructor is only valid if the underlying completion token is
   * default constructible and move constructible. The underlying completion
   * token is itself defaulted as an argument to allow it to capture a source
   * location.
   */
  ASIO_CONSTEXPR as_single_t(
      default_constructor_tag = default_constructor_tag(),
      CompletionToken token = CompletionToken())
    : token_(ASIO_MOVE_CAST(CompletionToken)(token))
  {
  }

  /// Constructor.
  template <typename T>
  ASIO_CONSTEXPR explicit as_single_t(
      ASIO_MOVE_ARG(T) completion_token)
    : token_(ASIO_MOVE_CAST(T)(completion_token))
  {
  }

  /// Adapts an executor to add the @c as_single_t completion token as the
  /// default.
  template <typename InnerExecutor>
  struct executor_with_default : InnerExecutor
  {
    /// Specify @c as_single_t as the default completion token type.
    typedef as_single_t default_completion_token_type;

    /// Construct the adapted executor from the inner executor type.
    executor_with_default(const InnerExecutor& ex) ASIO_NOEXCEPT
      : InnerExecutor(ex)
    {
    }

    /// Convert the specified executor to the inner executor type, then use
    /// that to construct the adapted executor.
    template <typename OtherExecutor>
    executor_with_default(const OtherExecutor& ex,
        typename constraint<
          is_convertible<OtherExecutor, InnerExecutor>::value
        >::type = 0) ASIO_NOEXCEPT
      : InnerExecutor(ex)
    {
    }
  };

  /// Type alias to adapt an I/O object to use @c as_single_t as its
  /// default completion token type.
#if defined(ASIO_HAS_ALIAS_TEMPLATES) \
  || defined(GENERATING_DOCUMENTATION)
  template <typename T>
  using as_default_on_t = typename T::template rebind_executor<
      executor_with_default<typename T::executor_type> >::other;
#endif // defined(ASIO_HAS_ALIAS_TEMPLATES)
       //   || defined(GENERATING_DOCUMENTATION)

  /// Function helper to adapt an I/O object to use @c as_single_t as its
  /// default completion token type.
  template <typename T>
  static typename decay_t<T>::template rebind_executor<
      executor_with_default<typename decay<T>::type::executor_type>
    >::other
  as_default_on(ASIO_MOVE_ARG(T) object)
  {
    return typename decay_t<T>::template rebind_executor<
        executor_with_default<typename decay<T>::type::executor_type>
      >::other(ASIO_MOVE_CAST(T)(object));
  }

//private:
  CompletionToken token_;
};

/// Adapt a @ref completion_token to specify that the completion handler
/// arguments should be combined into a single argument.
template <typename CompletionToken>
ASIO_NODISCARD inline
ASIO_CONSTEXPR as_single_t<typename decay<CompletionToken>::type>
as_single(ASIO_MOVE_ARG(CompletionToken) completion_token)
{
  return as_single_t<typename decay<CompletionToken>::type>(
      ASIO_MOVE_CAST(CompletionToken)(completion_token));
}

} // namespace experimental
} // namespace asio

#include "asio/detail/pop_options.hpp"

#include "asio/experimental/impl/as_single.hpp"

#endif // ASIO_EXPERIMENTAL_AS_SINGLE_HPP
