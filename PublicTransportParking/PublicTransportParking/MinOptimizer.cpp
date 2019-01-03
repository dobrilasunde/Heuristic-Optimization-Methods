#include <iostream>
#include <vector>
#include "Data.h"
#include "Track.h"
#include "Vehicle.h"
#include "Optimizer.h"
#include "MinOptimizer.h"

MinOptimizer::MinOptimizer(Data *data) : Optimizer(data) {
	this->_weight_factors.push_back(0.0f);
	this->_weight_factors.push_back((float)this->_data->GetTracks().size());
	this->_weight_factors.push_back(0.0f);
	this->_goal_values.push_back(0.0f);
	this->_goal_values.push_back(0.0f);
	this->_goal_values.push_back(0.0f);
	this->_worst_value = 200000000;
}

MinOptimizer::MinOptimizer(Data *data, std::vector<float> &weight_factors) : Optimizer(data, weight_factors) {
	this->_goal_values.push_back(0.0f);
	this->_goal_values.push_back(0.0f);
	this->_goal_values.push_back(0.0f);
	this->_worst_value = 200000000;
}

void MinOptimizer::calculate_number_of_track_pairs() {
	std::vector<Track*> tracks_temp = this->_data->GetTracks(), tracks;
	
	for (int i = 0; i < tracks_temp.size(); i++) {
		if (!tracks_temp[i]->GetParkedVehicles().empty()) {
			tracks.push_back(tracks_temp[i]);
		}
	}
	
	int number_of_pairs = 0;

	if (!tracks.empty()) {
		for (int i = 0; i < tracks.size() - 1; i++) {
			if (tracks[i]->GetCategory() != tracks[i + 1]->GetCategory()) {
				number_of_pairs++;
			}
		}
	}
	
	this->_goal_values[0] = (float)number_of_pairs;
}

void MinOptimizer::calculate_number_of_used_tracks() {
	std::vector<Track*> tracks = this->_data->GetTracks();
	
	int used_tracks = 0;

	for (int i = 0; i < tracks.size(); i++) {
		if (!tracks[i]->GetParkedVehicles().empty()) {
			used_tracks++;
		}
	}

	this->_goal_values[1] = (float)used_tracks;
	this->_weight_factors[0] = (float)used_tracks;
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
	this->_goal_values[2] = (float)(total_track_length - used_space);
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

	this->_weight_factors[2] = (float)(total_track_capacity - total_vehicle_length);
}

float MinOptimizer::calculate_global_goal() {
	
	this->calculate_number_of_track_pairs();
	this->calculate_number_of_used_tracks();
	this->calculate_unused_space();
	this->calculate_weight_factor_3();

	float res = 0.0f;

	for (int i = 0; i < this->_goal_values.size(); i++) {
		res += this->_goal_values[i] * (1.0f / this->_weight_factors[i]);
	}

	return res;
}

bool MinOptimizer::is_better(float *current_best) {
	
	float x = calculate_global_goal();

	if (x > *current_best) {
		return false;
	}

	*current_best = x;

	return true;
}
