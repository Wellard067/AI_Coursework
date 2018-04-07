#pragma once
class Neighbor
{
private:
	int row;
	int columns;
	bool diagonal;
	bool traversable;
public:
	Neighbor(int row, int columns, bool diagonal);
	~Neighbor();
	void setTraversable(bool exists);
	bool isTraversable();
	int getRow();
	int getColumns();
	bool isDiagonal();
};

