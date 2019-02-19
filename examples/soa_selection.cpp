#include "./someclass.h"
#include "PrSelection.h"
#include "SOAContainer.h"
#include "SOAContainerSet.h"
#include <iostream>

#include <boost/type_index.hpp>

/// pythonic sugar
#include "range/v3/all.hpp" // for pythonic range
auto range = ranges::view::indices;
/// end of sugar

template <typename PROXY>
void dumb_dump( const PROXY track, const std::string& greet ) {
  std::cout << greet << '\n';
  std::cout << "there's track " << track.accessor_fitres().q << " at (x,y,z): (" << track.accessor_track().x << ' '
            << track.accessor_track().y << ' ' << track.accessor_track().z << ")\n";
  std::cout << "            with p=(x,y,z): (" << track.accessor_fitres().px << ' ' << track.accessor_fitres().py << ' '
            << track.accessor_fitres().pz << ")\n\n";
  return;
}

int main() {
  IDedSOAContainer<SOA::Container<std::vector, s_track>>  foo1( 11 );
  IDedSOAContainer<SOA::Container<std::vector, s_fitres>> foo2( 11 );
  for ( auto i : range( 42 ) ) {
    track t{i / 100.f, 23.f, 1337.f, 8472.f, 3.14f};
    foo1.push_back( t );
  }
  for ( auto i : range( 42 ) ) {
    // const fitres r{100.f, 0.f, 7.f, i};
    foo2.push_back( fitres{100.f, 0.f, 7.f, i} );
  }

  // braces just for shadowing
  {
    [[maybe_unused]] auto x = myzip<s_track_with_fitres>( foo1, foo2 ); // works at compile time, works at run time

    for ( [[maybe_unused]] auto track : x ) { dumb_dump( track, "first loop" ); }
    dumb_dump( x[0], "lonely zero of main container" );
    SelectionView<decltype( x )> sx( x, []( auto&& track ) -> bool {
      return track.accessor_track().x >= 0.02 && ( track.accessor_fitres().q % 2 ) == 1;
    } );
    dumb_dump( sx[0], "lonely zero of selection" );
    for ( [[maybe_unused]] auto track : sx ) { dumb_dump( track, "selection loop" ); }
    auto expo = sx.export_selection();
  }

  {
    SelectionView<decltype( foo1 )> sfoo1( foo1,
                                           []( auto&& track ) -> bool { return track.accessor_track().x >= 0.03; } );
    for ( auto track : sfoo1 ) {
      std::cout << "small track part " << track.accessor_track().x << ' ' << track.accessor_track().y << ' '
                << track.accessor_track().z << '\n';
    }

    auto                  expo = sfoo1.export_selection();
    [[maybe_unused]] auto x    = myzip<s_track_with_fitres>( foo1, foo2 ); // works at compile time, works at run time
    SelectionView<decltype( x )> sx( x, expo );
    for ( [[maybe_unused]] auto track : sx ) { dumb_dump( track, "loop with exported selection" ); }
  }

  std::flush( std::cout );

  return 0;
}
