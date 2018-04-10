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
	bool pathExists(Node start, Node goal);
	list<Node> getPath();
	AStar(Map * map);
	~AStar();
private:
	vector<Node*> getNeighbours(Node * node);
	list<Node> path;
	void construct_path(std::list<Node>& closed, Node * node);
	Map* mapPointer;
};

