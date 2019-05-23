#include "FileExplorer.h"

FileExplorer::FileExplorer()
{

}

FileExplorer::~FileExplorer()
{

}

std::string FileExplorer::nameOfFileToOpen()
{
	std::string nameOfFilePath = "/Assets/FBX/";
	std::string nameOfFile = " ";
	std::string totalFileName = " ";

	std::cout << "Which file would you like to open? Don't write file ending.\n";
	std::getline(std::cin, nameOfFile);

	totalFileName = (nameOfFilePath + nameOfFile + ".fbx");

	return totalFileName;
}

std::string FileExplorer::nameOfFileToSave()
{
	std::string lBinFileName;
	std::cout << "What would you like to name the bin and txt files? Don't write file ending.\n";
	std::getline(std::cin, lBinFileName);

	return lBinFileName;
}
