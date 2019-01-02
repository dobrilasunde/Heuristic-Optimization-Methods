#include "Data.h"
#include <iostream>
#include "Vehicle.h"
int main()
{
	Data* data = new Data();
	data->LoadData("instanca1.txt");

	data->ArrangeVehiclesToTracks();
	data->print_data();

	data->start_optimizing();
}