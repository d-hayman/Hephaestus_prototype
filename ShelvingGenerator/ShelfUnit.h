#pragma once
class ShelfUnit
{
public:
	ShelfUnit();
	~ShelfUnit();
private:
	wchar_t* wszName; //section name
	int iHeight; //height of gap between shelves
	int iWidth; //width of gap between walls
	int iDepth; //depth of shelves
	int iShelfCount; // number of shelves
	int iExtraWalls; // number of additional columns
	ShelfUnit* suNext; // next shelf unit to the right
};

