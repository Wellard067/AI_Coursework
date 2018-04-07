#pragma once
#include "Node.h"
#include "Neighbor.h"
#include <SFML\Graphics.hpp>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
class Map
{
private:
	Node *node;// Reference to the node class
	static const short unsigned rows = 16;
	static const short unsigned columns = 22;
	int width = 35;
	int height = 35;
	static const short unsigned map_width = 780;
	static const short unsigned map_height = 570;
	int xBuffer = 15;
	int yBuffer = 10;
	list<Node> path;
public:
	Map();
	void DrawMap(sf::RenderTarget & target);
	~Map();
	Node* mapArray[rows][columns];
	bool AStar(Node start, Node goal);
	vector<Node*> getNeighbours(Node * node);
	void construct_path(std::list<Node>& closed, Node * node);
	list<Node> getPath();
};

