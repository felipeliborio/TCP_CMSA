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
#include "TCP_LNS.h"
#include <iostream>
#include <string>
#include <deque>
#include <experimental/filesystem>
#include <filesystem>
#include <windows.h>

struct FResult
{
	std::string Instance;
	long long Result = 2147483646;
	long long TimeMS = 2147483646;
};

using TInstData = std::deque<std::deque<int>>;

std::string GetInputFileLocation();
FResult RunInstance(std::string InputFile, std::string OutputFile);
std::string SetOutputFileLocation(std::string const &Input);
bool DoesDirectoryExists(std::string const &Directory);
std::deque<std::string> ListDirectoryFiles(std::string Directory);

int main(int argc, char ** argv)
{
	std::string InputDirectory;
	std::string OutputFile;
	if (argc < 2) {
		InputDirectory = "C:/Users/felip/Documents/Development/PIBIC/TCP_CMSA/Instances";
	}
	else {
		InputDirectory = argv[1];
	}
	if (argc < 3) {
		OutputFile = "C:/Users/felip/Documents/Development/PIBIC/TCP_CMSA/Results/Output.txt";
	}
	else {
		OutputFile = argv[2];
	}

	/*
	for (int i = 0; i < 10; i++) {
		FResult Current = RunInstance(InputDirectory + "/steinc11.txt", OutputFile);
		std::cout << Current.Instance << " " << Current.Result << " " << Current.TimeMS << "\n";
	}*/

	
	for (auto InstanceFile : ListDirectoryFiles(InputDirectory)) {
		FResult Best;
		for (int i = 0; i < 5; i++) {
			FResult Current = RunInstance(InputDirectory + "/" + InstanceFile, OutputFile);
			if (Current.Result < Best.Result) {
				Best = Current;
			}
		}
		std::cout << Best.Instance << " " << Best.Result << " " << Best.TimeMS << "\n";
	}
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	for (auto InstanceFile : ListDirectoryFiles(InputDirectory)) {
		FResult Best;
		for (int i = 0; i < 5; i++) {
			FResult Current = RunInstance(InputDirectory + "/" + InstanceFile, OutputFile);
			if (Current.Result < Best.Result) {
				Best = Current;
			}
		}
		std::cout << Best.Instance << " " << Best.Result << " " << Best.TimeMS << "\n";
	}
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	for (auto InstanceFile : ListDirectoryFiles(InputDirectory)) {
		FResult Best;
		for (int i = 0; i < 5; i++) {
			FResult Current = RunInstance(InputDirectory + "/" + InstanceFile, OutputFile);
			if (Current.Result < Best.Result) {
				Best = Current;
			}
		}
		std::cout << Best.Instance << " " << Best.Result << " " << Best.TimeMS << "\n";
	}
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	for (auto InstanceFile : ListDirectoryFiles(InputDirectory)) {
		FResult Best;
		for (int i = 0; i < 5; i++) {
			FResult Current = RunInstance(InputDirectory + "/" + InstanceFile, OutputFile);
			if (Current.Result < Best.Result) {
				Best = Current;
			}
		}
		std::cout << Best.Instance << " " << Best.Result << " " << Best.TimeMS << "\n";
	}
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "\n";
	for (auto InstanceFile : ListDirectoryFiles(InputDirectory)) {
		FResult Best;
		for (int i = 0; i < 5; i++) {
			FResult Current = RunInstance(InputDirectory + "/" + InstanceFile, OutputFile);
			if (Current.Result < Best.Result) {
				Best = Current;
			}
		}
		std::cout << Best.Instance << " " << Best.Result << " " << Best.TimeMS << "\n";
	}

	return 0;
}

std::deque<std::string> ListDirectoryFiles(std::string Directory) {
	std::deque<std::string> Output;
	std::string search_path = Directory + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				Output.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return Output;
}

FResult RunInstance(std::string InputFile, std::string OutputFile)
{
	srand((unsigned int)time(NULL));
	FResult Output;

	FInstance Instance(LoadInstance(InputFile));

	Instance.SetMaxLinks(99999);
	Instance.SetMaxRouters(99999);

	auto Start = std::chrono::high_resolution_clock::now();

	SimplifyInstance(Instance);
	auto PInstance = Instance;
	PInstance = MPrim(PInstance, 0);
	Instance = LNS(Instance, PInstance);
	auto Elapsed = std::chrono::high_resolution_clock::now() - Start;
	Output.Instance = InputFile.substr(InputFile.size() - 12, 12);
	Output.TimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(Elapsed).count();
	Output.Result = Instance.GetLength();

	return Output;
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
