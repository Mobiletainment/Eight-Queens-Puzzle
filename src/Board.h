
#ifndef header_map
#define header_map

#include <array>

using namespace std;

class Board
{
public:
	static Board &GetInstance(); //Singleton
	int GetWidth() { return width; }
	int GetHeight() { return height; }	 
	int GetTileWidth() { return 64; } 
	int GetTileHeight() { return 64; }
	int Turn;
	array<int, 8> QueenPositions;

	vector<vector<char>> Data; //the actual field
	void Draw(CL_GraphicContext &gc);
	~Board();

private:
	Board( CL_GraphicContext &gc); //private constructor for singleton
	static Board *instance;
	vector<vector<CL_Sprite>> tileImages;
	array<CL_Sprite, 8> queenImages;
	CL_SpriteDescription tileSpritedesc;
	CL_SpriteDescription overlaySpriteDesc;
	int width, height;
};

#endif