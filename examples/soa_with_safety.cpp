#include "./someclass.h"
#include "SOAContainer.h"
#include "SOAField.h"
#include "SOASkin.h"
#include "SOAUtils.h"
#include "SOAView.h"
#include <iostream>

/// pythonic sugar
#include "range/v3/all.hpp" // for pythonic range
auto range = ranges::view::indices;
//

class IncompatibleZip {
  std::string m_message{"unspecified failure"};

public:
  IncompatibleZip() {}
  IncompatibleZip( std::string s ) : m_message( s ) {}
  std::string message() { return m_message; }
};

template <typename... T>
bool variable_length_and( const T... v ) {
  return ( v && ... );
}

template <typename T>
struct is_IDed;

template <typename FIRST, typename SECOND, typename... OTHERS>
bool allsameid( FIRST& first, SECOND& second, OTHERS&... others );

template <typename FIRST>
bool allsameid( FIRST& /*unused*/ );

namespace {
  struct another_zip_helper;
}

template <class SOAContainer>
class IDedSOAContainer {
public:
  template <typename... Args>
  IDedSOAContainer( int ID, Args&&... args ) : m_container( args... ), m_identifier( ID ) {}
  using view = typename std::remove_reference<SOAContainer>::type;

private:
  SOAContainer m_container;
  int          m_identifier;

public:
  int ZipIdentifier() const { return m_identifier; }
  template <typename... T>
  decltype( auto ) emplace_back( T&&... t ) {
    return m_container.emplace_back( t... );
  }
  template <typename T1, typename T2, typename... T>
  decltype( auto ) push_back( T1&& t1, T2&& t2, T&&... t ) {
    return m_container.push_back( t1, t2, t... );
  }
  template <typename T1>
  decltype( auto ) push_back( T1&& t1 ) {
    return m_container.push_back( std::make_tuple( std::move( t1 ) ) );
  }
  auto begin() { return m_container.begin(); }
  auto rbegin() { return m_container.rbegin(); }
  auto end() { return m_container.end(); }
  auto rend() { return m_container.rend(); }
  auto back() { return m_container.back(); }
  auto front() { return m_container.front(); }

  friend another_zip_helper;
};

namespace {
  struct another_zip_helper {
    template <template <class> class SKIN, typename... IDeds,
              typename = typename std::enable_if_t<SOA::Utils::ALL(
                  SOA::impl::is_skin<SKIN>(),
                  is_IDed<typename std::remove_cv_t<typename std::remove_reference_t<IDeds>>>::value... )>>
    static auto myzip( IDeds&&... views )
        -> decltype( zip( std::forward<typename std::remove_reference_t<IDeds>::view>( views.m_container )... )
                         .template view<SKIN>() ) {
      assert( allsameid( views... ) );
      return zip( std::forward<typename std::remove_reference_t<IDeds>::view>( views.m_container )... )
          .template view<SKIN>();
    }
  };
} // namespace

template <template <class> class SKIN, typename... IDeds>
static auto myzip( IDeds&&... views ) -> decltype( another_zip_helper::myzip<SKIN>( std::move( views )... ) ) {
  return another_zip_helper::myzip<SKIN>( std::move( views )... );
}

template <typename T>
struct is_IDed {
  bool value = false;
};

template <typename VIEW>
struct is_IDed<IDedSOAContainer<VIEW>> {
  static constexpr bool value = SOA::Utils::is_view<VIEW>::value;
  using view                  = typename std::remove_reference<VIEW>::type;
};

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
    fitres r{100.f, 0.f, 7.f, i};
    foo2.push_back( r );
    foo4.push_back( r );
  }
  for ( auto i : range( 43 ) ) {
    fitqual q{1.f, i};
    foo3.push_back( q );
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
