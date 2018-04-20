/*
It's the interface with the user, loads an instance, applies the algorithm to it, 
and them prints the result on the screen or writes it to a file.
*/
#pragma once
//REVIEW try vector instead of deque in some cases, it might be faster
//FIXME try replacing map for a deque, it's possibly much faster and might even use less memory
/*
Make use of adjacency lists where each list is actually a deque for the fixme above. It might, 
theoretically, use only 2~3MB of RAM, in worst case (vs up to 33MB now). And might be faster 
than using a map as the adjacency list container (try both ways)
*/

#include "load_instance.h"
#include "FInstance.h"
#include "MPrim.h"
#include "CMSA.h"
#include <iostream>
#include <string>
#include <deque>
#include <experimental/filesystem>

using TInstData = std::deque<std::deque<int>>;

std::string GetInputFileLocation();
std::string SetOutputFileLocation(std::string const &Input);
bool DoesDirectoryExists(std::string const &Directory);

int main(int argc, char ** argv)
{
	srand((double)time(NULL));
	std::string InputFile;
	std::string OutputFile = "";

	switch (argc)
	{
	case 2:
		InputFile = argv[1];
		break;
	case 3:
		InputFile = argv[1];
		OutputFile = argv[2];
		break;
	default:
		InputFile = GetInputFileLocation();
		OutputFile = SetOutputFileLocation(InputFile);
	}

	FInstance Instance(LoadInstance(InputFile));
	std::cout << Instance.GetLength() << std::endl;
	//TInstData DataCopy = Instance.GetInstanceData();
	/*
	for (auto Line : DataCopy) {
		for (auto Number : Line) {
			std::cout << Number << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "<<<<<<<<<<<<<<AFTER PRIM>>>>>>>>>>>>>>>" << std::endl;
	*/

//	Instance = MPrim(Instance, 0);
	Instance = CMSA(Instance, 10, 3, 0.2);
	TInstData DataCopy = Instance.GetInstanceData();
	
	for (auto& Line : DataCopy) {
		for (auto& Number : Line) {
			std::cout << Number << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << Instance.GetLength() << std::endl;

	return 0;
}

std::string GetInputFileLocation()
{
	std::string Input = "";
	do {
		std::cout << "Enter input file address: ";
		std::getline(std::cin, Input);
	} while (!DoesDirectoryExists(Input));
	return Input;
}

std::string SetOutputFileLocation(std::string const &Input)
{
	std::string Output;
	std::cout << "Output to file(y/N): ";
	std::getline(std::cin, Output);
	if (tolower(Output[0]) == 'y') {
		Output = Input.substr(0, Input.size() - 4) + "_OUT.txt";
	}
	else {
		Output = "NULL";
	}
	return Output;
}

bool DoesDirectoryExists(std::string const &Directory)
{
	if (!std::experimental::filesystem::exists(Directory)) {
		std::cout << "File/Directory does not exists." << std::endl;
		return false;
	}
	return true;
}
