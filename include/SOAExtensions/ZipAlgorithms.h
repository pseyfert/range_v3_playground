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

#ifndef ZipAlgorithms_h
#define ZipAlgorithms_h

#include "SOAContainer/SOAContainer.h"
#include "SOAExtensions/ZipContainer.h"
#include "SOAExtensions/ZipSelection.h"
#include <algorithm>

namespace Zipping {
  template <template <typename> typename OUTSKIN, typename T, typename Predicate>
  auto for_each( const T& input, Predicate&& predicate ) {
    Zipping::ZipContainer<SOA::Container<T::template ContainerTemplate, OUTSKIN>> retval( input.zipIdentifier() );
    retval.reserve( input.size() );
    for ( auto in : input ) { retval.emplace_back( std::invoke( predicate, in ) ); }
    return retval;
  }

  template <template <typename> typename OUTSKIN, typename T, typename Predicate, typename IndexSize>
  auto for_some( const T& input, Predicate&& predicate, const ExportedSelection<IndexSize>& sel ) {
    Zipping::ZipContainer<SOA::Container<T::template ContainerTemplate, OUTSKIN>> retval( input.zipIdentifier() );
    retval.reserve( input.size() );
    assert( std::is_sorted( sel.m_indices.begin(), sel.m_indices.end() ) );

    IndexSize i                 = 0;
    auto      selectionIterator = sel.m_indices.begin();
    for ( ; i < input.size(); ++i ) {
      if ( selectionIterator != sel.m_indices.end() && *selectionIterator == i ) {
        retval.emplace_back( std::invoke( predicate, input[i] ) );
        ++selectionIterator;
      } else {
        retval.emplace_back(); // default / invalid construction
      }
    }

    assert( retval.size() == input.size() );
    assert( Zipping::areSemanticallyCompatible( retval, input ) );

    return retval;
  }
} // namespace Zipping

#endif // ZipAlgorithms_h
