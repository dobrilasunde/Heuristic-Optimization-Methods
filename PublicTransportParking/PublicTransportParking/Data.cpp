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
			Node* foundChild = mRoot->FindChild(std::stoi(results[i]));
			if (foundChild != nullptr)
			{
				newChild->AppendChild(foundChild);
			}
			else
			{
				newChild->AppendChild(new Node(mTracks.at(std::stoi(results[i]))));
			}
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
			mSortedVehicles.push_back(mVehicles[i]);
		}
		else
		{
			mUnsortedVehicles.push_back(mVehicles[i]);
			std::cout << "Vehicle " << i << ": " << mVehicles[i]->GetDepartureTime() << " unsorted" << std::endl;
		}
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
	f.open(filename); for (int i = 0; i < this->mTracks.size(); i++) {
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

void Data::start_optimizing(Optimizer *optimizer) {

	float best_value = optimizer->calculate_global_goal();
	float temp_best_value;
	int nothing_happened = 0, iter = 0;
	time_t start_time = time(NULL);
	bool measure_time = true, print_60 = true;
	std::vector<int> taboo_list;
	int taboo_time = 5, current_taboo_time = 0;

	while (nothing_happened < 2*this->mNumberOfVehicles) {
		iter++;

		int swaps = 0;
		bool cond = false;
		/*if (this->mUnsortedVehicles.size() > 0) {
			for (int i = 0; i < mSortedVehicles.size(); i++) {
				for (int j = 0; j < mUnsortedVehicles.size(); j++) {
					if (this->SwapUnsortedVehicle(mUnsortedVehicles[j], mSortedVehicles[i])) {
						std::cout << "Managed to swap an unsorted vehicle!" << std::endl;
						cond = true;
						break;
					}
				}
				if (cond) {
					break;
				}
			}
		}*/

		if (this->mUnsortedVehicles.size() > 0) {
			if (this->InsertFirstUnsorted()) {
				std::cout << "Managed to insert an unsorted vehicle!" << std::endl;
			}
		}
		
		if (current_taboo_time >= taboo_time && !taboo_list.empty()) {
			taboo_list.erase(taboo_list.begin());
			current_taboo_time--;
		}
		temp_best_value = optimizer->get_worst_value();
		int random_index = rand() % this->mSortedVehicles.size();
		while (true) {
			if (find(taboo_list.begin(), taboo_list.end(), random_index) != taboo_list.end()) {
				random_index = rand() % this->mSortedVehicles.size();
			}
			else {
				taboo_list.push_back(random_index);
				break;
			}
		}

		Vehicle *v = this->mSortedVehicles[random_index];
		int best_index = -1;

		for (int i = 0; i < this->mSortedVehicles.size(); i++) {
			if (v->GetTrackID() == mSortedVehicles[i]->GetTrackID()) {
				continue;
			}
			if (v == this->mSortedVehicles[i] || find(taboo_list.begin(), taboo_list.end(), i) != taboo_list.end()) {
				continue;
			}
			if (this->SwapVehicles(v, this->mSortedVehicles[i])) {
				swaps++;
				if (optimizer->is_better(&temp_best_value)) {
					best_index = i;
				}

				this->SwapVehicles(this->mSortedVehicles[i], v);
			}
		}
		 
		if (best_index != -1) {
			this->SwapVehicles(v, this->mSortedVehicles[best_index]);
			nothing_happened = 0;
		}
		else {
			nothing_happened++;
		}
		time_t checkpoint = time(NULL) - start_time;
		if (measure_time) {
			if (checkpoint >= 60 && print_60) {
				std::cout << std::endl;
				this->print_data(true);
				this->print_to_file("res-1m-i1.txt");
				print_60 = false;
			}
			else if (checkpoint >= 300) {
				std::cout << std::endl;
				this->print_data(true);
				measure_time = false;
				this->print_to_file("res-5m-i1.txt");
			}
		}
		/*if (this->mUnsortedVehicles.size() > 0) {
			if (this->InsertFirstUnsorted()) {
				std::cout << "Managed to insert an unsorted vehicle!" << std::endl;
			}
		}*/
		current_taboo_time++;
		//std::cout << "BEST VALUE: " << temp_best_value << ", swaps: " << swaps << std::endl;
		//std::cout << "ITERATION " << iter << " DONE!" << std::endl;
	}
	std::cout << "ALL DONE!" << std::endl;
	this->print_data(true);
	this->print_to_file("res-n-i1.txt");
}

bool Data::SwapUnsortedVehicle(Vehicle *unsorted, Vehicle *sorted) {
	
	std::vector<Node*> nodes;
	std::vector<Node*> nodes_to_search = this->mRoot->GetChildren();
	Node *new_node = nullptr;
	bool found = false;

	while (nodes_to_search.size() > 0) {
		nodes.clear();
		nodes = nodes_to_search;
		nodes_to_search.clear();
		for (int i = 0; i < nodes.size(); i++) {
			if (nodes[i]->GetTrack()->contains(sorted)) {
				new_node = nodes[i];
				found = true;
				break;
			}
			if (!nodes[i]->GetChildren().empty()) {
				std::vector<Node*> temp = nodes[i]->GetChildren();
				nodes_to_search.insert(nodes_to_search.end(), temp.begin(), temp.end());
			}
		}
		if (found) {
			break;
		}
	}

	if (new_node->SwitchVehicleInTrack(sorted, unsorted)) {
		new_node->GetTrack()->SortVehiclesInTrack();
		mSortedVehicles.erase(find(mSortedVehicles.begin(), mSortedVehicles.end(), sorted));
		mSortedVehicles.push_back(unsorted);
		mUnsortedVehicles.erase(find(mUnsortedVehicles.begin(), mUnsortedVehicles.end(), unsorted));
		mUnsortedVehicles.push_back(sorted);
		return true;
	}
	return false;
}

bool Data::InsertFirstUnsorted() {
	
	if (this->mUnsortedVehicles.empty()) {
		return false;
	}

	Vehicle *v = this->mUnsortedVehicles[0];
	this->mUnsortedVehicles.erase(this->mUnsortedVehicles.begin());

	if (this->mRoot->AddVehicleToTrack(v)) {
		mSortedVehicles.push_back(v);
		return true;
	}

	this->mUnsortedVehicles.push_back(v);
	return false;
}

bool Data::SwapVehicles(Vehicle *new_vehicle, Vehicle *old_vehicle) {
	std::vector<Node*> nodes;
	std::vector<Node*> nodes_to_search = this->mRoot->GetChildren();
	Node *new_node = nullptr;
	Node *old_node = nullptr;
	int found_nodes = 0;
	bool was_here_old = false, was_here_new = false;
	while (nodes_to_search.size() > 0) {
		nodes.clear();
		nodes = nodes_to_search;
		nodes_to_search.clear();
		for (int i = 0; i < nodes.size(); i++) {
			if (nodes[i]->GetTrack()->contains(new_vehicle)) {
				if (!was_here_new) {
					new_node = nodes[i];
					was_here_new = true;
					found_nodes++;
				}
			}
			if (nodes[i]->GetTrack()->contains(old_vehicle)) {
				if (!was_here_old) {
					old_node = nodes[i];
					was_here_old = true;
					found_nodes++;
				}
			}
			if (found_nodes > 1) {
				break;
			}
			if (!nodes[i]->GetChildren().empty()) {
				std::vector<Node*> temp = nodes[i]->GetChildren();
				nodes_to_search.insert(nodes_to_search.end(), temp.begin(), temp.end());
			}
		}
		if (found_nodes > 1) {
			break;
		}
	}
	if (new_node->SwitchVehicleInTrack(new_vehicle, old_vehicle)) {
		if (old_node->SwitchVehicleInTrack(old_vehicle, new_vehicle)) {
			new_node->GetTrack()->SortVehiclesInTrack();
			old_node->GetTrack()->SortVehiclesInTrack();
			return true;
		}
		new_node->SwitchVehicleInTrack(old_vehicle, new_vehicle);
	}
	return false;
}
