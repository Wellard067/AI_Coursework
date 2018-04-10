#include "AStar.h"



AStar::AStar(Map * map)
{
	mapPointer = map;
}

AStar::~AStar()
{
}

bool AStar::pathExists(Node start, Node goal)
{
	list<Node*> open;// List of open nodes to be explored
	list<Node> closed;// List of closed nodes which have been explored
	closed.empty();
	Node current_node = start; //Set the current node as a start node
	Node goal_node = goal; //Set the goal node
	if (current_node.equals(goal_node))
	{
		return false;
	}
	current_node.setParentIndex(-1);
	current_node.setGScore(0);
	//Calculate all three scores , f, g, h
	current_node.calculate_H(goal_node);
	current_node.calcuate_G(current_node.getGScore());
	current_node.calculate_F();
	cout << "Start Node : (" << start.getRow() << " " << start.getColumn() << ")" << endl;
	open.push_back(&current_node);
	while (!open.empty())
	{
		//Sort open list using lambda as a comparision function, node with the lowest F score will be the first element in the open list
		open.sort([](const Node *one, const Node * two) {return one->getFScore() < two->getFScore(); });
		//Set current node as the node that has the smallest F score in open list
		current_node = *open.front();
		//If the current node reaches the goal node
		if (current_node.equals(goal_node))
		{
			//Clear the previous path
			path.clear();
			//Construct the path using closed list
			construct_path(closed, &goal);
			cout << "Goal Node : (" << goal.getRow() << " " << goal.getColumn() << ")" << endl;
			return true;
		}
		//Remove node with smallest F score from the openlist
		open.pop_front();
		//Add the current node to closed list
		closed.push_back(current_node);
		//Code for drawing
		current_node.setClosed();
		list<Node>::iterator closedIterator;
		list<Node*>::iterator openIterator;
		//Get current node's neightbours
		for (Node* &neighbourNode : getNeighbours(&current_node))
		{
			bool nodeInClosed = false;
			//Check if the neightbour is in the closed list
			for (closedIterator = closed.begin(); closedIterator != closed.end(); ++closedIterator)
			{
				if (neighbourNode->equals(*closedIterator))
				{
					nodeInClosed = true;
				}
			}
			//If the neighbourNode is not in the closed list
			if (!nodeInClosed)
			{
				bool nodeInOpen = false;
				//Check if the neightbour is in the open list
				for (openIterator = open.begin(); openIterator != open.end(); ++openIterator)
				{
					if (neighbourNode->equals(**openIterator)) {
						nodeInOpen = true;
					}
				}
				//Calculate g, h for neighbourNode.
				neighbourNode->calculate_H(goal_node);
				neighbourNode->calcuate_G(current_node.getGScore());
				//if the node is not in open list
				if (!nodeInOpen)
				{
					open.push_back(neighbourNode);
					neighbourNode->setOpen();
				}
				else
				{
					//Check if the G score can be improve
					if (neighbourNode->canImproveG())
					{
						neighbourNode->improveG();
					}
				}
				neighbourNode->calculate_F();
			}
		}
	}

	return false;
}
vector<Node*> AStar::getNeighbours(Node *node)
{
	//This is where the program works out all the neighbours node are located
	int current_node_row = node->getRow();
	int current_node_column = node->getColumn();

	vector<Node*> neighbors;
	/*
	for loop index
	[i:-1 j:-1][i:0 j: 1][i: 1 j: 1]
	[i:-1 j: 0][i:0 j: 0][i: 1 j: 0]
	[i:-1 j: 1][i:0 j:-1][i: 1 j: 1]*/
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			//Ignore parent node 
			if (i==0&&j==0){continue;}
			//Neighbor row number
			int x = current_node_row + i;
			//Neighbor column number
			int y = current_node_column + j;
			//Variables to prevent out of bounds
			int maxRow = mapPointer->rows;
			int maxColumn = mapPointer->columns;
			//Check if neighbor out of bounds
			if (x > maxRow-1 || y > maxColumn-1) { continue; }
			if (x < 0 || y < 0) { continue; }
			//Get neighbour node
			Node* neighbour = mapPointer->mapArray[x][y];
			//Check if the node is traversable
			if (!neighbour->isTraversable()) { continue; }
			/*
			Result when i+j:
			[-2][1][0]
			[-1][C][1]
			[0][-1][2]
			C = current node*/
			//Assign diagonal cost
			if (i + j == 1 || i + j == -1) { neighbour->setDiagonal(false); }
			if (i + j == 0 || i + j == 2|| i + j == -2) { neighbour->setDiagonal(true); }
			neighbors.push_back(neighbour);
		}
	}
	return neighbors;
}

void AStar::construct_path(list<Node>& closed, Node* node) {
	list<Node>::iterator graphListIter;
	Node* currentNode = node; // Set the current node to goal node
	path.push_front(*currentNode); // Add the current node the the start of the path
								   //Now work backwards throught the closed list reconstructing the path
	for (graphListIter = closed.end(), graphListIter--; graphListIter != closed.begin(); --graphListIter)
	{
		path.push_front(*graphListIter);
		// Remove this node from the closed list
		closed.erase(graphListIter);
		// Start working backward throguht the cloased list again from the end
		graphListIter = closed.end();
	}
	//For testing
	for (Node n : path)
	{
		cout << n.getRow() << " " << n.getColumn() << endl;
	}
}

list<Node> AStar::getPath()
{
	return path;
}

