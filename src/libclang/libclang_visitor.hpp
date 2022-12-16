// Copyright (C) 2017-2022 Jonathan Müller and cppast contributors
// SPDX-License-Identifier: MIT

#ifndef CPPAST_LIBCLANG_VISITOR_HPP_INCLUDED
#define CPPAST_LIBCLANG_VISITOR_HPP_INCLUDED

#include <clang-c/Index.h>

#include "raii_wrapper.hpp"

namespace cppast
{
namespace detail
{
    // visits direct children of an entity
    // notes: only visits if directly defined in file, not included
    template <typename Func>
    void visit_children_checkfile(CXCursor parent, const std::string& path, Func f, bool recurse = false)
    {
        auto call_cb_if_in_file = [&](const CXCursor& cur) {
            auto location = clang_getCursorLocation(cur);

            CXString cx_file_name;
            clang_getPresumedLocation(location, &cx_file_name, nullptr, nullptr);
            cxstring file_name(cx_file_name);

            if ( file_name != path.c_str() )
                return;

            f( cur );
        };

        auto continue_lambda = [](CXCursor cur, CXCursor, CXClientData data) {
            auto& actual_cb = *static_cast<decltype(call_cb_if_in_file)*>(data);
            actual_cb( cur );
            return CXChildVisit_Continue;
        };
        auto recurse_lambda = [](CXCursor cur, CXCursor, CXClientData data) {
            auto& actual_cb = *static_cast<decltype(call_cb_if_in_file)*>(data);
            actual_cb( cur );
            return CXChildVisit_Recurse;
        };

        if (recurse)
            clang_visitChildren(parent, recurse_lambda, &call_cb_if_in_file);
        else
            clang_visitChildren(parent, continue_lambda, &call_cb_if_in_file);
    }

    template <typename Func>
    void visit_children(CXCursor parent, Func f, bool recurse = false)
    {
        auto continue_lambda = [](CXCursor cur, CXCursor, CXClientData data) {
            auto& actual_cb = *static_cast<Func*>(data);
            actual_cb(cur);
            return CXChildVisit_Continue;
        };
        auto recurse_lambda = [](CXCursor cur, CXCursor, CXClientData data) {
            auto& actual_cb = *static_cast<Func*>(data);
            actual_cb(cur);
            return CXChildVisit_Recurse;
        };

        if (recurse)
            clang_visitChildren(parent, recurse_lambda, &f);
        else
            clang_visitChildren(parent, continue_lambda, &f);
    }
} // namespace detail
} // namespace cppast

#endif // CPPAST_LIBCLANG_VISITOR_HPP_INCLUDED
