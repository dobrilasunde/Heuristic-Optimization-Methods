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
	std::vector<class Vehicle*> GetSortedVehicles() const { return mSortedVehicles; }
	std::vector<class Track*> GetTracks() const { return mTracks; }
	class Node* GetRoot() const { return mRoot; }
	void ArrangeVehiclesToTracks();
	void print_data(bool print_unsorted);
	void print_to_file(const std::string &filename);
	void start_optimizing(class Optimizer *optimizer);
	bool SwapUnsortedVehicle(class Vehicle *unsorted, class Vehicle *sorted);
	bool InsertFirstUnsorted();
	bool SwapVehicles(class Vehicle *new_vehicle, class Vehicle *old_vehicle);

private:
	std::vector<class Vehicle*> mVehicles;
	std::vector<class Vehicle*> mUnsortedVehicles;
	std::vector<class Vehicle*> mSortedVehicles;
	std::vector<class Track*> mTracks;
	int mNumberOfVehicles;
	class Node* mRoot;
};

