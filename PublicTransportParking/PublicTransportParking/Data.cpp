#include "Data.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Vehicle.h"
#include <iterator>
#include "Track.h"
#include <algorithm>
#include "Node.h"

Data::Data()
{
	mRoot = new Node();
}

Data::~Data()
{

}

bool Data::LoadData(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		std::cout << "File not found: Data::LoadData" << std::endl;
		return false;
	}

	std::string line;

	int numberOfVehicles;
	std::getline(file, line);
	if (line != "")
	{
		numberOfVehicles = std::stoi(line);
	}

	for (int i = 0; i < numberOfVehicles; ++i)
	{
		mVehicles.push_back(new Vehicle());
	}

	int numberOfTracks;
	std::getline(file, line);
	if (line != "")
	{
		numberOfTracks = std::stoi(line);
	}

	std::string lengths;
	std::getline(file, line);
	std::getline(file, line);

	if (line != "")
	{
		std::istringstream iss(line);
		std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

		for (int i = 0; i < mVehicles.size(); ++i)
		{
			mVehicles[i]->SetLength(std::stoi(results[i]));
		}
	}

	std::getline(file, line);
	std::getline(file, line);

	if (line != "")
	{
		std::istringstream iss(line);
		std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

		for (int i = 0; i < mVehicles.size(); ++i)
		{
			mVehicles[i]->SetCategory(std::stoi(results[i]));
		}
	}

	std::getline(file, line);

	for (int i = 0; i < numberOfVehicles; ++i)
	{
		std::getline(file, line);
		if (line != "")
		{
			std::istringstream iss(line);
			std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
			mVehicles[i]->SetCanPark(results);
		}
	}

	std::getline(file, line);
	std::getline(file, line);

	if (line != "")
	{
		std::istringstream iss(line);
		std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

		for (int i = 0; i < numberOfTracks; ++i)
		{
			mTracks.push_back(new Track(std::stoi(results[i]), i));
		}
	}

	std::getline(file, line);
	std::getline(file, line);

	if (line != "")
	{
		std::istringstream iss(line);
		std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

		for (int i = 0; i < mVehicles.size(); ++i)
		{
			mVehicles[i]->SetDepartureTime(std::stoi(results[i]));
		}
	}

	std::getline(file, line);
	std::getline(file, line);

	if (line != "")
	{
		std::istringstream iss(line);
		std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

		for (int i = 0; i < mVehicles.size(); ++i)
		{
			mVehicles[i]->SetSchedule(std::stoi(results[i]));
		}
	}

	std::getline(file, line);

	//Creating a tree
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

		Node* found = mRoot->FindChild(std::stoi(results[0]));
		Node* newChild;
		if (found != nullptr)
		{
			newChild = found->AppendChild(new Node(mTracks.at(std::stoi(results[0]))));
		}
		else
		{
			newChild = mRoot->AppendChild(new Node(mTracks.at(std::stoi(results[0]))));
		}

		for (int i = 1; i < results.size(); ++i)
		{
			newChild->AppendChild(new Node(mTracks.at(std::stoi(results[i]))));
			mTracks.at(std::stoi(results[0]))->SetBlockedTracks(mTracks.at(std::stoi(results[i])));
		}
	}

	for (int i = 0; i < mTracks.size(); ++i)
	{
		Node* found = mRoot->FindChild(mTracks[i]->GetID());
		if (found != nullptr)
		{
			continue;
		}
		else
		{
			mRoot->AppendChild(new Node(mTracks[i]));
		}
	}

	mRoot->SortByNumberOfChildren();
	mRoot->Print();
	std::cout << std::endl;

	return true;
}

bool Data::SortVehiclesByDepartureTime()
{
	if (mVehicles.empty())
	{
		return false;
	}

	std::sort(mVehicles.begin(), mVehicles.end(), Vehicle::CompareByDepartureTime());
	return true;
}

void Data::ArrangeVehiclesToTracks()
{
	SortVehiclesByDepartureTime();
	mRoot->SortByNumberOfChildren();
	for (int i = 0; i < mVehicles.size(); ++i)
	{
		if (mRoot->AddVehicleToTrack(mVehicles[i]))
		{
			std::cout << "Vehicle " << i << ": " << mVehicles[i]->GetDepartureTime() << " parked" << std::endl;
		}
	}
}
