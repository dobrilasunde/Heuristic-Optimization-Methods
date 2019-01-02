#include <iostream>
#include <vector>
#include "Data.h"
#include "Track.h"
#include "Vehicle.h"
#include "Optimizer.h"
#include "MinOptimizer.h"

MinOptimizer::MinOptimizer(Data *data) : Optimizer(data) {
	this->_weight_factors.push_back(0.0);
	this->_weight_factors.push_back(0.0);
	this->_weight_factors.push_back(0.0);
}

MinOptimizer::MinOptimizer(Data *data, std::vector<double> &weight_factors) : Optimizer(data, weight_factors) {
}

void MinOptimizer::calculate_number_of_track_pairs() {
	std::vector<Track*> tracks = this->_data->GetTracks();

	for (int i = 0; i < tracks.size(); i++) {
		for (int j = 0; j < tracks.size(); j++) {

		}
	}
}

void MinOptimizer::calculate_number_of_used_tracks() {
}

void MinOptimizer::calculate_unused_space() {
}

void MinOptimizer::calculate_weight_factor_3() {
	
	int total_track_capacity = 0;
	std::vector<Track*> tracks = this->_data->GetTracks();

	for (int i = 0; i < tracks.size(); i++) {
		total_track_capacity += tracks[i]->GetLength();
	}

	std::vector<Vehicle*> vehicles = this->_data->GetVehicles();

	int total_vehicle_length = 0;
	for (int i = 0; i < vehicles.size(); i++) {
		total_vehicle_length += vehicles[i]->GetLength();
	}

	this->_weight_factors[2] = total_track_capacity - total_vehicle_length;
}

double MinOptimizer::calculate_global_goal() {

	double res1 = 1;

	return 0.0;
}
