#ifndef _GRIDCELL_H
#define _GRIDCELL_H

struct GridCell {
	
	GridCell(int x, int z, bool valid)
	{
		this->x = x;
		this->z = z;
		this->valid = valid;
	}
	
	int x;
	int z;
	bool valid;

};


#endif