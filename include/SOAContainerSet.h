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
#include <assert.h>                    // for assert
#include <string>                      // for allocator, string
#include <type_traits>                 // for remove_reference_t, enable_if_t
#include <utility>                     // for forward
#include "IncompatibleZipException.h"  // for IncompatibleZipException
#include "SOASkin.h"                   // for is_skin
#include "SOAUtils.h"                  // for ALL, is_view
#include "SOAView.h"                   // for zip
#include "ZipTraits.h"
template <typename CONTAINER> class IDedSOAContainer;  // lines 14-15

template<typename FIRST, typename SECOND, typename... OTHERS>
bool allsameid(FIRST& first, SECOND& second, OTHERS&... others);

template<typename FIRST, typename... OTHERS>
int
firstid(FIRST& first, OTHERS&...)
{
    return first.ZipIdentifier();
}

template<typename FIRST>
bool allsameid(FIRST& /*unused*/);

template<typename CONTAINER>
class IDedSOAContainer : public CONTAINER
{
public:
    using view = CONTAINER;

    template<typename... Args>
    IDedSOAContainer(int ID, Args&&... args)
      : CONTAINER(args...)
      , m_identifier(ID)
    {}

    template<typename T>
    auto push_back(T&& t) -> std::enable_if_t<
        view::self_type::fields_typelist::size() == 1 &&
        std::is_same_v<std::remove_reference_t<T>,
                       typename view::self_type::fields_typelist::template at<
                           0>::type::type>>
    {
        CONTAINER::emplace_back(std::forward<T>(t));
    }
    template<typename T>
    auto push_back(const T& t) -> std::enable_if_t<
        view::self_type::fields_typelist::size() == 1 &&
        std::is_same_v<std::remove_reference_t<T>,
                       typename view::self_type::fields_typelist::template at<
                           0>::type::type>>
    {
        CONTAINER::emplace_back(std::forward<const T>(t));
    }

private:
    int m_identifier;

public:
    int ZipIdentifier() const
    {
        return m_identifier;
    }
};

template<template<class> class SKIN, typename... IDeds,
         typename = typename std::enable_if_t<SOA::Utils::ALL(
             SOA::impl::is_skin<SKIN>(),
             is_IDed<typename std::remove_cv_t<
                 typename std::remove_reference_t<IDeds>>>::value...)>>
auto
myzip(IDeds&&... views) -> IDedSOAContainer<
    decltype(zip(std::forward<typename std::remove_reference_t<IDeds>::view>(
                     static_cast<typename std::remove_reference_t<IDeds>::view>(
                         views))...)
                 .template view<SKIN>())>
{
    /// maybe assert
    assert(allsameid(views...));
/// or throw
#ifndef NDEBUG
    if(!allsameid(views...)) {
        throw IncompatibleZipException("zipping from different sets");
    }
#endif

    auto encapsulated_zip =
        zip(std::forward<typename std::remove_reference_t<IDeds>::view>(
                static_cast<typename std::remove_reference_t<IDeds>::view>(
                    views))...)
            .template view<SKIN>();
    return IDedSOAContainer<decltype(encapsulated_zip)>(firstid(views...),
                                                        encapsulated_zip);
}

template<typename FIRST, typename SECOND, typename... OTHERS>
bool
allsameid(FIRST& first, SECOND& second, OTHERS&... others)
{
    return (first.ZipIdentifier() == second.ZipIdentifier()) &&
           (allsameid(first, others...));
}

template<typename FIRST>
bool
allsameid(FIRST& /*unused*/)
{
    return true;
}

#endif
