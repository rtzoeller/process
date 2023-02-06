// Copyright (c) 2022 Klemens D. Morgenstern
// Copyright (c) 2022 Samuel Venable
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROCESS_V2_ENV_HPP
#define BOOST_PROCESS_V2_ENV_HPP
#include <string>
#include <vector>

#include <boost/process/v2/detail/config.hpp>
#include <boost/process/v2/detail/throw_error.hpp>
#include <boost/process/v2/process_handle.hpp>
#include <boost/process/v2/pid.hpp>

#include <boost/process/v2/environment.hpp>

BOOST_PROCESS_V2_BEGIN_NAMESPACE

namespace detail
{
namespace ext
{

#if defined(BOOST_PROCESS_V2_WINDOWS)
using native_env_handle_type = wchar_t *;
using native_env_iterator = wchar_t *;
#else
using native_env_handle_type = char *;
using native_env_iterator = char *;
#endif

struct native_env_handle_deleter
{
    BOOST_PROCESS_V2_DECL void operator()(native_env_handle_type) const;
};

BOOST_PROCESS_V2_DECL native_env_iterator next(native_env_iterator nh);
BOOST_PROCESS_V2_DECL native_env_iterator find_end(native_env_iterator nh);
BOOST_PROCESS_V2_DECL const environment::char_type * dereference(native_env_iterator iterator);

} // namespace ext
} // namespace detail

namespace ext {

struct env_view
{
    using native_handle_type = detail::ext::native_env_handle_type;
    using value_type = environment::key_value_pair_view;

    env_view() = default;
    env_view(env_view && nt) = default;

    native_handle_type native_handle() { return handle_.get(); }


    struct iterator
    {
        using value_type        = environment::key_value_pair_view;
        using difference_type   = int;
        using reference         = environment::key_value_pair_view;
        using pointer           = environment::key_value_pair_view;
        using iterator_category = std::forward_iterator_tag;

        iterator() = default;
        iterator(const iterator & ) = default;
        iterator(const detail::ext::native_env_iterator &native_handle) : iterator_(native_handle) {}

        iterator & operator++()
        {
            iterator_ = detail::ext::next(iterator_);
            return *this;
        }

        iterator operator++(int)
        {
            auto last = *this;
            iterator_ = detail::ext::next(iterator_);
            return last;
        }
        environment::key_value_pair_view operator*() const
        {
            return detail::ext::dereference(iterator_);
        }

        friend bool operator==(const iterator & l, const iterator & r) {return l.iterator_ == r.iterator_;}
        friend bool operator!=(const iterator & l, const iterator & r) {return l.iterator_ != r.iterator_;}

      private:
        detail::ext::native_env_iterator iterator_;
    };

    iterator begin() const {return iterator(handle_.get());}
    iterator   end() const {return iterator(detail::ext::find_end(handle_.get()));}

  private:
    friend BOOST_PROCESS_V2_DECL env_view env(pid_type pid, error_code & ec);
    #if defined(BOOST_PROCESS_V2_WINDOWS)
    friend BOOST_PROCESS_V2_DECL env_view env(HANDLE handle, error_code & ec);
    #endif

    std::unique_ptr<typename remove_pointer<detail::ext::native_env_handle_type>::type,
            detail::ext::native_env_handle_deleter> handle_;
};

/// Get the argument vector from a given pid
BOOST_PROCESS_V2_DECL env_view env(pid_type pid, error_code & ec);
BOOST_PROCESS_V2_DECL env_view env(pid_type pid);

#if defined(BOOST_PROCESS_V2_WINDOWS)
BOOST_PROCESS_V2_DECL env_view env(HANDLE handle, error_code & ec);
BOOST_PROCESS_V2_DECL env_view env(HANDLE handle);
#endif

template<typename Executor>
BOOST_PROCESS_V2_DECL env_view env(basic_process_handle<Executor> & handle, error_code & ec)
{
    return env(handle.native_handle(), ec);
}

template<typename Executor>
BOOST_PROCESS_V2_DECL env_view env(basic_process_handle<Executor> & handle)
{
    return env(handle.native_handle());
}

} // namespace ext

BOOST_PROCESS_V2_END_NAMESPACE

#if defined(BOOST_PROCESS_V2_HEADER_ONLY)

#include <boost/process/v2/ext/impl/env.ipp>

#endif
#endif // BOOST_PROCESS_V2_ENV_HPP
