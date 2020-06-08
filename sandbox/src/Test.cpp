/*
This is a prototype simulation that uses the 'molecool' simulation engine.
*/

//#define BOOST_TEST_MODULE mytests
//#include <boost/test/included/unit_test.hpp>
#include <mkl.h>
#include <omp.h>
#include <array>
#include "test.h"
#include "molecool.h"

class Sandbox : public molecool::Simulation {
	
public:
	Sandbox() {};
	~Sandbox() {};
};

// function called by the engine
molecool::Simulation* molecool::createSimulation() {
	//---------------------------------------------------------------------
	// user/client simulation code run before simulation proceeds goes here

	//---------------------------------------------------------------------
	return new Sandbox();
}

/*
BOOST_AUTO_TEST_CASE(myTestCase)
{
  ///////////////////////////////////////////////
  // test connection to 'molecool' library
  molecool::Print();

  ///////////////////////////////////////////////
  // boost library test
  BOOST_TEST(1 == 1);
  BOOST_TEST(true);

  ///////////////////////////////////////////////
  // quick test of MKL random number generation
  int seedy = (int)time(0);							// current time in seconds as a seed
  VSLStreamStatePtr stream;							// stream for random number generation
  vslNewStream(&stream, VSL_BRNG_MCG31, seedy); 	// stream, generator type, seed
  const int METHOD = 0;
  const int nValues = 10;
  std::array<double, nValues> randomArrayTarget;
  double mean = 0;
  double width = 1;
  double d = vdRngGaussian(METHOD, stream, nValues, randomArrayTarget.data(), mean, width);

  // print the array the old-fashioned way
  for (int i = 0; i < nValues; ++i) {
	  printf("value %d = %f\n", i, randomArrayTarget.at(i));
  }

  // or, printing the C++11 way using a range-based for loop
  // actually awkward to extract the current index in this case
  for (const auto& rand : randomArrayTarget) {
	  printf("value %I64u = %f\n", &rand - &(randomArrayTarget[0]), rand);
  }


  ///////////////////////////////////////////////
  // quick test of openmp

  printf("Starting openmp test!\n");
  //omp_set_dynamic(0);		// Explicitly disable dynamic teams
  omp_set_num_threads(8);	// Use N threads for all consecutive parallel regions
  #pragma omp parallel
  {
	  // This code will run on each thread (with num/id = 0..N-1)
	  printf("Running on thread %d\n", omp_get_thread_num());
  }

  // We're out of the parallelized section.
  // Therefore, this should execute only once
  printf("Finished!\n");

}
*/
