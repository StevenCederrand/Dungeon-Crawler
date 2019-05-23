#include "FileExplorer.h"

FileExplorer::FileExplorer()
{

}

FileExplorer::~FileExplorer()
{

}

std::string FileExplorer::nameOfFileToOpen()
{
	std::string nameOfFilePath = "\\Assets\\FBX\\";
	std::string nameOfFile = " ";
	std::string totalFileName = " ";

	std::cout << "Which file would you like to open?\n";
	std::getline(std::cin, nameOfFile);

	totalFileName = (nameOfFilePath + nameOfFile);

	return totalFileName;
}