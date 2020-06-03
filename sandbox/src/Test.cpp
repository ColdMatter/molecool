#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>
#include <mkl.h>


BOOST_AUTO_TEST_CASE(myTestCase)
{
  BOOST_TEST(1 == 1);
  BOOST_TEST(true);

  int seedy = (int)time(0);							// current time in seconds as a seed
  VSLStreamStatePtr stream;							// stream for random number generation
  vslNewStream(&stream, VSL_BRNG_MCG31, seedy); 	// stream, generator type, seed 

  // random number generation
  const int METHOD = 0;
  const int nValues = 10;
  double randomArrayTarget[nValues];
  double mean = 0;
  double width = 1;
  double d = vdRngGaussian(METHOD, stream, nValues, randomArrayTarget, mean, width);
  
  for (int i = 0; i < nValues; ++i) {
	  printf("value %d = %f\n", i, randomArrayTarget[i]);
  }
}