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

bool Track::ParkVehicle(Vehicle* vehicle)
{
	if ((vehicle->GetLength() + mLengthOfParkedVehicles + 0.5 > mLength) || (vehicle->GetCanParkOnTrack(mID) != true) || (mCategory != 0 && mCategory != vehicle->GetCategory()))
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
}
