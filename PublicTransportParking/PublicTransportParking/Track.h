#pragma once

#include<vector>

class Track
{

public:
	Track(int length, int mID);
	~Track();

	bool ParkVehicle(class Vehicle* vehicle);
	bool UnparkVehicle(class Vehicle* vehicle);
	bool CanPark(class Vehicle* vehicle);
	bool CanSwitchVehicles(class Vehicle* oldVehicle, class Vehicle* newVehicle);
	bool SwitchVehicles(class Vehicle* oldVehicle, class Vehicle* newVehicle);
	void SortVehiclesInTrack();
	bool contains(class Vehicle *vehicle);

	void SetLength(int length) { mLength = length; }
	void SetBlockedTracks(class Track* blockedTrack) { mBlockedTracks.push_back(blockedTrack); }
	void SetNode(class Node *node) { mNode = node; }

	int GetLength() const { return mLength; }
	int GetCategory() const { return mCategory; }
	int GetID() const { return mID; }
	class Node* GetNode() { return mNode; }
	std::vector<class Vehicle*> GetParkedVehicles() { return mParkedVehicles; }
	std::vector<class Track*> GetBlockedTracks() { return mBlockedTracks; }

private:
	int mLength;
	int mCategory;
	int mID;
	float mLengthOfParkedVehicles;
	class Node *mNode;
	std::vector<class Vehicle*> mParkedVehicles;
	std::vector<class Track*> mBlockedTracks;
};