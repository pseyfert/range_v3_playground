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

// TODO: clean up includes
// TODO: add comments for doc-gen
// TODO: add RANGES_DECLTYPE_AUTO_RETURN for "old" c++ standards
// TODO: check for reference correctness

#include <boost/type_index.hpp>
#include <iostream>

#include <range/v3/range_fwd.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/static_const.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

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
                    // std::cout << "received" << std::endl;
                    // for(auto i : msk)
                    //     std::cout << i << std::endl;
                    CONCEPT_ASSERT(Range<Rng>());
                    CONCEPT_ASSERT(Range<Msk>());
                    for(auto t : ranges::view::zip(rng, msk))
                        std::cout << "w: " << t.first << "\tf: " << t.second
                                  << std::endl;
                    for(auto t :
                        ranges::view::zip(rng, msk) |
                            ranges::view::filter([](auto&& range_item) -> bool {
                                return range_item.second;
                            }))
                        std::cout << "w: " << t.first << "\tf: " << t.second
                                  << std::endl;
                    for(auto t :
                        ranges::view::zip(rng, msk) |
                            ranges::view::filter([](auto&& range_item) -> bool {
                                return range_item.second;
                            }) |
                            ranges::view::transform(
                                [](auto&& range_item) -> decltype(auto) {
                                    return range_item.first;
                                }))
                        std::cout << "w: " << t << std::endl;
                    return ranges::view::zip(rng, msk) |
                           ranges::view::filter([](auto&& range_item) -> bool {
                               std::cout << "checking widget "
                                         << range_item.first << std::endl;
                               std::cout << "returning " << range_item.second
                                         << std::endl;
                               return range_item.second;
                           }) |
                           ranges::view::transform(
                               [](auto&& range_item) -> decltype(auto) {
                                   std::cout << "here passing "
                                             << range_item.first << std::endl;
                                   return range_item.first;
                               });
                }
                template<typename Msk>
                auto operator()(Msk&& msk) const -> decltype(
                    make_pipeable(std::bind(*this, std::placeholders::_1,
                                            protect(std::forward<Msk>(msk)))))
                {
                    CONCEPT_ASSERT(Range<Msk>());
                    return make_pipeable(
                        std::bind(*this,
                                  std::placeholders::_1,
                                  protect(std::forward<Msk>(msk))));
                }
            };

            RANGES_INLINE_VARIABLE(mask_fn, masker)
        }
    }
}

#endif
