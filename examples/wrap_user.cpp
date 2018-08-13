// #include "SOAField.h"
// #include "SOASkin.h"
#include "SOAUtils.h"
#include "vectorwrapper.h"
#include <iostream>
#include <range/v3/view/zip.hpp>
#include <type_traits>

class IncompatibleZip
{
  std::string m_message{"unspecified failure"};

public:
  IncompatibleZip() {}
  IncompatibleZip( std::string s ) : m_message( s ) {}
  std::string message() { return m_message; }
};

template <typename... T>
bool variable_length_or( const T... v )
{
  return ( v || ... );
}
template <typename... T>
bool variable_length_and( const T... v )
{
  return ( v && ... );
}

template <typename CORE, typename... EXTS>
typename std::enable_if<
    SOA::Utils::ALL( std::is_same<typename CORE::value_type, typename EXTS::value_type::extends_t>::value... ),
    decltype( ranges::view::zip( std::declval<CORE>(), std::declval<EXTS>()... ) )>::type
ziprange( CORE c, EXTS... e )
{
#ifndef NDEBUG
  if ( variable_length_or( c.size() != e.size()... ) ) throw IncompatibleZip( "length mismatch" );
  if ( ! variable_length_and( e.validate_ref( &c )... ) ) throw IncompatibleZip( "extension extends other core" );
#endif
  return ranges::view::zip( c, e... );
}

class core
{
  int m_v;

public:
  core( int v ) : m_v( v ) {}
  friend std::ostream& operator<<( std::ostream&, const core& );
};

inline std::ostream& operator<<( std::ostream& str, const core& c )
{
  str << c.m_v;
  return str;
}

// SOAFIELD_TRIVIAL( f_core, accessor_core, core );
// SOASKIN_TRIVIAL( s_core, f_core );

class extend_core
{
  int m_w;

public:
  using extends_t = core;
  extend_core( int w ) : m_w( w ) {}
};
// SOAFIELD_TRIVIAL( f_extends, accessor_extends, extend_core );
// SOASKIN_TRIVIAL( s_extends, f_extends );
// SOASKIN_TRIVIAL( s_extended_core, f_core, f_extends );

auto create()
{

  std::vector<core>      v( {{1, 2, 3}} );
  extendablevector<core> vc( v );
  std::cout << "v  is of length " << v.size() << std::endl;
  std::cout << "vc is of length " << vc.size() << std::endl;
  std::cout << "elements:" << std::endl;
  for ( size_t i = 0; i < v.size(); ++i ) {
    std::cout << "\t" << v[i] << "\t" << vc[i] << std::endl;
  }
  vectorextension<extend_core> ve( {1, 2, 3} );
  std::cout << "core is " << &vc << std::endl;
  std::cout << "extension extends " << ve.base() << std::endl;
  std::cout << "validation: " << ve.validate_ref(&vc) << std::endl;
  ve.set_ref( vc );
  std::cout << "core is " << &vc << std::endl;
  std::cout << "extension extends " << ve.base() << std::endl;
  std::cout << "validation: " << ve.validate_ref(&vc) << std::endl;
  return std::make_tuple(vc,ve);
}

int main() {
  auto t = create();
  auto vc = std::get<0>(t);
  auto ve = std::get<1>(t);

  std::cout << "core is " << &vc << std::endl;
  std::cout << "extension extends " << ve.base() << std::endl;
  std::cout << "validation: " << ve.validate_ref(&vc) << std::endl;

  extendablevector<core> vcc(std::move(vc));

  try {
    auto zv = ziprange( vcc, ve );

    return zv.size();
  } catch ( IncompatibleZip i ) {
    std::cout << i.message() << std::endl;
  }
}
