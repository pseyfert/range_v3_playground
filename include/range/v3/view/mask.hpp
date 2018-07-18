/// \file
// Range v3 library
//
//  Copyright CERN 2018-present
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef RANGES_V3_VIEW_MASK_HPP
#define RANGES_V3_VIEW_MASK_HPP

// TODO: add comments for doc-gen
// TODO: add RANGES_DECLTYPE_AUTO_RETURN for "old" c++ standards

#include <functional> // for _1, bind
#include <range/v3/detail/config.hpp> // for RANGES_INLINE_VARIABLE
#include <range/v3/range_concepts.hpp> // for Range
#include <range/v3/utility/concepts.hpp> // for CONCEPT_ASSERT
#include <range/v3/utility/functional.hpp> // for operator|, make_pipeable
#include <range/v3/view/all.hpp> // for all
#include <range/v3/view/filter.hpp> // for filter
#include <range/v3/view/transform.hpp> // for transform
#include <range/v3/view/zip.hpp> // for zip
#include <utility> // for forward

namespace ranges
{
    inline namespace v3
    {
        namespace view
        {
            struct mask_fn
            {
                template<typename Rng, typename Msk>
                auto operator()(Rng&& rng, Msk&& msk) const
                {
                    CONCEPT_ASSERT(Range<Rng>());
                    CONCEPT_ASSERT(Range<Msk>());
                    return ranges::view::zip(std::forward<Rng>(rng),
                                             std::forward<Msk>(msk)) |
                           ranges::view::filter([](auto&& range_item) -> bool {
                               return range_item.second;
                           }) |
                           ranges::view::transform(
                               [](auto&& range_item) -> decltype(auto) {
                                   return range_item.first;
                               });
                }
                template<typename Msk>
                auto operator()(Msk&& msk) const
                {
                    CONCEPT_ASSERT(Range<Msk>());
                    return make_pipeable(
                        std::bind(*this,
                                  std::placeholders::_1,
                                  ranges::view::all(std::forward<Msk>(msk))));
                }
            };
            RANGES_INLINE_VARIABLE(mask_fn, masker)
        }
    }
}

#endif
