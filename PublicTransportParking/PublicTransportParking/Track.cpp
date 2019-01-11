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
	float bonus = 0.0f;
	if (!this->mParkedVehicles.empty()) {
		bonus = 0.5f;
	}

	if ((vehicle->GetLength() + mLengthOfParkedVehicles + bonus > (float)(mLength)) || (vehicle->GetCanParkOnTrack(mID-1) != true) || (mCategory != 0 && !mParkedVehicles.empty() && mCategory != vehicle->GetCategory()))
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
		mCategory = vehicle->GetCategory();
		mLengthOfParkedVehicles += vehicle->GetLength();
		if (!mParkedVehicles.empty()) {
			mLengthOfParkedVehicles += 0.5f;
		}
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
			mLengthOfParkedVehicles -= vehicle->GetLength();
			mParkedVehicles.erase(mParkedVehicles.begin() + i);
			if (!mParkedVehicles.empty()) {
				mLengthOfParkedVehicles -= 0.5f;
			}
			if (mParkedVehicles.empty()) {
				mCategory = 0;
			}
			vehicle->SetTrackID(-1);
			break;
		}
	}
	return true;
}

bool Track::CanSwitchVehicles(Vehicle* oldVehicle, Vehicle* newVehicle)
{
	float lengthOfParkedVehicles = mLengthOfParkedVehicles - oldVehicle->GetLength();

	if ((newVehicle->GetLength() + lengthOfParkedVehicles > (float)(mLength)) || (newVehicle->GetCanParkOnTrack(mID-1) != true) || (mCategory != 0 && mParkedVehicles.size() != 1 && mParkedVehicles.size() != 0 && mCategory != newVehicle->GetCategory()))
	{
		return false;
	}

	return true;
}

bool Track::SwitchVehicles(Vehicle* oldVehicle, Vehicle* newVehicle)
{
	if (CanSwitchVehicles(oldVehicle, newVehicle))
	{
		auto iter = std::find(mParkedVehicles.begin(), mParkedVehicles.end(), oldVehicle);
		if (iter != mParkedVehicles.end())
		{
			mLengthOfParkedVehicles -= oldVehicle->GetLength();
			mParkedVehicles.erase(iter);
			mParkedVehicles.push_back(newVehicle);
			mCategory = newVehicle->GetCategory();
			mLengthOfParkedVehicles += newVehicle->GetLength();
			return true;
		}
		else
		{
			return false;
		}
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
