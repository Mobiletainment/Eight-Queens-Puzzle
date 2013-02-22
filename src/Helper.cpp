#include "precomp.h"
#include "Helper.h"

using namespace std;

static random_device seed;
static mt19937 gen(seed());
static uniform_int<> dist(0,7);
static uniform_int<> distTop(0,(int)(POPULATION_SIZE * TOP_PERCENT_POPULATION - 1));

static function<int()> rnd = bind(dist, gen);
static function<int()> rndTopPop = bind(distTop, gen);

Helper::Helper(void)
{
}

int Helper::GetRandomNumber()
{
	return rnd();
}

int Helper::GetRandomNumberWithinTopPopulation()
{
	return rndTopPop();
}

Helper::~Helper(void)
{
}
