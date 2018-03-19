#include "Map.h"


using namespace std;
Map::Map()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			node = new Node(xBuffer + width * j, yBuffer + height * i, width, height, i, j);
			mapArray[i][j] = node;
		}
	}
}
//void Map::DrawMap(sf::RenderTarget &target) {
//	for (int i = 0; i < 22; i++) {
//		for (int j = 0; j < 16; j++) {
//			mapArray[j][i]->DrawNode(target);
//		}
//	}
//}

Map::~Map()
{
}

bool Map::AStar(std::list<Node*>& path, Node &start, Node &goal)
{
	list<Node*> open;// List of open nodes to be explored
	list<Node*> closed;// List of closed nodes which have been explored
	closed.empty();
	Node *current_node = &start;
	Node *goal_node = &goal;
	current_node->setParentIndex(-1);
	current_node->setGScore(0);
	//Calculate all three scores , f, g, h
	current_node->calculate_H(goal_node);
	current_node->calcuate_G(current_node->getGScore());
	current_node->calculate_F();
	open.push_back(current_node);
	while (!open.empty())
	{
		//sort open list using lambda as a comparision function
		open.sort([](const Node *one, const Node * two) {return one->getFScore() < two->getFScore(); });
		//Remove node with smallest score from the openlist
		current_node = open.front();
		open.pop_front();
		if (current_node->equals(goal_node))
		{
			path = construct_path(path, closed, &goal);//if the current node  has reached the goal then return path
			return true;
		}
		else
		{
			closed.push_back(current_node);
		}
		list<Node*>::iterator closedIterator;
		list<Node*>::iterator openIterator;
		for (Node* neighbourNode:getNeighbours(current_node))
		{
			bool nodeInClosed = false;
			for (closedIterator = closed.begin(); closedIterator != closed.end();++closedIterator)
			{
				if (neighbourNode->equals(*closedIterator))
				{
					nodeInClosed = true;
				}
			}
			if (!nodeInClosed)
			{
				neighbourNode->calcuate_G(current_node->getGScore());
				neighbourNode->calculate_H(goal_node);
				neighbourNode->calculate_F();
				bool nodeInOpen = false;
				for (openIterator = open.begin(); openIterator != open.end(); ++openIterator)
				{
					if (neighbourNode->equals(*openIterator)) {
						nodeInOpen = true;
					}
				}
				//if the node is not in open list
				if (!nodeInOpen)
				{
					open.push_back(neighbourNode);
					mapArray[neighbourNode->getRow()][neighbourNode->getColumn()]->setClosed();
				}
				else
				{
					Node* openNeighbour = neighbourNode;
					if (neighbourNode->getGScore() < openNeighbour->getGScore()) {
						//Update the g score
						openNeighbour->setGScore(openNeighbour->getGScore());
					}
				}
			}
		}
	}
	return false;
}
std::vector<Node*> Map::getNeighbours(Node * node)
{
	//This is where the program works out all the neighbours node are located
	int current_node_index = node->getIndex();
	int current_node_row = node->getRow();
	int current_node_column = node->getColumn();
	/*[0][1][2]
	  [3][C][4]
	  [5][6][7]
	  C = current node
	*/
	int top_left = 0, top = 1, top_right = 2, left = 3, right = 4, bottom_left = 5, bottom = 6, bottom_right = 7;
	Neighbor top_left_neighbor(current_node_row - 1, current_node_column - 1, true);
	Neighbor top_neighbor(current_node_row, current_node_column - 1, false);
	Neighbor top_right_neighbor(current_node_row+1, current_node_column - 1, true);
	Neighbor left_neighbor(current_node_row - 1, current_node_column, false);
	Neighbor right_neighbor(current_node_row +1, current_node_column, false);
	Neighbor bottom_left_neighbor(current_node_row-1 , current_node_column + 1, true);
	Neighbor bottom_neighbor(current_node_row, current_node_column + 1, false);
	Neighbor bottom_right_neighbor(current_node_row + 1, current_node_column + 1, true);
	vector<Neighbor> neighbors;
	neighbors.push_back(top_left_neighbor);
	neighbors.push_back(top_neighbor);
	neighbors.push_back(top_right_neighbor);
	neighbors.push_back(left_neighbor);
	neighbors.push_back(right_neighbor);
	neighbors.push_back(bottom_left_neighbor);
	neighbors.push_back(bottom_neighbor);
	neighbors.push_back(bottom_right_neighbor);
	//If the current node is on most left hand side row
	if (current_node_row-1<0)
	{
		neighbors[top_left].setExists(false);
		neighbors[left].setExists(false);
		neighbors[bottom_left].setExists(false);
	}
	//If the current node is on the most top column
	if (current_node_column-1<0)
	{
		neighbors[top_left].setExists(false);
		neighbors[top].setExists(false);
		neighbors[top_right].setExists(false);
	}
	//If the current node is on the most right hand side row
	if (current_node_row + 1 > this->rows)
	{
		neighbors[top_right].setExists(false);
		neighbors[right].setExists(false);
		neighbors[bottom_right].setExists(false);
	}
	//If the current node is on the most bottom column
	if (current_node_column + 1 > this->columns)
	{
		neighbors[bottom_right].setExists(false);
		neighbors[bottom].setExists(false);
		neighbors[bottom_right].setExists(false);
	}
	std::vector<Node*> neighbourNodes;
	std::vector<bool> diagonals;
	for (Neighbor n:neighbors)
	{
		if (n.isExists())
		{
			neighbourNodes.push_back(mapArray[n.getRow()][n.getColumns()]);		
			neighbourNodes.front()->setDiagonal(n.isDiagonal());
		}
	}
	//Set parentIndex
	for (Node* neigNode : neighbourNodes)
	{
		neigNode->setParentIndex(node->getIndex());
	}
	return neighbourNodes;
}
std::list<Node*> Map::construct_path(std::list<Node*>& path, std::list<Node*>& closed, Node* node) {
	std::list<Node*>::iterator graphListIter;

	Node* currentNode = node; // Set the current node to goal node
	path.push_front(currentNode); // Add the current node the the start of the path
	closed.pop_back(); // Remove the curretn node from the closed list
					   //Now work backwards throught the closed list reconstructing the path
	for (graphListIter = closed.end(), graphListIter--; graphListIter != closed.begin(); --graphListIter)
	{
		// Add it to the path
		path.push_front(*graphListIter);
		// Remove this node from the closed list
		closed.erase(graphListIter);
		// Start working backward throguht the cloased list again from the end
		graphListIter = closed.end();
	}
	return path;
}
