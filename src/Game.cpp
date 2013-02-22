/*
 * 8 Queens Puzzle
 * Copyright (C) 2012 David Pertiller <http://pertiller.me>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "precomp.h"
#include "Game.h"
#include "Helper.h"

using namespace std;

CL_GraphicContext *Game::gc;
string Game::winner;

const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%H.%M.%S.csv", &tstruct);

	return buf;
}

Game::Game(CL_DisplayWindow &window)
{
	gc = &window.get_gc();
	board = &Board::GetInstance();

	solution = 0;
	generations = 0;

	sprintf_s(twinsCreated, "Press enter to find a solution");

	outputFile = new ofstream(currentDateTime());

	/* initialize random seed: */

	//adjust vector to population size
	population.resize(POPULATION_SIZE);	
}

void Game::CalculateSolution()
{
	generations = 1;
	
	//generate new population
	for (int i = 0; i < POPULATION_SIZE; ++i)
		population[i] = Genome();

	SortParentsByFitness();
	
	int maxFitness = population[0].GetFitness();
	
	while (true)
	{
		//marry two genomes
		int parent1Index, parent2Index;

		do 
		{
			parent1Index = Helper::GetRandomNumberWithinTopPopulation();
			parent2Index = Helper::GetRandomNumberWithinTopPopulation();
		} while (parent1Index == parent2Index);

		Genome parent1 = population[parent1Index];
		Genome parent2 = population[parent2Index];

		//do a one-point crossover on parents and create 2 children by combining the chromosome halfs of the parents
		Genome children1;
		Genome children2;

		for (int i = 0; i < 4; i++)
		{
			children1.Genes[i] = parent2.Genes[i];
			children2.Genes[i] = parent1.Genes[i];
		}

		for (int i = 4; i < 8; i++)
		{
			children1.Genes[i] = parent1.Genes[i];
			children2.Genes[i] = parent2.Genes[i];
		}

		/* //Tracking fitness
		children1.EvaluateFitness();
		children2.EvaluateFitness();
		*/

		//1st Mutation randomly
		children1.Genes[Helper::GetRandomNumber()] = Helper::GetRandomNumber();
		children2.Genes[Helper::GetRandomNumber()] = Helper::GetRandomNumber();

		//children1.Genes[Helper::GetRandomNumber()] = Helper::GetRandomNumber();
		//children2.Genes[Helper::GetRandomNumber()] = Helper::GetRandomNumber();
		/*
		//2nd Mutation changes a gene specifically to match a missing one
		int missingNumberChild1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
		int missingNumberChild2[8] = {0, 0, 0, 0, 0, 0, 0, 0};

		//count occurrence of identical genes
		for (int i = 0; i < 4; ++i)
		{
			++missingNumberChild1[children1.Genes[i]];
			++missingNumberChild2[children2.Genes[i]];
		}

		//swap a gene that occurs twice or more with one that doesn't occur
		int nonOccuringGeneChild1 = 0, nonOccuringGeneChild2 = 0;
		int multipleOccuringGeneChild1 = 0, multipleOccuringGeneChild2 = 0; 
		for (int i = 0; i < 4; ++i)
		{
			if (missingNumberChild1[i] == 0)
				nonOccuringGeneChild1 = i;
			else if (missingNumberChild1[i] >= 2)
				multipleOccuringGeneChild1 = i;

			if (missingNumberChild2[i] == 0)
				nonOccuringGeneChild2 = i;
			else if (missingNumberChild2[i] >= 2)
				multipleOccuringGeneChild2 = i;
		}

		for (int i = 0; i < 8; ++i)
		{
			if (children1.Genes[i] == multipleOccuringGeneChild1)
			{
				children1.Genes[i] = nonOccuringGeneChild1;
				break;
			}
		}

		for (int i = 0; i < 8; ++i)
		{
			if (children2.Genes[i] == multipleOccuringGeneChild2)
			{
				children2.Genes[i] = nonOccuringGeneChild2;
				break;
			}
		}
		*/

		children1.EvaluateFitness();
		children2.EvaluateFitness();

		if (children1.GetFitness() > maxFitness)
			maxFitness = children1.GetFitness();
		else if (children2.GetFitness() > maxFitness)
			maxFitness = children2.GetFitness();


		//*outputFile << maxFitness << ";";

		//replace parents by their children
		population[parent1Index] = children1;
		population[parent2Index] = children2;

		if (children1.GetFitness() == 28)
		{
			solution = &population[parent1Index];
			break;
		}
		else if (children2.GetFitness() == 28)
		{	
			solution = &population[parent2Index];
			break;
		}

		++generations;
	}
}

void Game::SortParentsByFitness()
{
	sort(population.begin(), population.end(), &SortByFittest);
}

bool Game::SortByFittest(Genome first, Genome second)
{
	return first.GetFitness() > second.GetFitness();
}

Game::~Game()
{
	if (outputFile->is_open())
		outputFile->close();

	delete outputFile;
}

void Game::Run(CL_DisplayWindow &window){
	game_display_window = window;

	*gc = window.get_gc();

	quit = false;
	CL_Slot slot_key_down = window.get_ic().get_keyboard().sig_key_down().connect(this, &Game::OnKeyDown);
	
	//Set up font
	CL_FontDescription system_font_desc;
	system_font_desc.set_typeface_name("courier new");
	CL_Font_System system_font(*gc, system_font_desc);
	system_font.set_font_metrics(CL_FontMetrics(7.0f));
	/*
	for (int i = 0; i < 10000; ++i)
	{
		//timer.start();
		CalculateSolution();
		//timer.stop();

		*outputFile << generations << '\n';
	}
	
	outputFile->flush();
	*/
	
	while (!quit)
	{	
		gc->clear(CL_Colorf(0, 0, 0));

		
		//Draw column numbers
		board->Draw(*gc); 

		system_font.draw_text(*gc, 20, 50, twinsCreated);

		if(!winner.empty())
		{
			system_font.draw_text(*gc, 10, 60, winner);
		}
		window.flip(0);
		CL_KeepAlive::process();
	}	
	
}



void Game::PlayerHasWonNotification(string whosTheWinner)
{
	if (winner.empty()) //only assign winner the first time a winner is determined
		winner = whosTheWinner;
}

void Game::OnKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	if (key.id == CL_KEY_ESCAPE)
		quit = true;

	else if (key.id == CL_KEY_ENTER)
	{
		timer.start();
		CalculateSolution();
		*outputFile << '\n';
		timer.stop();
	
		for (int i = 0; i < 8; i++)
			board->QueenPositions[i] = solution->Genes[i];

		sprintf_s(twinsCreated, "#Twins created (needed): %d, t=%.0lfms", generations, timer.getElapsedTimeInMilliSec());
	}

	else if (key.id == CL_KEY_1)
	{
		for (int i = 0; i < 1000; ++i)
		{
			//timer.start();
			CalculateSolution();
			//timer.stop();

			*outputFile << generations << '\n';
		}

		for (int i = 0; i < 8; i++)
			board->QueenPositions[i] = solution->Genes[i];
	}
}