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

            struct make_and_mask_fn
            {
                template<typename... Msks>
                auto operator()(Msks&&... msks) const
                {
                    CONCEPT_ASSERT((Range<Msks>() || ...));
                    return ranges::view::zip(std::forward<Msks>(msks) ...) |
                           ranges::view::transform(
                               [](auto&& range_item) -> bool {
                                   return tuple_and(range_item);
                               });
                }
                template<typename Msk1, typename Msk2>
                auto operator()(Msk1&& msk1, Msk2&& msk2) const
                {
                    CONCEPT_ASSERT(Range<Msk1>());
                    CONCEPT_ASSERT(Range<Msk2>());
                    return ranges::view::zip(std::forward<Msk1>(msk1),
                                             std::forward<Msk2>(msk2)) |
                           ranges::view::transform(
                               [](auto&& range_item) -> bool {
                                   return range_item.first && range_item.second;
                               });
                }

            private:
                template<typename... T>
                static bool variable_length_and(const T... v)
                {
                    return (v && ...);
                }
                template<typename... T, std::size_t... Idx>
                static bool tuple_and(const std::tuple<T...> t, std::index_sequence<Idx...>)
                {
                    return variable_length_and(std::get<Idx>(t)...);
                }
                template<typename... T>
                static bool tuple_and(const std::tuple<T...> t)
                {
                    return tuple_and(t, std::index_sequence_for<T...>{});
                }
            };
            RANGES_INLINE_VARIABLE(make_and_mask_fn, make_and_masker)

            struct make_or_mask_fn
            {
                template<typename... Msks>
                auto operator()(Msks&&... msks) const
                {
                    CONCEPT_ASSERT((Range<Msks>() || ...));
                    return ranges::view::zip(std::forward<Msks>(msks) ...) |
                           ranges::view::transform(
                               [](auto&& range_item) -> bool {
                                   return tuple_or(range_item);
                               });
                }
                template<typename Msk1, typename Msk2>
                auto operator()(Msk1&& msk1, Msk2&& msk2) const
                {
                    CONCEPT_ASSERT(Range<Msk1>());
                    CONCEPT_ASSERT(Range<Msk2>());
                    return ranges::view::zip(std::forward<Msk1>(msk1),
                                             std::forward<Msk2>(msk2)) |
                           ranges::view::transform(
                               [](auto&& range_item) -> bool {
                                   return range_item.first || range_item.second;
                               });
                }

            private:
                template<typename... T>
                static bool variable_length_or(const T... v)
                {
                    return (v || ...);
                }
                template<typename... T, std::size_t... Idx>
                static bool tuple_or(const std::tuple<T...> t, std::index_sequence<Idx...>)
                {
                    return variable_length_or(std::get<Idx>(t)...);
                }
                template<typename... T>
                static bool tuple_or(const std::tuple<T...> t)
                {
                    return tuple_or(t, std::index_sequence_for<T...>{});
                }
            };
            RANGES_INLINE_VARIABLE(make_or_mask_fn, make_or_masker)

            struct apply_or_mask_fn
            {
                template<typename Rng, typename Msk1, typename Msk2>
                auto operator()(Rng&& rng, Msk1&& msk1, Msk2&& msk2) const
                {
                    CONCEPT_ASSERT(Range<Rng>());
                    CONCEPT_ASSERT(Range<Msk1>());
                    CONCEPT_ASSERT(Range<Msk2>());
                    return ranges::view::zip(std::forward<Rng>(rng),
                                             std::forward<Msk1>(msk1),
                                             std::forward<Msk2>(msk2)) |
                           ranges::view::filter([](auto&& range_item) -> bool {
                               return std::get<1>(range_item) ||
                                      std::get<2>(range_item);
                           }) |
                           ranges::view::transform(
                               [](auto&& range_item) -> decltype(auto) {
                                   return std::get<0>(range_item);
                               });
                }
                template<typename Msk1, typename Msk2>
                auto operator()(Msk1&& msk1, Msk2&& msk2) const
                {
                    CONCEPT_ASSERT(Range<Msk1>());
                    CONCEPT_ASSERT(Range<Msk2>());
                    return make_pipeable(
                        std::bind(*this,
                                  std::placeholders::_1,
                                  ranges::view::all(std::forward<Msk1>(msk1)),
                                  ranges::view::all(std::forward<Msk2>(msk2))));
                }
            };
            RANGES_INLINE_VARIABLE(apply_or_mask_fn, apply_or_masker)

            struct and_mask_fn
            {
                template<typename Rng, typename Msk1, typename Msk2>
                auto operator()(Rng&& rng, Msk1&& msk1, Msk2&& msk2) const
                {
                    CONCEPT_ASSERT(Range<Rng>());
                    CONCEPT_ASSERT(Range<Msk1>());
                    CONCEPT_ASSERT(Range<Msk2>());
                    return ranges::view::zip(std::forward<Rng>(rng),
                                             std::forward<Msk1>(msk1),
                                             std::forward<Msk2>(msk2)) |
                           ranges::view::filter([](auto&& range_item) -> bool {
                               return std::get<1>(range_item) &&
                                      std::get<2>(range_item);
                           }) |
                           ranges::view::transform(
                               [](auto&& range_item) -> decltype(auto) {
                                   return std::get<0>(range_item);
                               });
                }
                template<typename Msk1, typename Msk2>
                auto operator()(Msk1&& msk1, Msk2&& msk2) const
                {
                    CONCEPT_ASSERT(Range<Msk1>());
                    CONCEPT_ASSERT(Range<Msk2>());
                    return make_pipeable(
                        std::bind(*this,
                                  std::placeholders::_1,
                                  ranges::view::all(std::forward<Msk1>(msk1)),
                                  ranges::view::all(std::forward<Msk2>(msk2))));
                }
            };
            RANGES_INLINE_VARIABLE(and_mask_fn, and_masker)
        }
    }
}

#endif
