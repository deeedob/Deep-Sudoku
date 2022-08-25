#include "../src/img_proc/cv_segmentation.hpp"
#include <filesystem>
#include <opencv4/opencv2/opencv.hpp>
#include <benchmark/benchmark.h>

std::filesystem::path path( std::filesystem::current_path()
	                            .parent_path().parent_path() += "/test/res/Training_002.jpg" );

static QImage img( path.c_str());
static CVSegmentation seg( img );

static void BM_processInstantiation( benchmark::State& state )
{
	for( auto _ : state ) {
		CVSegmentation instance( img );
		instance.process();
	}
}

static void BM_processOnly( benchmark::State& state )
{
	for( auto _ : state ) {
		seg.process();
	}
}

BENCHMARK( BM_processInstantiation )->Repetitions( 50 )->Unit( benchmark::kMillisecond );
BENCHMARK( BM_processOnly )->Repetitions( 50 )->Unit( benchmark::kMillisecond );
BENCHMARK_MAIN();