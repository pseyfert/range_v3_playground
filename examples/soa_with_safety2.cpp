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

#include "./someclass.h"                  // for fitres, s_fitres, s_track_...
#include "SOAContainer.h"                 // for Container
#include "SOAContainerSet.h"              // for ZipContainer, semantic_zip
#include <bits/exception.h>               // for exception
#include <iostream>                       // for operator<<, basic_ostream
#include <range/v3/view/indices.hpp>      // for indices_fn, indices
#include <range/v3/view/take_exactly.hpp> // for take_exactly_view_
#include <utility>                        // for move
#include <vector>                         // for vector

/// pythonic sugar
#include "range/v3/all.hpp" // IWYU pragma: keep
// IWYU pragma : no_include <range/v3/view/indices.hpp>
// IWYU pragma : no_include <range/v3/view/take_exactly.hpp>
auto range = ranges::view::indices;
/// end of sugar

int main() {
  ZipContainer<SOA::Container<std::vector, s_track>>   foo1{};
  ZipContainer<SOA::Container<std::vector, s_fitres>>  foo2{foo1.zipIdentifier()};
  ZipContainer<SOA::Container<std::vector, s_fitres>>  foo4{};
  ZipContainer<SOA::Container<std::vector, s_fitqual>> foo3{foo1.zipIdentifier()};
  for ( auto i : range( 42 ) ) {
    track t{i / 100.f, 23.f, 1337.f, 8472.f, 3.14f};
    foo1.push_back( t );
  }
  for ( auto i : range( 42 ) ) {
    const fitres r{100.f, 0.f, 7.f, i};
    foo2.push_back( fitres{100.f, 0.f, 7.f, i} );
    foo4.push_back( r );
  }
  for ( auto i : range( 43 ) ) {
    fitqual q{1.f, i};
    foo3.emplace_back( q );
  }

  try {
    [[maybe_unused]] auto x =
        semantic_zip<s_track_with_fitres>( foo1, foo2 ); // works at compile time, works at run time
  } catch ( const std::exception& e ) {
    std::cout << "caught UNEXPECTED exception: " << e.what() << std::endl;
    return 1;
  }
  try {
    [[maybe_unused]] auto x =
        semantic_zip<s_track_with_fitres>( foo1, foo4 ); // works at compile time, assertion failure at run time
  } catch ( const std::exception& e ) { std::cout << "caught expected exception: " << e.what() << std::endl; }
  try {
    [[maybe_unused]] auto x =
        semantic_zip<s_track_with_fitqual>( foo1, foo3 ); // works at compile time, throws at run time
  } catch ( const std::exception& e ) { std::cout << "caught expected exception: " << e.what() << std::endl; }
  {
    //[[maybe_unused]] auto x = semantic_zip<s_track_with_fitres>( foo1, foo3 ); // fails at compile time
  }

  return 0;
}
