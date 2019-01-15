#include "Data.h"
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include "Vehicle.h"
#include "Data.h"
#include "Optimizer.h"
#include "MaxOptimizer.h"
#include "MinOptimizer.h"
#include "OptimizerRunner.h"

int main()
{
	srand(time(NULL));
	int n;
	std::cout << "Input file index: ";
	std::cin >> n;
	Data* data = new Data();

	std::string file_to_load = "instanca";
	file_to_load += std::to_string(n) + ".txt";
	std::ifstream f(file_to_load.c_str());
	if (!f.good()) {
		std::cout << "File doesn't exist!\n";
		return 0;
	}

	data->LoadData(file_to_load);

	data->ArrangeVehiclesToTracks();
	data->print_data(true);

	OptimizerRunner *runner = new OptimizerRunner(new MinOptimizer(data), new MaxOptimizer(data), data);
	runner->start_optimizing(file_to_load);
}
