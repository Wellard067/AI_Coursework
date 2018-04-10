#include "Map.h"


using namespace std;
Map::Map()
{
	float width = 17.5;
	float height = 17.275;
	float xBuffer = 15;
	float yBuffer = 13;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			mapArray[i][j] = new Node(xBuffer + width * j, yBuffer + height * i, width, height, i, j);
		}
	}
}
void Map::DrawMap(sf::RenderTarget &target) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			mapArray[i][j]->DrawNode(target);
		}
	}
}

Map::~Map()
{
}