#pragma once
class Neighbor
{
private:
	int row;
	int columns;
	bool diagonal;
	bool exists;
public:
	Neighbor(int row, int columns, bool diagonal);
	~Neighbor();
	void setExists(bool exists);
	bool isExists();
	int getRow();
	int getColumns();
	bool isDiagonal();
};

