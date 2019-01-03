#include "Track.h"
#include "Vehicle.h"
#include <string>
#include <iostream>
#include <algorithm>

Track::Track(int length, int id): mLength(length), mID(id), mLengthOfParkedVehicles(0), mCategory(0)
{

}

Track::~Track()
{

}

bool Track::CanPark(Vehicle* vehicle)
{
	if ((vehicle->GetLength() + mLengthOfParkedVehicles + 0.5 > mLength) || (vehicle->GetCanParkOnTrack(mID) != true) || (mCategory != 0 && mCategory != vehicle->GetCategory()))
	{
		return false;
	}
	
	return true;
}

bool Track::ParkVehicle(Vehicle* vehicle)
{
	if (!CanPark(vehicle))
	{
		return false;
	}
	else
	{
		if (mCategory == 0)
		{
			mCategory = vehicle->GetCategory();
		}
		mLengthOfParkedVehicles += vehicle->GetLength() + 0.5;
		mParkedVehicles.push_back(vehicle);
		vehicle->SetTrackID(mID);
	}

	return true;
}

bool Track::UnparkVehicle(Vehicle *vehicle) {
	if (find(mParkedVehicles.begin(), mParkedVehicles.end(), vehicle) == mParkedVehicles.end()) {
		return false;
	}
	for (int i = 0; i < mParkedVehicles.size(); i++) {
		if (vehicle == mParkedVehicles[i]) {
			mLengthOfParkedVehicles -= 0.5 + vehicle->GetLength();
			mParkedVehicles.erase(mParkedVehicles.begin() + i);
			if (mParkedVehicles.empty()) {
				mCategory = 0;
			}
			vehicle->SetTrackID(-1);
			break;
		}
	}
}

bool Track::CanSwitchVehicles(Vehicle* oldVehicle, Vehicle* newVehicle)
{
	float lengthOfParkedVehicles = mLengthOfParkedVehicles - oldVehicle->GetLength();

	if ((newVehicle->GetLength() + lengthOfParkedVehicles > mLength) || (newVehicle->GetCanParkOnTrack(mID) != true) || (mCategory != 0 && mParkedVehicles.size() != 1 && mCategory != newVehicle->GetCategory()))
	{
		return false;
	}

	return true;
}

bool Track::SwitchVehicles(Vehicle* oldVehicle, Vehicle* newVehicle)
{
	auto iter = std::find(mParkedVehicles.begin(), mParkedVehicles.end(), oldVehicle);
	if (iter != mParkedVehicles.end())
	{
		mLengthOfParkedVehicles -= oldVehicle->GetLength();
		mParkedVehicles.erase(iter);
		mParkedVehicles.push_back(newVehicle);
		newVehicle->SetTrackID(mID);
		mCategory = newVehicle->GetCategory();
		mLengthOfParkedVehicles += newVehicle->GetLength();
		return true;
	}

	return false;
}

void Track::SortVehiclesInTrack() {
	
	if (this->mParkedVehicles.empty()) {
		return;
	}
	std::sort(this->mParkedVehicles.begin(), this->mParkedVehicles.end(), Vehicle::CompareByDepartureTime());
}

bool Track::contains(Vehicle *vehicle) {
	if (std::find(this->mParkedVehicles.begin(), this->mParkedVehicles.end(), vehicle) != this->mParkedVehicles.end()) {
		return true;
	}
	else {
		return false;
	}
}
