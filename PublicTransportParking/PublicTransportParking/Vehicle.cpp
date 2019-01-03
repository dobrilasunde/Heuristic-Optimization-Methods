#include "Vehicle.h"
#include <string>
#include <iostream>
Vehicle::Vehicle(int ID)
{
	mVehicleID = ID;
	mTrackID = -1;
}

Vehicle::~Vehicle()
{

}

void Vehicle::SetCanPark(std::vector<std::string> canPark)
{
	for (int i = 0; i < canPark.size(); i++)
	{
		mCanPark.push_back(std::stoi(canPark[i]));
	}
}