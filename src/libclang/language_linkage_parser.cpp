// Copyright (C) 2017-2022 Jonathan Müller and cppast contributors
// SPDX-License-Identifier: MIT

#include "parse_functions.hpp"

#include <clang-c/Index.h>
#include <cppast/cpp_language_linkage.hpp>

#include "libclang_visitor.hpp"

using namespace cppast;

std::unique_ptr<cpp_entity> detail::try_parse_cpp_language_linkage(const parse_context& context,
                                                                   const CXCursor&      cur)
{
    DEBUG_ASSERT(cur.kind == CXCursor_UnexposedDecl,
                 detail::assert_handler{}); // not exposed currently

    detail::cxtokenizer    tokenizer(context.tu, context.file, cur);
    detail::cxtoken_stream stream(tokenizer, cur);

    // extern <name> ...
    if (!detail::skip_if(stream, "extern"))
        return nullptr;
    // unexposed variable starting with extern - must be a language linkage
    // (function, variables are not unexposed)
    auto& name = stream.get().value();

    auto builder = cpp_language_linkage::builder(name.c_str());
    context.comments.match(builder.get(), cur);

    auto cb = [&](const CXCursor& child) {
        auto entity = parse_entity(context, &builder.get(), child);
        if (entity)
            builder.add_child(std::move(entity));
    };

    if ( context.keepEntitiesInIncludes )
        detail::visit_children(cur, cb );
    else
        detail::visit_children_checkfile(cur, context.path, cb );

    return builder.finish();
}
