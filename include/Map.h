#pragma once
#include "Node.h"
#include <SFML\Graphics.hpp>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
class Map
{
private:
public:
	Map();
	~Map();
	static const short unsigned rows = 32;
	static const short unsigned columns = 44;
	//pointer to an 2D array which stores all the nodes we need for A*
	Node* mapArray[rows][columns];
	//draw the map
	void DrawMap(sf::RenderTarget & target);
};

