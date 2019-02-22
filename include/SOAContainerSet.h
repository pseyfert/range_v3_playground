/*
 * Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
 * Author: Paul Seyfert <pseyfert@cern.ch>
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

#ifndef SOAContainerSet_h
#define SOAContainerSet_h
#include "IncompatibleZipException.h" // for IncompatibleZipException
#include "SOASkin.h"                  // for is_skin
#include "SOAUtils.h"                 // for ALL, is_view
#include "SOAView.h"                  // for zip
#include "ZipTraits.h"
#include "ZipUtils.h"
#include <cassert>     // for assert
#include <type_traits> // for remove_reference_t, enable_if_t
#include <utility>     // for forward

template <typename FIRST, typename... OTHERS>
int firstid(FIRST& first, OTHERS&... /*unused*/)
{
    return first.ZipIdentifier();
}

template <typename CONTAINER>
class ZipContainer : public CONTAINER {
public:
    using view = CONTAINER;

    template <typename... Args>
    ZipContainer(int ID, Args&&... args)
            : CONTAINER(args...), m_identifier(ID)
    {}

    template <typename T>
    auto push_back(T&& t) -> std::enable_if_t<
            view::self_type::fields_typelist::size() == 1 &&
            std::is_same_v<std::remove_reference_t<T>,
                           typename view::self_type::fields_typelist::
                                   template at<0>::type::type>>
    {
        CONTAINER::emplace_back(std::forward<T>(t));
    }
    template <typename T>
    auto push_back(const T& t) -> std::enable_if_t<
            view::self_type::fields_typelist::size() == 1 &&
            std::is_same_v<std::remove_reference_t<T>,
                           typename view::self_type::fields_typelist::
                                   template at<0>::type::type>>
    {
        CONTAINER::emplace_back(std::forward<const T>(t));
    }

private:
    int m_identifier;

public:
    int ZipIdentifier() const { return m_identifier; }
};

template <
        template <class> class SKIN, typename... IDeds,
        typename = typename std::enable_if_t<SOA::Utils::ALL(
                SOA::impl::is_skin<SKIN>(),
                has_semantic_zip<typename std::remove_cv_t<
                        typename std::remove_reference_t<IDeds>>>::value...)>>
auto semantic_zip(IDeds&&... views) -> ZipContainer<decltype(
        zip(std::forward<typename std::remove_reference_t<IDeds>::view>(
                    static_cast<typename std::remove_reference_t<
                            IDeds>::view>(views))...)
                .template view<SKIN>())>
{
    /// maybe assert
    assert(are_semantically_compatible(views...));
/// or throw
#ifndef NDEBUG
    if (!are_semantically_compatible(views...)) {
        throw IncompatibleZipException("zipping from different sets");
    }
#endif

    auto encapsulated_zip =
            zip(std::forward<typename std::remove_reference_t<IDeds>::view>(
                        static_cast<typename std::remove_reference_t<
                                IDeds>::view>(views))...)
                    .template view<SKIN>();
    return ZipContainer<decltype(encapsulated_zip)>(firstid(views...),
                                                    encapsulated_zip);
}

#endif
