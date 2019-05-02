#ifndef _GRIDCELL_H
#define _GRIDCELL_H

class GameObject;

struct GridCell {


	struct CellInfo {
		bool occupied = false;
		GameObject* occupant = nullptr;
	};


	GridCell(int x, int z, bool valid)
	{
		this->x = x;
		this->z = z;
		this->valid = valid;
	}

	void setOccupied(GameObject* occupant)
	{
		info.occupant = occupant;
		info.occupied = true;
	}
	
	void setFree()
	{
		info.occupant = nullptr;
		info.occupied = false;
	}

	int x;
	int z;
	bool valid;
	CellInfo info;
};


#endif