#pragma once

#define POPULATION_SIZE 1000
#define TOP_PERCENT_POPULATION 0.05

class Helper
{
public:
	Helper(void);
	~Helper(void);
	static int GetRandomNumber();
	static int GetRandomNumberWithinTopPopulation();
};

