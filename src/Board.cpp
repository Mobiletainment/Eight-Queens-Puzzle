#include "precomp.h"
#include "Board.h"
#include "Game.h"

using namespace std;

Board *Board::instance = 0;

//Map Initialization
Board::Board(CL_GraphicContext &gc) : width(8), height(8), Turn(0)
{	 
	Data = vector<vector<char>>(height,vector<char>(width));
	tileImages.resize(height, vector<CL_Sprite>(width));

	tileSpritedesc.add_frame(CL_ImageProviderFactory::load("resources/Field1.png")); //odd rows
	tileSpritedesc.add_frame(CL_ImageProviderFactory::load("resources/Field2.png")); //even rows
	
	overlaySpriteDesc.add_frame(CL_ImageProviderFactory::load("resources/Queen.png"));

	for(int row = 0; row < height; ++row)
	{
		for(int column = 0; column < width; ++column)
		{
			Data[row][column] = 0;
			//Initialize Tile Images
			tileImages[row][column] = CL_Sprite(gc, tileSpritedesc);
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		QueenPositions[i] = -1;
		queenImages[i] = CL_Sprite(gc, overlaySpriteDesc);
	}

}

Board &Board::GetInstance() //Singleton
{
	if (!instance)
		instance =  new Board(*Game::gc);
	return *instance;
}

Board::~Board()
{
}

void Board::Draw(CL_GraphicContext &gc)
{
	int tileOffsetX = 0;
	int tileOffsetY = 60;
	int tilePosX = tileOffsetX;
	int tilePosY = tileOffsetY;	

	for(int x = 0; x < height; ++x)
	{	 
		tilePosX = tileOffsetX;

		int index = x % 2;

		for(int y = 0; y < width; ++y)
		{
			//0 -> Leer, 1 -> Gelb, 2 -> rot
			tileImages[x][y].set_frame(index);
			index = (index+1) % 2;
			
			/*
			if(Data[x][y] == 0)
				tileImages[x][y].set_frame(0);				
			else if(Data[x][y] == 'x')
				tileImages[x][y].set_frame(1);				
			else if(Data[x][y] == 'o')
				tileImages[x][y].set_frame(2);				
			*/

			tileImages[x][y].draw(gc, tilePosX, tilePosY);
			tilePosX += Board::GetTileWidth();
		}

		tilePosY += Board::GetTileHeight();
	}

	
	
	if (QueenPositions[0] > -1)
	{
		tilePosX = tileOffsetX + 10;
		tileOffsetY += 10;

		for(int column = 0; column < 8; ++column)
		{
			int row = QueenPositions[column];
			tilePosY = tileOffsetY + row*Board::GetTileHeight();

			queenImages[column].draw(gc, tilePosX, tilePosY);

			tilePosX += Board::GetTileWidth();
		}
	}
}