//With the .lib file included the fbxbinreader should interpret the binary data and set the data to the structs and return the data
#include "FBXImporter.h"
#include <iostream>
int main()
{
	FBXImporter::displayVertices();
	std::cin.get();
}