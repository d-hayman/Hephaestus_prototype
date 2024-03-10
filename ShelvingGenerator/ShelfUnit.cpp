#include "stdafx.h"
#include "ShelfUnit.h"


ShelfUnit::ShelfUnit()
{
}


ShelfUnit::~ShelfUnit()
{
	printf("Deleting %s", wszName);
	delete suNext;

}
