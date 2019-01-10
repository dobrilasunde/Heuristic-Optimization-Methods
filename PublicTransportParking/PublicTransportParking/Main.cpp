#include "Data.h"
#include <ctime>
#include <string>
#include <iostream>
#include "Vehicle.h"
#include "Optimizer.h"
#include "MaxOptimizer.h"
#include "MinOptimizer.h"
#include "OptimizerRunner.h"

int main()
{
	srand(time(NULL));

	Data* data = new Data();

	std::string file_to_load = "instanca3.txt";

	data->LoadData(file_to_load);

	data->ArrangeVehiclesToTracks();
	data->print_data(true);

	//data->start_optimizing(new MinOptimizer(data));
	OptimizerRunner *runner = new OptimizerRunner(new MinOptimizer(data), new MaxOptimizer(data), data);
	runner->start_optimizing(file_to_load);
}
