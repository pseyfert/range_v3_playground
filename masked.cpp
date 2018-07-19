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

public:
  Widget() {}
  Widget( int i ) : m_int( i ) {}
  int   the_int() const { return m_int; }
  void  upit() { m_int += 23; }
};

inline std::ostream& operator<<( std::ostream& str, const Widget& obj )
{
  str << '\t' << obj.the_int();
  return str;
}

int main()
{
  std::vector<Widget> widgets;
  std::vector<Widget> tmp;
  std::vector<bool>   mask;
  std::vector<bool>   mask2;

  for ( auto i : ranges::view::indices( 24 ) ) {
    widgets.emplace_back( i );
    tmp.emplace_back( i );
    mask.push_back( i % 3 != 1 );
    mask2.push_back( (i+3) % 4 != 2 );
  }
  const std::vector<Widget> cwidgets(tmp);

  std::cout << "wrapped" << std::endl;
  for ( auto& el : ranges::view::masker( widgets, mask ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "piped" << std::endl;
  for ( auto& el : ranges::view::all( widgets ) | ranges::view::masker( mask ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "piped" << std::endl;
  for ( auto& el : ranges::view::all( widgets ) | ranges::view::masker( mask2 ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "piped" << std::endl;
  for ( auto& el : ranges::view::all( widgets ) | ranges::view::masker( ranges::view::make_or_masker( mask, mask2 ) ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "piped" << std::endl;
  for ( auto& el : ranges::view::apply_or_masker( widgets, mask, mask2 ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "piped" << std::endl;
  for ( auto& el : ranges::view::all( widgets ) | ranges::view::apply_or_masker( mask, mask2 ) ) {
    std::cout << el << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  return 0;
}
