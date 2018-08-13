#include "SOAField.h"
#include "SOASkin.h"
#include "SOAView.h"
#include "SOAUtils.h"
#include "vectorwrapper.h"
#include <iostream>
#include <range/v3/view/zip.hpp>
#include <type_traits>

#undef NDEBUG

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
    SOA::Utils::ALL(
        std::is_same<
            typename std::remove_cv<typename std::remove_reference<typename CORE::value_type>::type>::type,
            typename std::remove_cv<typename std::remove_reference<typename EXTS::value_type::extends_t>::type>::type>::value... ),
    decltype( ranges::view::zip( std::declval<CORE>(), std::declval<EXTS>()... ) )>::type
ziprange( CORE& c, EXTS&... e )
{
#ifndef NDEBUG
  if ( variable_length_or( c.size() != e.size()... ) ) throw IncompatibleZip( "length mismatch" );
  if ( ! variable_length_and( e.validate_ref( &c )... ) ) throw IncompatibleZip( "extension extends other core" );
#endif
  return ranges::view::zip( c, e... );
}

template <template <typename> typename SKIN, typename CORE, typename... EXTS, typename =
typename std::enable_if<
    SOA::Utils::ALL(
        SOA::impl::is_skin<SKIN>(),
        std::is_same<
            typename std::remove_cv<typename std::remove_reference<typename CORE::value_type>::type>::type,
            typename std::remove_cv<typename std::remove_reference<typename EXTS::value_type::extends_t>::type>::type>::value... )> >
auto
zipsoa( CORE c, EXTS... e )
{
#ifndef NDEBUG
  if ( ! variable_length_and( e.validate_ref( &c )... ) ) throw IncompatibleZip( "extension extends other core" );
#endif
  return SOA::make_soaview<SKIN>(c, e... );
}

// /// don't get this to work :(
// template <typename CORE, typename... VIEWS>
// typename std::enable_if<
//     SOA::Utils::ALL(
//         SOA::Utils::is_view<typename std::remove_cv<typename std::remove_reference<CORE>::type>::type>::value,
//         SOA::Utils::is_view<typename std::remove_cv<typename std::remove_reference<VIEWS>::type>::type>::value...,
//         std::is_same<
//             typename std::remove_cv<typename std::remove_reference<typename CORE::contained_type::type>::type>::type,
//             typename std::remove_cv<typename std::remove_reference<typename VIEWS::contained_type::type::extends_t>::type>::type>::value... ),
//     decltype( zip<VIEWS...>( std::declval<VIEWS>()... ) )>::type
// zipsoa( CORE c, VIEWS... v)
// {
// #ifndef NDEBUG
//   // if ( variable_length_or( c.size() != e.size()... ) ) throw IncompatibleZip( "length mismatch" ); // done by SOA
//   // if ( ! variable_length_and( e.validate_ref( &c )... ) ) throw IncompatibleZip( "extension extends other core" ); // FIXME!!!
// #endif
//   return zip<CORE,VIEWS...>(c,v...);
// }

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

SOAFIELD_TRIVIAL( f_core, accessor_core, core );
SOASKIN_TRIVIAL( s_core, f_core );

class extend_core
{
  int m_w;

public:
  using extends_t = core;
  extend_core( int w ) : m_w( w ) {}
};
SOAFIELD_TRIVIAL( f_extends, accessor_extends, extend_core );
SOASKIN_TRIVIAL( s_extends, f_extends );
SOASKIN_TRIVIAL( s_extended_core, f_core, f_extends );

// auto create()
int main()
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
//   return std::make_tuple(vc,ve);
// }
//
// int main() {
//   auto t = create();
//   auto vc = std::get<0>(t);
//   auto ve = std::get<1>(t);

  std::cout << "core is " << &vc << std::endl;
  std::cout << "extension extends " << ve.base() << std::endl;
  std::cout << "validation: " << ve.validate_ref(&vc) << std::endl;

  std::cout << "vc is of length " << vc.size() << std::endl;
  std::cout << "ve is of length " << ve.size() << std::endl;

  //extendablevector<core> vcc(std::move(vc));

  try {
    auto zv = ziprange( vc, ve );
    //auto zv = zipsoa<s_extended_core, decltype(vc), decltype(ve)>( vc, ve );

    return zv.size();
  } catch ( IncompatibleZip i ) {
    std::cout << i.message() << std::endl;
  }
}
