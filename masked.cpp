#include <range/v3/all.hpp>
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

int main()
{
  std::vector<Widget> widgets;
  std::vector<bool>   mask;

  for ( auto i : ranges::view::indices( 23 ) ) {
    widgets.emplace_back( i );
    mask.push_back( i % 3 != 1 );
  }
  return 0;
}
