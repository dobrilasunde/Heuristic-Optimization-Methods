#pragma once

#include<string>
#include<vector>

class Data
{
public:
	Data();
	~Data();
	bool LoadData(const std::string& fileName);
	bool SortVehiclesByDepartureTime();
	std::vector<class Vehicle*> GetVehicles() const { return mVehicles; }
	std::vector<class Vehicle*> GetUnsortedVehicles() const { return mUnsortedVehicles; }
	std::vector<class Track*> GetTracks() const { return mTracks; }
	class Node* GetRoot() const { return mRoot; }
	void ArrangeVehiclesToTracks();
	void print_data(bool print_unsorted);
	void start_optimizing(class Optimizer *optimizer);

private:
	std::vector<class Vehicle*> mVehicles;
	std::vector<class Vehicle*> mUnsortedVehicles;
	std::vector<class Track*> mTracks;
	class Node* mRoot;
};

