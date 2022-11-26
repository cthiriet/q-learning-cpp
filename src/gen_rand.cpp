#include <random>
#include "gen_rand.hpp"
using namespace std;

double randomNumber(double upperBound)
{
  random_device rd;
  default_random_engine re(rd());
  uniform_real_distribution<double> distrib(0, upperBound);
  double p = distrib(re);
  return p;
};