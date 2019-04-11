#ifndef _WRITE_CUSTOM_FILE_H
#define _WRITE_CUSTOM_FILE_H

#define DELTA 0.0001
#define EQUAL(A,B) (abs((A)-(B)) < DELTA) ? true:false

#include <iostream>
#include <fstream>
#include "HeaderForFileWriting.h"

class WriteCustomFile
{
private:

public:
	WriteCustomFile();
	~WriteCustomFile();
	void CreateCustomFile();
};



#endif