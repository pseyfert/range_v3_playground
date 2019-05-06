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

#include "./someclass.h"                // for fitres, s_track_with_fitres
#include "SOAContainer/SOAContainer.h"  // for Container
#include "SOAContainer/SOASkin.h"       // for SOASkinCreatorSimple<>::type
#include "SOAContainer/SOAView.h"       // for _View<>::reference, _View
#include "SOAExtensions/ZipContainer.h" // IWYU pragma: keep
#include "SOAExtensions/ZipSelection.h" // for SelectionView, SelectionVi...
#include <bits/exception.h>             // for exception
#include <iostream>                     // for operator<<, basic_ostream
#include <memory>                       // for allocator, allocator_trait...
#include <string>                       // for operator<<, string
#include <utility>                      // for move
#include <vector>                       // for vector

#include "SOAExtensions/ZipAlgorithms.h"

/// pythonic sugar
#include "range/v3/all.hpp" // IWYU pragma: keep
// IWYU pragma: no_include <range/v3/view/indices.hpp>
// IWYU pragma: no_include <range/v3/view/take_exactly.hpp>

auto range = ranges::view::indices;
/// end of sugar

template <typename PROXY>
void dumb_dump( const PROXY track, const std::string& greet ) {
  std::cout << greet << '\n';
  std::cout << "there's track " << track.accessor_fitres().q << " at (x,y,z): (" << track.accessor_track().x << ' '
            << track.accessor_track().y << ' ' << track.accessor_track().z << ")\n";
  std::cout << "            with p=(x,y,z): (" << track.accessor_fitres().px << ' ' << track.accessor_fitres().py << ' '
            << track.accessor_fitres().pz << ")\n\n";
}

int main() {
  Zipping::ZipContainer<SOA::Container<std::vector, s_track>>  foo1{};
  Zipping::ZipContainer<SOA::Container<std::vector, s_fitres>> foo2( foo1.zipIdentifier() );
  for ( auto i : range( 42 ) ) {
    track t{i / 100.f, 23.f, 1337.f, 8472.f, 3.14f};
    foo1.push_back( t );
  }
  for ( auto i : range( 42 ) ) {
    // const fitres r{100.f, 0.f, 7.f, i};
    foo2.push_back( fitres{100.f, 0.f, 7.f, i} );
  }

  try {
    [[maybe_unused]] auto x =
        Zipping::semantic_zip<s_track_with_fitres>( foo1, foo2 ); // works at compile time, works at run time

    for ( [[maybe_unused]] auto track : x ) { dumb_dump( track, "first loop" ); }
    dumb_dump( x[0], "lonely zero of main container" );
    Zipping::ExportedSelection<> se = Zipping::makeSelection( &x, []( auto&& track ) -> bool {
      return track.accessor_track().x >= 0.02 && ( track.accessor_fitres().q % 2 ) == 1;
    } );
    Zipping::SelectionView       sx( &x, se );

    dumb_dump( sx[0], "lonely zero of selection" );
    for ( [[maybe_unused]] auto track : sx ) { dumb_dump( track, "selection loop" ); }
    auto expo = sx.export_selection();
  } catch ( const std::exception& e ) {
    std::cout << "caught UNEXPECTED exception: " << e.what() << std::endl;
    return 1;
  }

  try {
    Zipping::ExportedSelection<> sfoo1e =
        Zipping::makeSelection( &foo1, []( auto&& track ) -> bool { return track.accessor_track().x >= 0.03; } );

    Zipping::SelectionView<decltype( foo1 )> sfoo1( &foo1, sfoo1e );
    for ( auto track : sfoo1 ) {
      std::cout << "small track part " << track.accessor_track().x << ' ' << track.accessor_track().y << ' '
                << track.accessor_track().z << '\n';
    }

    auto                  expo = sfoo1.export_selection();
    [[maybe_unused]] auto x =
        Zipping::semantic_zip<s_track_with_fitres>( foo1, foo2 ); // works at compile time, works at run time
    Zipping::SelectionView<decltype( x )> sx( &x, expo );
    for ( [[maybe_unused]] auto track : sx ) { dumb_dump( track, "loop with exported selection" ); }
  } catch ( const std::exception& e ) {
    std::cout << "caught UNEXPECTED exception: " << e.what() << std::endl;
    return 1;
  }

  {
    auto track_to_fitres_lambda = []( const auto track ) { return fitres{track.accessor_track().x, 2.f, 3.f, 42}; };
    // Zipping::ZipContainer<SOA::Container<std::vector, s_fitres>> foo2 =
    auto foo2 =
        Zipping::for_each<s_fitres>( foo1, track_to_fitres_lambda );

    std::cout << "expect Zipping::ZipContainer<SOA::Container<std::vector, s_fitres>>, got " << boost::typeindex::type_id_with_cvr<decltype(foo2)>().pretty_name() << '\n';

    [[maybe_unused]] auto x = Zipping::semantic_zip<s_track_with_fitres>( foo1, foo2 );

    Zipping::ExportedSelection<> sfoo1e =
        Zipping::makeSelection( &foo1, []( auto&& track ) -> bool { return track.accessor_track().x >= 0.03; } );

    // Zipping::ZipContainer<SOA::Container<std::vector, s_fitres>> foo2_ =
    auto foo2_ =
        Zipping::for_some<s_fitres>( foo1, track_to_fitres_lambda, sfoo1e );

    [[maybe_unused]] auto y = Zipping::semantic_zip<s_track_with_fitres>( foo1, foo2_ );
  }

  std::flush( std::cout );

  return 0;
}
