#include "Data.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include "Vehicle.h"
#include <iterator>
#include "Track.h"
#include <algorithm>
#include "Node.h"
#include "Optimizer.h"

Data::Data()
{
	mRoot = new Node();
}

Data::~Data()
{
	delete mRoot;
	for (int i = 0; i < mTracks.size(); i++) {
		delete mTracks[i];
	}
	mSortedVehicles.clear();
	mUnsortedVehicles.clear();
	for (int i = 0; i < mVehicles.size(); i++) {
		delete mVehicles[i];
	}
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

	std::getline(file, line);
	if (line != "")
	{
		mNumberOfVehicles = std::stoi(line);
	}

	for (int i = 0; i < mNumberOfVehicles; ++i)
	{
		mVehicles.push_back(new Vehicle(i+1));
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

	for (int i = 0; i < mNumberOfVehicles; ++i)
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
			mTracks.push_back(new Track(std::stoi(results[i]), i+1));
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
			newChild = found->AppendChild(new Node(mTracks.at(std::stoi(results[0]) - 1)));
		}
		else
		{
			newChild = mRoot->AppendChild(new Node(mTracks.at(std::stoi(results[0]) - 1)));
		}

		for (int i = 1; i < results.size(); ++i)
		{
			Node* foundChild = mRoot->FindChild(std::stoi(results[i]));
			if (foundChild != nullptr)
			{
				newChild->AppendChild(foundChild);
			}
			else
			{
				newChild->AppendChild(new Node(mTracks.at(std::stoi(results[i]) - 1)));
			}
			mTracks.at(std::stoi(results[0]) - 1)->SetBlockedTracks(mTracks.at(std::stoi(results[i]) - 1));
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

	for (int i = 0; i < mTracks.size(); i++) {
		Node* found = mRoot->FindChild(mTracks[i]->GetID());
		if (found != nullptr) {
			mTracks[i]->SetNode(found);
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
	/*for (int i = 0; i < mVehicles.size(); ++i)
	{
		if (mRoot->AddVehicleToTrack(mVehicles[i]))
		{
			std::cout << "Vehicle " << mVehicles[i]->GetVehicleID() << ": " << mVehicles[i]->GetDepartureTime() << " parked" << std::endl;
			mSortedVehicles.push_back(mVehicles[i]);
		}
		else
		{
			std::cout << "Vehicle " << mVehicles[i]->GetVehicleID() << ": " << mVehicles[i]->GetDepartureTime() << " unsorted" << std::endl;
			mUnsortedVehicles.push_back(mVehicles[i]);
		}
	}*/
	mRoot->AddVehicleToTrack(mVehicles[0]);
	mSortedVehicles.push_back(mVehicles[0]);
	for (int i = 1; i < mVehicles.size(); i++) {
		mUnsortedVehicles.push_back(mVehicles[i]);
	}
}

void Data::print_data(bool print_unsorted) {
	for (int i = 0; i < this->mTracks.size(); i++) {
		std::cout << this->mTracks[i]->GetID() << "(" << this->mTracks[i]->GetCategory() << "):";
		for (int j = 0; j < this->mTracks[i]->GetParkedVehicles().size(); j++) {
			std::cout << " " << this->mTracks[i]->GetParkedVehicles()[j]->GetDepartureTime() 
				<< "(" << this->mTracks[i]->GetParkedVehicles()[j]->GetCategory() << ")";
		}
		std::cout << std::endl;
	}

	if (print_unsorted) {
		std::cout << "Unsorted:";
		for (int i = 0; i < this->mUnsortedVehicles.size(); i++) {
			std::cout << " " << this->mUnsortedVehicles[i]->GetDepartureTime()
				<< "(" << this->mUnsortedVehicles[i]->GetCategory() << ")";
		}
		std::cout << std::endl;
	}
}

void Data::print_to_file(const std::string &filename) {
	std::ofstream f;
	f.open(filename);
	for (int i = 0; i < this->mTracks.size(); i++) {
		f << this->mTracks[i]->GetID() << "(" << this->mTracks[i]->GetCategory() << "):";
		for (int j = 0; j < this->mTracks[i]->GetParkedVehicles().size(); j++) {
			f << " " << this->mTracks[i]->GetParkedVehicles()[j]->GetDepartureTime()
				<< "(" << this->mTracks[i]->GetParkedVehicles()[j]->GetCategory() << ")";
		}
		f << std::endl;
	}

	f << "Unsorted:";
	for (int i = 0; i < this->mUnsortedVehicles.size(); i++) {
		f << " " << this->mUnsortedVehicles[i]->GetDepartureTime()
			<< "(" << this->mUnsortedVehicles[i]->GetCategory() << ")";
	}
	f << std::endl;

	f.close();
}

bool Data::SwapUnsortedVehicle(Vehicle *unsorted, Vehicle *sorted) {
	
	std::vector<Node*> nodes;
	Node *new_node = nullptr;
	
	new_node = mRoot->FindChild(sorted->GetTrackID());

	if (new_node->SwitchVehicleInTrack(sorted, unsorted)) {
		new_node->GetTrack()->SortVehiclesInTrack();
		mSortedVehicles.erase(find(mSortedVehicles.begin(), mSortedVehicles.end(), sorted));
		mSortedVehicles.push_back(unsorted);
		mUnsortedVehicles.erase(find(mUnsortedVehicles.begin(), mUnsortedVehicles.end(), unsorted));
		mUnsortedVehicles.push_back(sorted);
		sorted->SetTrackID(-1);
		unsorted->SetTrackID(new_node->GetTrack()->GetID());
		return true;
	}
	return false;
}

bool Data::InsertUnsorted() {
	
	if (this->mUnsortedVehicles.empty()) {
		return false;
	}

	for (int i = 0; i < this->mUnsortedVehicles.size(); i++) {
		Vehicle *v = this->mUnsortedVehicles[i];
		
		if (this->mRoot->AddVehicleToTrack(v)) {
			mSortedVehicles.push_back(v);
			this->mUnsortedVehicles.erase(this->mUnsortedVehicles.begin() + i);
			return true;
		}
	}

	return false;
}

bool Data::SwapVehicles(Vehicle *new_vehicle, Vehicle *old_vehicle) {
	std::vector<Node*> nodes;
	std::vector<Node*> nodes_to_search = this->mRoot->GetChildren();
	Node *new_node = nullptr;
	Node *old_node = nullptr;
	Node *new_node_temp = nullptr;
	Node *old_node_temp = nullptr;
	int found_nodes = 0;
	bool was_here_old = false, was_here_new = false;

	new_node = mRoot->FindChild(new_vehicle->GetTrackID());
	old_node = mRoot->FindChild(old_vehicle->GetTrackID());

	if (new_node->SwitchVehicleInTrack(new_vehicle, old_vehicle)) {
		if (old_node->SwitchVehicleInTrack(old_vehicle, new_vehicle)) {
			new_node->GetTrack()->SortVehiclesInTrack();
			old_node->GetTrack()->SortVehiclesInTrack();
			new_vehicle->SetTrackID(old_node->GetTrack()->GetID());
			old_vehicle->SetTrackID(new_node->GetTrack()->GetID());
			return true;
		}
		new_node->SwitchVehicleInTrack(old_vehicle, new_vehicle);
	}
	return false;
}
