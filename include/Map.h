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
	static const short unsigned rows = 32;
	static const short unsigned columns = 44;
	Map();
	void DrawMap(sf::RenderTarget & target);
	~Map();
	Node* mapArray[rows][columns];
};

