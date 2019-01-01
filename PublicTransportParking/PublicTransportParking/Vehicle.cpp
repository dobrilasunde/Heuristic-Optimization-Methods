#include "Vehicle.h"
#include <string>
#include <iostream>
Vehicle::Vehicle()
{

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