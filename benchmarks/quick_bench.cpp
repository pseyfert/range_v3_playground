#include <benchmark/benchmark.h>
#include <range/v3/view/indices.hpp>
#include <range/v3/view/mask.hpp>

constexpr int fill = 100;

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

auto generate_widgets()
{
  std::vector<Widget> widgets;

  for ( auto i : ranges::view::indices( fill ) ) {
    widgets.emplace_back( i );
  }
  return widgets;
}

template <class T>
auto generate_flags( int i )
{
  std::vector<T> mask;
  for ( auto j : ranges::view::indices( fill ) ) {
    mask.push_back( ( j + 7 * i ) % 4 );
  }
  return mask;
}

static void use_boolbool( benchmark::State& state )
{
  auto                           widgets = generate_widgets();
  std::vector<std::vector<bool>> varlengthmasks;

  varlengthmasks.push_back( generate_flags<bool>( 33 ) );
  int i = 0;
  for ( auto _ : state ) {
    state.PauseTiming();
    i++;
    varlengthmasks.push_back( generate_flags<bool>( i ) );
    varlengthmasks.push_back( generate_flags<bool>( i + 1 ) );
    varlengthmasks.push_back( generate_flags<bool>( i + 3 ) );
    state.ResumeTiming();
    auto asdf = ranges::view::make_bool_vector_or_masker( varlengthmasks );
    for ( auto& el : ranges::view::masker( widgets, asdf ) ) {
      benchmark::DoNotOptimize( el );
    }
  }
}

static void use_bool( benchmark::State& state )
{
  auto                          widgets = generate_widgets();
  std::vector<std::vector<int>> varlengthmasks;

  varlengthmasks.push_back( generate_flags<int>( 33 ) );
  int i = 0;
  for ( auto _ : state ) {
    state.PauseTiming();
    i++;
    varlengthmasks.push_back( generate_flags<int>( i ) );
    varlengthmasks.push_back( generate_flags<int>( i + 1 ) );
    varlengthmasks.push_back( generate_flags<int>( i + 3 ) );
    state.ResumeTiming();
    auto asdf = ranges::view::make_bool_vector_or_masker( varlengthmasks );
    for ( auto& el : ranges::view::masker( widgets, asdf ) ) {
      benchmark::DoNotOptimize( el );
    }
  }
}

static void use_int( benchmark::State& state )
{
  auto                          widgets = generate_widgets();
  std::vector<std::vector<int>> varlengthmasks;

  varlengthmasks.push_back( generate_flags<int>( 33 ) );
  int i = 0;
  for ( auto _ : state ) {
    state.PauseTiming();
    i++;
    varlengthmasks.push_back( generate_flags<int>( i ) );
    varlengthmasks.push_back( generate_flags<int>( i + 1 ) );
    varlengthmasks.push_back( generate_flags<int>( i + 3 ) );
    state.ResumeTiming();
    auto asdf = ranges::view::make_int_vector_or_masker( varlengthmasks );
    for ( auto& el : ranges::view::masker( widgets, asdf ) ) {
      benchmark::DoNotOptimize( el );
    }
  }
}

static void use_int_backend( benchmark::State& state )
{
  auto                          widgets = generate_widgets();
  std::vector<std::vector<bool>> varlengthmasks;

  varlengthmasks.push_back( generate_flags<bool>( 33 ) );
  int i = 0;
  for ( auto _ : state ) {
    state.PauseTiming();
    i++;
    varlengthmasks.push_back( generate_flags<bool>( i ) );
    varlengthmasks.push_back( generate_flags<bool>( i + 1 ) );
    varlengthmasks.push_back( generate_flags<bool>( i + 3 ) );
    state.ResumeTiming();
    auto asdf = ranges::view::make_int_vector_or_masker( varlengthmasks );
    for ( auto& el : ranges::view::masker( widgets, asdf ) ) {
      benchmark::DoNotOptimize( el );
    }
  }
}

auto compute_min = []( const std::vector<double>& v ) -> double {
  return *( std::min_element( std::begin( v ), std::end( v ) ) );
};

// clang-format off
BENCHMARK( use_boolbool )   ->ComputeStatistics( "min", compute_min )/*->ThreadRange( 1, 4 )*/->UseRealTime();
BENCHMARK( use_int )        ->ComputeStatistics( "min", compute_min )/*->ThreadRange( 1, 4 )*/->UseRealTime();
BENCHMARK( use_int_backend )->ComputeStatistics( "min", compute_min )/*->ThreadRange( 1, 4 )*/->UseRealTime();
BENCHMARK( use_bool )       ->ComputeStatistics( "min", compute_min )/*->ThreadRange( 1, 4 )*/->UseRealTime();
// clang-format on

BENCHMARK_MAIN();
