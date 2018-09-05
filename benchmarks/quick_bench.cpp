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

static void fixed_length_runtime_8minus( benchmark::State& state )
{
  auto widgets = generate_widgets();

  // std::vector<int> m0 = generate_flags<int>( 33 );
  // std::vector<int> m1 = generate_flags<int>( 44 );
  // std::vector<int> m2 = generate_flags<int>( 55 );
  // std::vector<int> m3 = generate_flags<int>( 55 );
  // std::vector<int> m4 = generate_flags<int>( 34 );
  // std::vector<int> m5 = generate_flags<int>( 45 );
  // std::vector<int> m6 = generate_flags<int>( 56 );
  // std::vector<int> m7 = generate_flags<int>( 56 );
  std::vector<std::vector<int>> varlengthmasks;
  varlengthmasks.push_back( generate_flags<int>( 33 ) );
  varlengthmasks.push_back( generate_flags<int>( 44 ) );
  varlengthmasks.push_back( generate_flags<int>( 55 ) );
  varlengthmasks.push_back( generate_flags<int>( 55 ) );
  varlengthmasks.push_back( generate_flags<int>( 34 ) );
  varlengthmasks.push_back( generate_flags<int>( 45 ) );
  varlengthmasks.push_back( generate_flags<int>( 56 ) );
  varlengthmasks.push_back( generate_flags<int>( 56 ) );

  for ( auto _ : state ) {
    auto asdf = ranges::view::make_int_vector_or_masker( varlengthmasks );
    for ( auto& el : ranges::view::masker( widgets, asdf ) ) {
      benchmark::DoNotOptimize( el );
    }
  }
}

static void fixed_length_compiletime_8minus( benchmark::State& state )
{
  auto widgets = generate_widgets();

  std::vector<int> m0 = generate_flags<int>( 33 );
  std::vector<int> m1 = generate_flags<int>( 44 );
  std::vector<int> m2 = generate_flags<int>( 55 );
  std::vector<int> m3 = generate_flags<int>( 55 );
  std::vector<int> m4 = generate_flags<int>( 34 );
  std::vector<int> m5 = generate_flags<int>( 45 );
  std::vector<int> m6 = generate_flags<int>( 56 );
  std::vector<int> m7 = generate_flags<int>( 56 );
  // std::vector<std::vector<int>> varlengthmasks;
  // varlengthmasks.push_back(generate_flags<int>( 33 ));
  // varlengthmasks.push_back(generate_flags<int>( 44 ));
  // varlengthmasks.push_back(generate_flags<int>( 55 ));
  // varlengthmasks.push_back(generate_flags<int>( 55 ));
  // varlengthmasks.push_back(generate_flags<int>( 34 ));
  // varlengthmasks.push_back(generate_flags<int>( 45 ));
  // varlengthmasks.push_back(generate_flags<int>( 56 ));
  // varlengthmasks.push_back(generate_flags<int>( 56 ));

  for ( auto _ : state ) {
    // auto asdf = ranges::view::make_int_vector_or_masker( varlengthmasks );
    auto asdf = ranges::view::make_or_masker( m0, m1, m2, m3, m4, m5, m6, m7 );
    for ( auto& el : ranges::view::masker( widgets, asdf ) ) {
      benchmark::DoNotOptimize( el );
    }
  }
}

static void fixed_length_runtime_8plus( benchmark::State& state )
{
  auto widgets = generate_widgets();

  // std::vector<int> m0 = generate_flags<int>( 33 );
  // std::vector<int> m1 = generate_flags<int>( 44 );
  // std::vector<int> m2 = generate_flags<int>( 55 );
  // std::vector<int> m3 = generate_flags<int>( 55 );
  // std::vector<int> m4 = generate_flags<int>( 34 );
  // std::vector<int> m5 = generate_flags<int>( 45 );
  // std::vector<int> m6 = generate_flags<int>( 56 );
  // std::vector<int> m7 = generate_flags<int>( 56 );
  // std::vector<int> m8 = generate_flags<int>( 34 );
  // std::vector<int> m9 = generate_flags<int>( 45 );
  std::vector<std::vector<int>> varlengthmasks;
  varlengthmasks.push_back( generate_flags<int>( 33 ) );
  varlengthmasks.push_back( generate_flags<int>( 44 ) );
  varlengthmasks.push_back( generate_flags<int>( 55 ) );
  varlengthmasks.push_back( generate_flags<int>( 55 ) );
  varlengthmasks.push_back( generate_flags<int>( 34 ) );
  varlengthmasks.push_back( generate_flags<int>( 45 ) );
  varlengthmasks.push_back( generate_flags<int>( 56 ) );
  varlengthmasks.push_back( generate_flags<int>( 56 ) );
  varlengthmasks.push_back( generate_flags<int>( 34 ) );
  varlengthmasks.push_back( generate_flags<int>( 45 ) );

  varlengthmasks.push_back( generate_flags<int>( 35 ) );
  varlengthmasks.push_back( generate_flags<int>( 46 ) );
  varlengthmasks.push_back( generate_flags<int>( 57 ) );
  varlengthmasks.push_back( generate_flags<int>( 57 ) );
  varlengthmasks.push_back( generate_flags<int>( 36 ) );
  varlengthmasks.push_back( generate_flags<int>( 47 ) );
  varlengthmasks.push_back( generate_flags<int>( 58 ) );
  varlengthmasks.push_back( generate_flags<int>( 58 ) );
  varlengthmasks.push_back( generate_flags<int>( 36 ) );
  varlengthmasks.push_back( generate_flags<int>( 47 ) );

  for ( auto _ : state ) {
    auto asdf = ranges::view::make_int_vector_or_masker( varlengthmasks );
    for ( auto& el : ranges::view::masker( widgets, asdf ) ) {
      benchmark::DoNotOptimize( el );
    }
  }
}

