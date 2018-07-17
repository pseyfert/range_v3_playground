#include <iostream>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/indices.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>
#include <vector>

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
  int   the_int() { return m_int; }
  float the_float() { return m_float; }
};

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

  for ( auto i : ranges::view::indices( 23 ) ) {
    widgets.emplace_back( i );
    mask.push_back( i % 3 != 1 );
  }

  // too long but works
  for ( auto el : ranges::view::zip( widgets, mask ) | ranges::view::filter( keep_true ) |
                      ranges::view::transform( get_first ) ) {
    std::cout << el.the_int() << std::endl;
  }

  // does the same here, but don't want to know what doesn't work
  for ( auto el : view_masked( ranges::view::zip( widgets, mask ) ) ) {
    std::cout << el.the_int() << std::endl;
  }

  return 0;
}
