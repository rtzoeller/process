// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008 Ilya Sokolov, Boris Schaeling
// Copyright (c) 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
// Copyright (c) 2011, 2012 Jeff Flinn, Boris Schaeling
// Copyright (c) 2016 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROCESS_WINDOWS_INITIALIZERS_SET_CMD_LINE_HPP
#define BOOST_PROCESS_WINDOWS_INITIALIZERS_SET_CMD_LINE_HPP

#include <boost/detail/winapi/config.hpp>
#include <boost/process/detail/initializers/handler_base.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/shared_array.hpp>
#include <memory>

namespace boost { namespace process { namespace windows { namespace initializers {

template <class String>
struct set_cmd_line_ : ::boost::process::detail::initializers::handler_base
{
    typedef String string_type;
    typedef typename String::value_type char_type;

    explicit set_cmd_line_(const String &s)
        : cmd_line_(new char_type[s.size() + 1])
    {
        boost::copy(s, cmd_line_.get());
        cmd_line_[s.size()] = 0;
    }

    template <class WindowsExecutor>
    void on_setup(WindowsExecutor &e) const
    {
        e.cmd_line = cmd_line_.get();
    }

private:
    boost::shared_array<char_type> cmd_line_;
};

struct cmd_
{
    inline set_cmd_line_<std::wstring> operator()(const wchar_t *ws) const
    {
        return set_cmd_line_<std::wstring>(ws);
    }
    inline set_cmd_line_<std::wstring> operator= (const wchar_t *ws) const
    {
        return set_cmd_line_<std::wstring>(ws);
    }

    inline set_cmd_line_<std::wstring> operator()(const std::wstring &ws) const
    {
        return set_cmd_line_<std::wstring>(ws);
    }
    inline set_cmd_line_<std::wstring> operator= (const std::wstring &ws) const
    {
        return set_cmd_line_<std::wstring>(ws);
    }
    #if !defined( BOOST_NO_ANSI_APIS )
    inline set_cmd_line_<std::string> operator()(const char *s) const
    {
        return set_cmd_line_<std::string>(s);
    }
    inline set_cmd_line_<std::string> operator= (const char *s) const
    {
        return set_cmd_line_<std::string>(s);
    }

    inline set_cmd_line_<std::string> operator()(const std::string &s) const
    {
        return set_cmd_line_<std::string>(s);
    }
    inline set_cmd_line_<std::string> operator= (const std::string &s) const
    {
        return set_cmd_line_<std::string>(s);
    }
    #endif //BOOST_NO_ANSI_APIS
    };

constexpr static cmd_ cmd;
constexpr static cmd_ set_cmd;

}}}}

#endif
