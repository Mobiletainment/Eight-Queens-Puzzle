#pragma once
#include "precomp.h"
#include "Genome.h"
#include "Game.h"
#include "Helper.h"

Genome::Genome(void)
{
	

	
	//position the genes on a row randomly
	for (int i = 0; i < 8; ++i)
		Genes[i] = Helper::GetRandomNumber();

	EvaluateFitness();
}

void Genome::EvaluateFitness()
{
	//the fitness is determined by the number of non-collisions (no collisions horizontally, vertically or diagonally)
	//at maximum, 28 collisions can happen (worst case):
	//7+6+5+4+3+2+1

	int collisions = 0;
	for(int i = 0; i < n; ++i) //compare every gene with every other for collisions
	{
		for(int j = i+1; j < n; ++j) //we don't need to compare backwards too, just forward (right-looking)
		{
			int dist = abs(Genes[i]-Genes[j]);
			
			//a collision occurs if 2 genes are on the same row OR
			//the absolute distance between the genes matches their grid distance
			if( dist == 0 || dist == j-i)
				++collisions;
		}
	}
	
	fitness = 28 - collisions;
}

Genome::~Genome(void)
{
}
