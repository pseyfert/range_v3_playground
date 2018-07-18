/*
 * Copyright (C) 2018  CERN for the benefit of the LHCb collaboration
 * Author: Paul Seyfert <pseyfert@cern.ch>
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

#include <boost/type_index.hpp>
#include <iostream>
#include <range/v3/all.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/indices.hpp>
#include <range/v3/view/mask.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>
#include <vector>

using indextype = unsigned int;

class Widget
{
private:
  int   m_int{0};
  float m_float{0.f};

public:
  Widget() {}
  Widget( int i ) : m_int( i ) {}
  Widget( float f ) : m_float( f ) {}
  Widget( int i, float f ) : m_int( i ), m_float( f ) {}
  Widget( float f, int i ) : m_int( i ), m_float( f ) {}
  int   the_int() const { return m_int; }
  float the_float() const { return m_float; }
  void  upit() { m_float = m_float + 42.f; }
};

inline std::ostream& operator<<( std::ostream& str, const Widget& obj )
{
  str << obj.the_int() << '\t' << obj.the_float();
  return str;
}

auto keep_true = []( auto range_item ) -> bool { return range_item.second; };

// I have next to no idea how much sense this one makes with regard to const correctness or
// move semantics or obtaining references to the original contained objects.
auto get_first = []( auto range_item ) -> decltype( auto ) { return range_item.first; };

// TODO: get all the nice syntactic sugar that range v3 usually has.
auto view_masked = []( auto rng ) {
  return ( ranges::view::filter( rng, keep_true ) | ranges::view::transform( get_first ) );
};

int main()
{
  std::vector<Widget> widgets;
  std::vector<bool>   mask;

  for ( auto i : ranges::view::indices( 24 ) ) {
    widgets.emplace_back( i, float( i ) );
  }
  for ( auto i : ranges::view::indices( 42 ) ) {
    mask.push_back( i % 3 != 1 );
  }

  // // too long but works
  // std::cout << "long version" << std::endl;
  // for ( auto& el : ranges::view::zip( widgets, mask ) | ranges::view::filter( keep_true ) |
  //                      ranges::view::transform( get_first ) ) {
  //   std::cout << el << std::endl;
  //   el.upit();
  // }
  // std::cout << std::endl;
  // std::cout << std::endl;
  //
  // // does the same here, but don't want to know what doesn't work
  // std::cout << "bad version" << std::endl;
  // for ( auto& el : view_masked( ranges::view::zip( widgets, mask ) ) ) {
  //   std::cout << el << std::endl;
  //   el.upit();
  // }
  // std::cout << std::endl;
  // std::cout << std::endl;

  std::cout << "wrapped" << std::endl;
  for ( auto& el : ranges::view::masker( widgets, mask ) ) {
    std::cout << el << std::endl;
    el.upit();
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "piped" << std::endl;
  for ( auto& el : ranges::view::all( widgets ) | ranges::view::masker( mask ) ) {
    std::cout << el << std::endl;
  }
  // std::cout << std::endl;
  // std::cout << std::endl;
  //
  // const indextype getit = 4;
  // std::cout << "element index " << getit << " is: " << std::endl;
  //
  // indextype i     = 0;
  // using rangetype = decltype( ranges::view::masker( widgets, mask ) );
  // ranges::iterator_t<rangetype> it;
  // auto theend = ranges::view::masker( widgets, mask ).end();
  // for ( it = ranges::view::masker( widgets, mask ).begin(); it != theend; ++it ) {
  //   if ( i++ == getit ) break;
  // }
  //
  // // std::cout << '\t' << ( *it ).the_int() << '\t' << ( *it ).the_float() << std::endl;
  // std::cout << (*it) << std::endl;
  // // std::cout << '\t' << ranges::view::masker( widgets, mask ).begin()[4].the_int()
  // //           << '\t' << ranges::view::masker( widgets, mask ).begin()[4].the_float() << std::endl;
  //
  // // using ind_t = ranges::range_difference_type_t<decltype(ranges::view::masker(widgets, mask))>;
  // // ind_t getit_argh = getit;
  // //
  // // std::cout << '\t' << ranges::begin( ranges::view::masker( widgets, mask ) )[getit] << std::endl;
  //
  return 0;
}
