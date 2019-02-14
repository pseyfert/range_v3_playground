#include "./someclass.h"
#include "SOAContainerSet.h"
#include "SOAContainer.h"
#include <iostream>


/// pythonic sugar
#include "range/v3/all.hpp"      // for pythonic range
auto range = ranges::view::indices;
/// end of sugar

int main() {
  IDedSOAContainer<SOA::Container<std::vector, s_track>>   foo1( 11 );
  IDedSOAContainer<SOA::Container<std::vector, s_fitres>>  foo2( 11 );
  IDedSOAContainer<SOA::Container<std::vector, s_fitres>>  foo4( 33 );
  IDedSOAContainer<SOA::Container<std::vector, s_fitqual>> foo3( 11 );
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

  // braces just for shadowing
  {
    [[maybe_unused]] auto x = myzip<s_track_with_fitres>( foo1, foo2 ); // works at compile time, works at run time
  }
  {
#if 0
    [[maybe_unused]] auto x = myzip<s_track_with_fitres>( foo1, foo4 ); // works at compile time, assertion failure at run time
#endif
  }
  {
#if 0
    [[maybe_unused]] auto x = myzip<s_track_with_fitqual>( foo1, foo3 ); // works at compile time, throws at run time
#endif
  }
  {
#if 0
    [[maybe_unused]] auto x = myzip<s_track_with_fitres>( foo1, foo3 ); // fails at compile time
#endif
  }

  return 0;
}
