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
  int m_int{0};

public:
  Widget() {}
  Widget( int i ) : m_int( i ) {}
  int  the_int() const { return m_int; }
  void upit() { m_int += 23; }
};

inline std::ostream& operator<<( std::ostream& str, const Widget& obj )
{
  str << '\t' << obj.the_int();
  return str;
}

int main( int argc, char** )
{
  std::vector<Widget> widgets;
  std::vector<Widget> tmp;
  std::vector<int>    mask;
  std::vector<int>    mask2;
  std::vector<int>    mask3;
  std::vector<int>    mask4;

  for ( auto i : ranges::view::indices( 24 ) ) {
    widgets.emplace_back( i );
    tmp.emplace_back( i );
    mask.push_back( i % 3 != 1 );
    mask2.push_back( ( i + 3 ) % 4 != 2 );
    mask3.push_back( ( i + 4 ) % 3 != 1 );
    mask4.push_back( ( i + 4 ) % 6 != 1 );
  }
  const std::vector<Widget> cwidgets( tmp );

  std::cout << "functional, single mask (mask)" << std::endl;
  for ( auto& el : ranges::view::masker( widgets, mask ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "piped, single mask (mask)" << std::endl;
  for ( auto& el : ranges::view::all( widgets ) | ranges::view::masker( mask ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "piped, single mask (mask2)" << std::endl;
  for ( auto& el : ranges::view::all( widgets ) | ranges::view::masker( mask2 ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "piped, into funtional mask || mask2 (N masks, non pipable mask)" << std::endl;
  for ( auto& el :
        ranges::view::all( widgets ) | ranges::view::masker( ranges::view::make_or_masker( mask, mask2 ) ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "staged functional, mask || mask2 (N masks, non pipable mask)" << std::endl;
  for ( auto& el : ranges::view::masker( widgets, ranges::view::make_or_masker( mask, mask2 ) ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "onecall functional, mask || mask2 (2 masks, pipe and functional)" << std::endl;
  for ( auto& el : ranges::view::or_masker( widgets, mask, mask2 ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "onecall piped, mask || mask2 (2 masks, pipe and functional)" << std::endl;
  for ( auto& el : ranges::view::all( widgets ) | ranges::view::or_masker( mask, mask2 ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "staged functional, mask || mask || mask || mask (N masks, non pipable mask)" << std::endl;
  for ( auto& el : ranges::view::masker( widgets, ranges::view::make_or_masker( mask, mask, mask, mask ) ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "piped, mask || mask || mask || mask (N masks, non functional)" << std::endl;
  for ( auto& el : ranges::view::all( widgets ) | ranges::view::apply_or_masker( mask, mask, mask, mask ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::vector<std::vector<int>> varlengthmasks;
  for ( [[maybe_unused]] auto i : ranges::view::indices( argc ) ) {
    if ( 0 == ( i % 4 ) ) varlengthmasks.push_back( mask );
    if ( 1 == ( i % 4 ) ) varlengthmasks.push_back( mask2 );
    if ( 2 == ( i % 4 ) ) varlengthmasks.push_back( mask3 );
    if ( 3 == ( i % 4 ) ) varlengthmasks.push_back( mask4 );
  }
  for ( auto& el : ranges::view::masker( widgets, ranges::view::make_bool_vector_or_masker( varlengthmasks ) ) ) {
    std::cout << el << std::endl;
  }
  return 0;
}
