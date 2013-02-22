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

#pragma once
#include "precomp.h"
#include "Board.h"
#include "Genome.h"
#include "Timer.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Helper.h"


class Game
{ 
private:
	void OnKeyDown(const CL_InputEvent &key, const CL_InputState &state);
	Board *board;
	bool quit;
	CL_DisplayWindow game_display_window;
	void static PlayerHasWonNotification(string winner);
	static string winner;
	long generations;
	Genome *solution;
	char twinsCreated[128];
	ofstream *outputFile;
protected:
	static bool SortByFittest(Genome first, Genome second);
	void CalculateSolution();
	Timer timer;
public:
	static CL_GraphicContext *gc;
	Game( CL_DisplayWindow &window);

	void SortParentsByFitness();

	~Game();
	void Run(CL_DisplayWindow &window);
	vector<Genome> population;
	void OnQuit() { quit = true; }
};
