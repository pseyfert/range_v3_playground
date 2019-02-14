#include "./someclass.h"
#include "SOAContainer.h"
#include "SOAField.h"
#include "SOASkin.h"
#include "SOAUtils.h"
#include "SOAView.h"
#include <iostream>

/// pythonic sugar
#include "range/v3/all.hpp" // for pythonic range
#include <boost/type_index.hpp>
auto range = ranges::view::indices;
//

class IncompatibleZip {
  std::string m_message{"unspecified failure"};

public:
  IncompatibleZip() {}
  IncompatibleZip( std::string s ) : m_message( s ) {}
  std::string message() { return m_message; }
};

template <typename T>
struct is_IDed {
  bool value = false;
};

template <typename CONTAINER>
class IDedSOAContainer;

template <typename VIEW>
struct is_IDed<IDedSOAContainer<VIEW>> {
  static constexpr bool value = SOA::Utils::is_view<std::decay_t<VIEW>>::value;
};

template <typename FIRST, typename SECOND, typename... OTHERS>
bool allsameid( FIRST& first, SECOND& second, OTHERS&... others );

template <typename FIRST>
bool allsameid( FIRST& /*unused*/ );

template <typename T1, typename... T>
struct typegetter {
  using type = T1;
};

template <typename CONTAINER>
class IDedSOAContainer : public CONTAINER {
public:
  using view = CONTAINER;

  template <typename... Args>
  IDedSOAContainer( int ID, Args&&... args ) : CONTAINER( args... ), m_identifier( ID ) {}

  // template <typename T>
  // auto push_back( T& t )
  //     -> std::enable_if_t<view::self_type::fields_typelist::size() == 1 &&
  //                         std::is_same_v<T, typename view::self_type::fields_typelist::template at<0>::type::type>> {
  //   CONTAINER::emplace_back( std::forward<T>( t ) );
  // }
  template <typename T>
  auto push_back( T&& t )
      -> std::enable_if_t<view::self_type::fields_typelist::size() == 1 &&
                          std::is_same_v<std::remove_reference_t<T>, typename view::self_type::fields_typelist::template at<0>::type::type>> {
    CONTAINER::emplace_back( std::forward<T>( t ) );
  }
  template <typename T>
  auto push_back( const T& t )
      -> std::enable_if_t<view::self_type::fields_typelist::size() == 1 &&
                          std::is_same_v<std::remove_reference_t<T>, typename view::self_type::fields_typelist::template at<0>::type::type>> {
    CONTAINER::emplace_back( std::forward<const T>( t ) );
  }

private:
  int m_identifier;

public:
  int ZipIdentifier() const { return m_identifier; }
};

template <template <class> class SKIN, typename... IDeds,
          typename = typename std::enable_if_t<
              SOA::Utils::ALL( SOA::impl::is_skin<SKIN>(),
                               is_IDed<typename std::remove_cv_t<typename std::remove_reference_t<IDeds>>>::value... )>>
auto myzip( IDeds&&... views )
    -> decltype( zip( std::forward<typename std::remove_reference_t<IDeds>::view>(
                          static_cast<typename std::remove_reference_t<IDeds>::view>( views ) )... )
                     .template view<SKIN>() ) {
  assert( allsameid( views... ) );
  return zip( std::forward<typename std::remove_reference_t<IDeds>::view>(
                  static_cast<typename std::remove_reference_t<IDeds>::view>( views ) )... )
      .template view<SKIN>();
}

template <typename FIRST, typename SECOND, typename... OTHERS>
bool allsameid( FIRST& first, SECOND& second, OTHERS&... others ) {
  return ( first.ZipIdentifier() == second.ZipIdentifier() ) && ( allsameid( first, others... ) );
}

template <typename FIRST>
bool allsameid( FIRST& /*unused*/ ) {
  return true;
}

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
