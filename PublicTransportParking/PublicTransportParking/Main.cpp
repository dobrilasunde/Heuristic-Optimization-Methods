#include "Data.h"
#include <iostream>
#include "Vehicle.h"
#include "Optimizer.h"
#include "MaxOptimizer.h"
#include "MinOptimizer.h"

int main()
{
	Data* data = new Data();
	data->LoadData("instanca1.txt");

	data->ArrangeVehiclesToTracks();
	data->print_data(true);

	data->start_optimizing(new MinOptimizer(data));
}
