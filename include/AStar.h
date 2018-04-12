#pragma once
#include <Node.h>
#include <vector>
#include <list>
#include <algorithm>
#include <Map.h>
using namespace std;
class AStar
{
public:
	//check if the path exsits using A* algorithem
	bool pathExists(Node start, Node goal);
	list<Node> getPath();
	AStar(Map * map);
	~AStar();
private:
	//get all the neighbours around the current node
	vector<Node*> getNeighbours(Node * node);
	//construct the path using the closed list that the A* constructed
	void constructPath(std::list<Node>& closed, Node * node);
	//where the constructed path is stored
	list<Node> path;
	Map* mapPointer;
};

