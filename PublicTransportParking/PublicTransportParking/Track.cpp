#include "Track.h"
#include "Vehicle.h"
#include <string>
#include <iostream>
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
	}

	return true;
}

bool Track::CanSwitchVehicles(Vehicle* oldVehicle, Vehicle* newVehicle)
{
	float lengthOfParkedVehicles = mLengthOfParkedVehicles - oldVehicle->GetLength();

	if ((newVehicle->GetLength() + lengthOfParkedVehicles > mLength) || (newVehicle->GetCanParkOnTrack(mID) != true) || (mCategory != 0 && mCategory != newVehicle->GetCategory()))
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
		mLengthOfParkedVehicles += newVehicle->GetLength();
		return true;
	}

	return false;
}
