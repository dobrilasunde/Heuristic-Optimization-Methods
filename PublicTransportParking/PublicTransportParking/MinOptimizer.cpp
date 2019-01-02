#include <iostream>
#include <vector>
#include "Data.h"
#include "Track.h"
#include "Vehicle.h"
#include "Optimizer.h"
#include "MinOptimizer.h"

MinOptimizer::MinOptimizer(Data *data) : Optimizer(data) {
	this->_weight_factors.push_back(0.0);
	this->_weight_factors.push_back(this->_data->GetTracks().size());
	this->_weight_factors.push_back(0.0);
}

MinOptimizer::MinOptimizer(Data *data, std::vector<float> &weight_factors) : Optimizer(data, weight_factors) {
}

void MinOptimizer::calculate_number_of_track_pairs() {
	std::vector<Track*> tracks = this->_data->GetTracks();

	int number_of_pairs = 0;

	for (int i = 0; i < tracks.size() - 1; i++) {
		for (int j = i + 1; j < tracks.size(); j++) {
			if (tracks[i]->GetCategory() == tracks[j]->GetCategory()) {
				number_of_pairs++;
			}
		}
	}

	this->_goal_values[0] = number_of_pairs;
}

void MinOptimizer::calculate_number_of_used_tracks() {
	std::vector<Track*> tracks = this->_data->GetTracks();
	
	int used_tracks = 0;

	for (int i = 0; i < tracks.size(); i++) {
		if (!tracks[i]->GetParkedVehicles().empty()) {
			used_tracks++;
		}
	}

	this->_goal_values[1] = used_tracks;
	this->_weight_factors[0] = used_tracks;
}

void MinOptimizer::calculate_unused_space() {
	std::vector<Track*> tracks = this->_data->GetTracks();

	float used_space = 0.0f;
	float total_track_length = 0.0f;
	for (int i = 0; i < tracks.size(); i++) {
		if (!tracks[i]->GetParkedVehicles().empty()) {
			std::vector<Vehicle*> veh = tracks[i]->GetParkedVehicles();
			used_space += 0.5f*veh.size() - 0.5f;
			for (int j = 0; j < veh.size(); j++) {
				used_space += veh[j]->GetLength();
			}
			total_track_length += tracks[i]->GetLength();
		}
	}
	this->_goal_values[2] = total_track_length - used_space;
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
