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

#ifndef ZipTraits_h
#define ZipTraits_h 1
#include "SOAUtils.h"                  // for is_view
template<typename CONTAINER>
class IDedSOAContainer;

template<typename T>
struct is_IDed
{
    bool value = false;
};

template<typename VIEW>
struct is_IDed<IDedSOAContainer<VIEW>>
{
    static constexpr bool value =
        SOA::Utils::is_view<std::decay_t<VIEW>>::value;
};


#endif
