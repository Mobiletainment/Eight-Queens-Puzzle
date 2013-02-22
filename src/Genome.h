#pragma once

#include "precomp.h"
#include <array>

using namespace std;

#define n 8

class Genome
{
public:
	Genome(void);
	~Genome(void);
	int GetFitness() {return fitness;}
	void EvaluateFitness();
	//int* GetGenes() { return Genes; } //TODO: getter and setter for Genes
	int Genes[8]; //we have 8 genes (one for each column), the value describes in which row the gene is on the field (for the according column)
private:
	
	int fitness;

};