static void fixed_length_compiletime_8plus( benchmark::State& state )
{
  auto widgets = generate_widgets();

  std::vector<int> m0 = generate_flags<int>( 33 );
  std::vector<int> m1 = generate_flags<int>( 44 );
  std::vector<int> m2 = generate_flags<int>( 55 );
  std::vector<int> m3 = generate_flags<int>( 55 );
  std::vector<int> m4 = generate_flags<int>( 34 );
  std::vector<int> m5 = generate_flags<int>( 45 );
  std::vector<int> m6 = generate_flags<int>( 56 );
  std::vector<int> m7 = generate_flags<int>( 56 );
  std::vector<int> m8 = generate_flags<int>( 34 );
  std::vector<int> m9 = generate_flags<int>( 45 );

  std::vector<int> m10 = generate_flags<int>( 35 );
  std::vector<int> m11 = generate_flags<int>( 46 );
  std::vector<int> m12 = generate_flags<int>( 57 );
  std::vector<int> m13 = generate_flags<int>( 57 );
  std::vector<int> m14 = generate_flags<int>( 36 );
  std::vector<int> m15 = generate_flags<int>( 47 );
  std::vector<int> m16 = generate_flags<int>( 58 );
  std::vector<int> m17 = generate_flags<int>( 58 );
  std::vector<int> m18 = generate_flags<int>( 36 );
  std::vector<int> m19 = generate_flags<int>( 47 );
  // std::vector<std::vector<int>> varlengthmasks;
  // varlengthmasks.push_back(generate_flags<int>( 33 ));
  // varlengthmasks.push_back(generate_flags<int>( 44 ));
  // varlengthmasks.push_back(generate_flags<int>( 55 ));
  // varlengthmasks.push_back(generate_flags<int>( 55 ));
  // varlengthmasks.push_back(generate_flags<int>( 34 ));
  // varlengthmasks.push_back(generate_flags<int>( 45 ));
  // varlengthmasks.push_back(generate_flags<int>( 56 ));
  // varlengthmasks.push_back(generate_flags<int>( 56 ));
  // varlengthmasks.push_back(generate_flags<int>( 34 ));
  // varlengthmasks.push_back(generate_flags<int>( 45 ));

  for ( auto _ : state ) {
    // auto asdf = ranges::view::make_int_vector_or_masker( varlengthmasks );
    auto asdf = ranges::view::make_or_masker( m0, m1, m2, m3, m4, m5, m6, m7, m8, m9,
                                              m10, m11, m12, m13, m14, m15, m16, m17, m18, m19 );
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

BENCHMARK(fixed_length_runtime_8minus)    ->ComputeStatistics( "min", compute_min )/*->ThreadRange( 1, 4 )*/->UseRealTime();
BENCHMARK(fixed_length_runtime_8plus)     ->ComputeStatistics( "min", compute_min )/*->ThreadRange( 1, 4 )*/->UseRealTime();
BENCHMARK(fixed_length_compiletime_8minus)->ComputeStatistics( "min", compute_min )/*->ThreadRange( 1, 4 )*/->UseRealTime();
BENCHMARK(fixed_length_compiletime_8plus) ->ComputeStatistics( "min", compute_min )/*->ThreadRange( 1, 4 )*/->UseRealTime();
// clang-format on

BENCHMARK_MAIN();
