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

#undef NDEBUG

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utestSelection
#include "SOAContainer/SOAContainer.h"    // for Container
#include "SOAExtensions/ZipContainer.h" // for ZipContainer, semantic_zip
#include "SOAContainer/SOASkin.h"         // for SOASkinCreatorSimple<>::type
#include "SOAContainer/SOAView.h"         // for _View<>::reference, _View
#include "SOAExtensions/ZipSelection.h"    // for SelectionView, SelectionVi...
#include <array>
#include <boost/container/static_vector.hpp>
#include <boost/test/unit_test.hpp>
namespace utf = boost::unit_test;
// #include <boost/test/execution_monitor.hpp>
#include <memory>  // for allocator, allocator_trait...
#include <string>  // for operator<<, string
#include <utility> // for move
#include <vector>  // for vector

#include "SOAContainer/SOAField.h"
#include "SOAContainer/SOASkin.h"
#include "SOAContainer/SOAUtils.h"
#include "SOAContainer/SOAView.h"

struct hit {
  float x;
  float y;
  float z;
  float adc;
};

struct track {
  track( float x, float y, float z ) : m_x{x}, m_y{y}, m_z{z} {}
  track( float tx, float ty ) : m_tx{tx}, m_ty{ty} {}
  // std::vector<hit>     m_hits{};
  boost::container::static_vector<hit, 8> m_hits{};
  // std::array<hit, 8>   m_hits{};
  float                m_x{0.f};
  float                m_y{0.f};
  float                m_z{0.f};
  float                m_tx{0.f};
  float                m_ty{0.f};
  friend bool          operator==( const track& lhs, const track& rhs ) { return &lhs == &rhs; }
  friend std::ostream& operator<<( std::ostream& stream, const track& t )
  {
    char buf[100];
    snprintf( buf, 99, "track at (%#8.2f, %#8.2f, %#8.2f)", t.m_x, t.m_y, t.m_z );
    stream << buf;
    return stream;
  }
};

SOAFIELD_TRIVIAL( f_track, accessor_track, track );
SOASKIN( s_track, f_track )
{
  SOASKIN_INHERIT_DEFAULT_METHODS( s_track );
  template <typename... T>
  s_track( T && ... t ) : __BASE__( track{std::forward<T>( t )...} )
  {
  }
};

struct fitres {
  float                px;
  float                py;
  float                pz;
  int                  q;
  friend bool          operator==( const fitres& lhs, const fitres& rhs ) { return &lhs == &rhs; }
  friend std::ostream& operator<<( std::ostream& stream, const fitres& t )
  {
    char buf[100];
    snprintf( buf, 99, "momentum = (%#8.2f, %#8.2f, %#8.2f), charge %d", t.px, t.py, t.pz, t.q );
    stream << buf;
    return stream;
  }
};

SOAFIELD_TRIVIAL( f_fitres, accessor_fitres, fitres );
SOASKIN_TRIVIAL( s_fitres, f_fitres );

SOASKIN_TRIVIAL( s_track_with_fitres, f_track, f_fitres );

/// pythonic sugar
#include "range/v3/all.hpp" // IWYU pragma: keep
// IWYU pragma : no_include <range/v3/view/indices.hpp>
// IWYU pragma : no_include <range/v3/view/take_exactly.hpp>
auto range = ranges::view::indices;
/// end of sugar

using namespace Zipping;

void get_const_ref_container( const ZipContainer<SOA::Container<std::vector, s_track>>& v )
{
  auto vv = semantic_zip<s_track>( v );

  for ( auto i : range( v.size() ) ) {
    BOOST_CHECK_EQUAL( v[i].accessor_track().m_x, vv[i].accessor_track().m_x );
    BOOST_CHECK_EQUAL( v[i].accessor_track().m_y, vv[i].accessor_track().m_y );
    BOOST_CHECK_EQUAL( v[i].accessor_track().m_z, vv[i].accessor_track().m_z );
    BOOST_CHECK_EQUAL( v[i].accessor_track().m_tx, vv[i].accessor_track().m_tx );
    BOOST_CHECK_EQUAL( v[i].accessor_track().m_ty, vv[i].accessor_track().m_ty );

    BOOST_CHECK_EQUAL( v[i].accessor_track().m_hits.size(), vv[i].accessor_track().m_hits.size() );
    for ( auto j : range( v[i].accessor_track().m_hits.size() ) ) {
      BOOST_CHECK_EQUAL( v[i].accessor_track().m_hits[j].x, vv[i].accessor_track().m_hits[j].x );
      BOOST_CHECK_EQUAL( v[i].accessor_track().m_hits[j].y, vv[i].accessor_track().m_hits[j].y );
      BOOST_CHECK_EQUAL( v[i].accessor_track().m_hits[j].z, vv[i].accessor_track().m_hits[j].z );
      BOOST_CHECK_EQUAL( v[i].accessor_track().m_hits[j].adc, vv[i].accessor_track().m_hits[j].adc );
    }
  }
}

BOOST_AUTO_TEST_CASE( some_failures_I_had, *utf::tolerance( 0.005f ) )
{
  ZipContainer<SOA::Container<std::vector, s_track>> v;

  for ( auto i : range( 10 ) ) {
    v.emplace_back( 100.f * i + 1.f, 2.f, 3.f );
    for ( auto j : range( 8 ) ) {
      v.back().accessor_track().m_hits.emplace_back( hit{10.f * i, j + 0.4f, j + 0.3f, 1237.f + j} );
      // v.back().accessor_track().m_hits[j] = hit{10.f * i, j + 0.4f, j + 0.3f, 1237.f + j};
    }
  }
  get_const_ref_container( v );

  auto w = semantic_zip<s_track>( v );

  for ( auto i : range( v.size() ) ) {
    BOOST_CHECK_EQUAL( v[i].accessor_track().m_x, w[i].accessor_track().m_x );
    BOOST_CHECK_EQUAL( v[i].accessor_track().m_y, w[i].accessor_track().m_y );
    BOOST_CHECK_EQUAL( v[i].accessor_track().m_z, w[i].accessor_track().m_z );
    BOOST_CHECK_EQUAL( v[i].accessor_track().m_tx, w[i].accessor_track().m_tx );
    BOOST_CHECK_EQUAL( v[i].accessor_track().m_ty, w[i].accessor_track().m_ty );

    BOOST_CHECK_EQUAL( v[i].accessor_track().m_hits.size(), w[i].accessor_track().m_hits.size() );
    for ( auto j : range( v[i].accessor_track().m_hits.size() ) ) {
      BOOST_CHECK_EQUAL( v[i].accessor_track().m_hits[j].x, w[i].accessor_track().m_hits[j].x );
      BOOST_CHECK_EQUAL( v[i].accessor_track().m_hits[j].y, w[i].accessor_track().m_hits[j].y );
      BOOST_CHECK_EQUAL( v[i].accessor_track().m_hits[j].z, w[i].accessor_track().m_hits[j].z );
      BOOST_CHECK_EQUAL( v[i].accessor_track().m_hits[j].adc, w[i].accessor_track().m_hits[j].adc );
    }
  }
}
