#pragma once
#include "Node.h"
#include "Neighbor.h"
#include <SFML\Graphics.hpp>
#include <list>
#include <vector>
#include <algorithm>
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
public:
	Map();
	~Map();
	Node* mapArray[rows][columns];
	bool AStar(std::list<Node*>& path,Node &start, Node &goal);
	std::vector<Node*> getNeighbours(Node * node);
	std::list<Node*> construct_path(std::list<Node*>& path, std::list<Node*>& closed, Node * node);
};

