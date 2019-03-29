/// \file
// Range v3 library
//
//  Copyright CERN 2018-present
//
//  In applying this licence, CERN does not waive the privileges and immunities
//  granted to it by virtue of its status as an Intergovernmental Organization
//  or submit itself to any jurisdiction.
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
#include <range/v3/view/any_view.hpp>
#include <range/v3/view/filter.hpp> // for filter
#include <range/v3/view/slice.hpp>
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
                    return ranges::view::zip(std::forward<Msks>(msks)...) |
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
                static bool tuple_and(const std::tuple<T...> t,
                                      std::index_sequence<Idx...>)
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

            struct apply_and_mask_fn
            {
                template<typename... Msks>
                auto operator()(Msks&&... msks) const
                {
                    CONCEPT_ASSERT((Range<Msks>() || ...));
                    return masker(make_and_masker(msks...));
                }
            };
            RANGES_INLINE_VARIABLE(apply_and_mask_fn, apply_and_masker)

            struct make_or_mask_fn
            {
                template<typename... Msks>
                auto operator()(Msks&&... msks) const
                {
                    CONCEPT_ASSERT((Range<Msks>() || ...));
                    return ranges::view::zip(std::forward<Msks>(msks)...) |
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
                static bool tuple_or(const std::tuple<T...> t,
                                     std::index_sequence<Idx...>)
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

            struct make_bool_vector_or_mask_fn
            {
            private:
                template<typename Msk1, typename Msk2>
                static auto or_ranges(Msk1&& msk1, Msk2&& msk2)
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
                template<typename... Msks>
                static auto or_ranges(Msks&&... msks)
                {
                    CONCEPT_ASSERT((Range<Msks>() || ...));
                    return ranges::view::zip(std::forward<Msks>(msks)...) |
                           ranges::view::transform(
                               [](auto&& range_item) -> bool {
                                   return tuple_or(range_item);
                               });
                }
                template<typename... T>
                static bool variable_length_or(const T... v)
                {
                    return (v || ...);
                }
                template<typename... T, std::size_t... Idx>
                static bool tuple_or(const std::tuple<T...> t,
                                     std::index_sequence<Idx...>)
                {
                    return variable_length_or(std::get<Idx>(t)...);
                }
                template<typename... T>
                static bool tuple_or(const std::tuple<T...> t)
                {
                    return tuple_or(t, std::index_sequence_for<T...>{});
                }

            public:
                template<typename Msks_>
                ranges::v3::any_view<bool> shortlist(
                    Msks_&& msks) const // TODO const and reference and stuff
                // precondition msks is size 1..8
                {
                    using Msks = decltype(msks);
                    CONCEPT_ASSERT(ForwardRange<Msks>());

                    // // todo return range with all true (an or of zero
                    // elements
                    // // is true)
                    if(msks.size() == 1)
                        return msks[0];
                    if(msks.size() == 2)
                        return or_ranges(msks[0], msks[1]);
                    if(msks.size() == 3)
                        return or_ranges(msks[0], msks[1], msks[2]);
                    if(msks.size() == 4)
                        return or_ranges(msks[0], msks[1], msks[2], msks[3]);
                    if(msks.size() == 5)
                        return or_ranges(
                            msks[0], msks[1], msks[2], msks[3], msks[4]);
                    if(msks.size() == 6)
                        return or_ranges(msks[0],
                                         msks[1],
                                         msks[2],
                                         msks[3],
                                         msks[4],
                                         msks[5]);
                    if(msks.size() == 7)
                        return or_ranges(msks[0],
                                         msks[1],
                                         msks[2],
                                         msks[3],
                                         msks[4],
                                         msks[5],
                                         msks[6]);
                    return or_ranges(msks[0],
                                     msks[1],
                                     msks[2],
                                     msks[3],
                                     msks[4],
                                     msks[5],
                                     msks[6],
                                     msks[7]);
                }
                template<typename Msks_>
                ranges::v3::any_view<bool> operator()(
                    Msks_&& msks) const // TODO const and reference and stuff
                {
                    using Msks = decltype(msks);
                    CONCEPT_ASSERT(ForwardRange<Msks>());

                    // // todo return range with all true (an or of zero
                    // elements
                    // // is true)
                    assert(msks.size() != 0);
                    if(msks.size() <= 8)
                    {
                        return shortlist(std::move(msks));
                    }

                    size_t low = ((msks.size() + 5) % 7) + 2;

                    // below the long version
                    // if(msks.size() % 7 == 0)
                    // {
                    //     low = 7;
                    // }
                    // else if(msks.size() % 7 == 1)
                    // {
                    //     low = 8;
                    // }
                    // else
                    // {
                    //     low = msks.size() % 7;
                    // }
                    ranges::v3::any_view<bool> start =
                        shortlist(ranges::view::slice(msks, 0, low));
                    for(; low + 6 < msks.size(); low += 7)
                    {
                        start = or_ranges(start,
                                          msks[low + 0],
                                          msks[low + 1],
                                          msks[low + 2],
                                          msks[low + 3],
                                          msks[low + 4],
                                          msks[low + 5],
                                          msks[low + 6]);
                    }
                    assert(low == msks.size());
                    return start;
                }
            };
            RANGES_INLINE_VARIABLE(make_bool_vector_or_mask_fn,
                                   make_bool_vector_or_masker)

            struct make_int_vector_or_mask_fn
            {
            private:
                template<typename Msk1, typename Msk2>
                static auto or_ranges(Msk1&& msk1, Msk2&& msk2)
                {
                    CONCEPT_ASSERT(Range<Msk1>());
                    CONCEPT_ASSERT(Range<Msk2>());
                    return ranges::view::zip(std::forward<Msk1>(msk1),
                                             std::forward<Msk2>(msk2)) |
                           ranges::view::transform(
                               [](auto&& range_item) -> int {
                                   return range_item.first || range_item.second;
                               });
                }
                template<typename... Msks>
                static auto or_ranges(Msks&&... msks)
                {
                    CONCEPT_ASSERT((Range<Msks>() || ...));
                    return ranges::view::zip(std::forward<Msks>(msks)...) |
                           ranges::view::transform(
                               [](auto&& range_item) -> int {
                                   return tuple_or(range_item);
                               });
                }
                template<typename... T>
                static int variable_length_or(const T... v)
                {
                    return (v || ...);
                }
                template<typename... T, std::size_t... Idx>
                static int tuple_or(const std::tuple<T...> t,
                                    std::index_sequence<Idx...>)
                {
                    return variable_length_or(std::get<Idx>(t)...);
                }
                template<typename... T>
                static int tuple_or(const std::tuple<T...> t)
                {
                    return tuple_or(t, std::index_sequence_for<T...>{});
                }

            public:
                template<typename Msks_>
                ranges::v3::any_view<int> shortlist(
                    Msks_&& msks) const // TODO const and reference and stuff
                // precondition msks is size 1..8
                {
                    using Msks = decltype(msks);
                    CONCEPT_ASSERT(ForwardRange<Msks>());

                    // // todo return range with all true (an or of zero
                    // elements
                    // // is true)
                    if(msks.size() == 1)
                        return msks[0];
                    if(msks.size() == 2)
                        return or_ranges(msks[0], msks[1]);
                    if(msks.size() == 3)
                        return or_ranges(msks[0], msks[1], msks[2]);
                    if(msks.size() == 4)
                        return or_ranges(msks[0], msks[1], msks[2], msks[3]);
                    if(msks.size() == 5)
                        return or_ranges(
                            msks[0], msks[1], msks[2], msks[3], msks[4]);
                    if(msks.size() == 6)
                        return or_ranges(msks[0],
                                         msks[1],
                                         msks[2],
                                         msks[3],
                                         msks[4],
                                         msks[5]);
                    if(msks.size() == 7)
                        return or_ranges(msks[0],
                                         msks[1],
                                         msks[2],
                                         msks[3],
                                         msks[4],
                                         msks[5],
                                         msks[6]);
                    return or_ranges(msks[0],
                                     msks[1],
                                     msks[2],
                                     msks[3],
                                     msks[4],
                                     msks[5],
                                     msks[6],
                                     msks[7]);
                }
                template<typename Msks_>
                ranges::v3::any_view<int> operator()(
                    Msks_&& msks) const // TODO const and reference and stuff
                {
                    using Msks = decltype(msks);
                    CONCEPT_ASSERT(ForwardRange<Msks>());

                    // // todo return range with all true (an or of zero
                    // elements
                    // // is true)
                    assert(msks.size() != 0);
                    if(msks.size() <= 8)
                    {
                        return shortlist(std::move(msks));
                    }

                    size_t low = ((msks.size() + 5) % 7) + 2;

                    // below the long version
                    // if(msks.size() % 7 == 0)
                    // {
                    //     low = 7;
                    // }
                    // else if(msks.size() % 7 == 1)
                    // {
                    //     low = 8;
                    // }
                    // else
                    // {
                    //     low = msks.size() % 7;
                    // }
                    ranges::v3::any_view<int> start =
                        shortlist(ranges::view::slice(msks, 0, low));
                    for(; low + 6 < msks.size(); low += 7)
                    {
                        start = or_ranges(start,
                                          msks[low + 0],
                                          msks[low + 1],
                                          msks[low + 2],
                                          msks[low + 3],
                                          msks[low + 4],
                                          msks[low + 5],
                                          msks[low + 6]);
                    }
                    assert(low == msks.size());
                    return start;
                }
            };
            RANGES_INLINE_VARIABLE(make_int_vector_or_mask_fn,
                                   make_int_vector_or_masker)

            struct apply_or_mask_fn
            {
                template<typename... Msks>
                auto operator()(Msks&&... msks) const
                {
                    CONCEPT_ASSERT((Range<Msks>() || ...));
                    return masker(make_or_masker(msks...));
                }
            };
            RANGES_INLINE_VARIABLE(apply_or_mask_fn, apply_or_masker)

            struct or_mask_fn
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
            RANGES_INLINE_VARIABLE(or_mask_fn, or_masker)

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
