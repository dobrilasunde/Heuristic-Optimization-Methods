#include <iostream>
#include "Data.h"
#include "Track.h"
#include "Vehicle.h"
#include "Optimizer.h"
#include "MaxOptimizer.h"

MaxOptimizer::MaxOptimizer(Data *data) : Optimizer(data) {
	this->_weight_factors.push_back(0.0f);
	this->_weight_factors.push_back(0.0f);
	this->_goal_values.push_back(0.0f);
	this->_goal_values.push_back(0.0f);
	this->_goal_values.push_back(0.0f);
	this->_worst_value = -200000000;
}

MaxOptimizer::MaxOptimizer(Data *data, std::vector<float>& weight_factors) : Optimizer(data, weight_factors) {
	this->_goal_values.push_back(0.0f);
	this->_goal_values.push_back(0.0f);
	this->_goal_values.push_back(0.0f);
	this->_worst_value = -200000000;
}

void MaxOptimizer::calculate_vehicle_pairs() {
	std::vector<Track*> tracks = this->_data->GetTracks();

	int number_of_pairs = 0;

	for (int i = 0; i < tracks.size(); i++) {
		std::vector<Vehicle*> vehicles = tracks[i]->GetParkedVehicles();
		if (vehicles.empty()) {
			continue;
		}
		for (int j = 0; j < vehicles.size() - 1; j++) {
			if (vehicles[j]->GetSchedule() == vehicles[j + 1]->GetSchedule()) {
				number_of_pairs++;
			}
		}
	}

	this->_goal_values[0] = number_of_pairs;
}

void MaxOptimizer::calculate_weight_factor_1() {
	std::vector<Track*> tracks = this->_data->GetTracks();

	int used_tracks = 0;
	for (int i = 0; i < tracks.size(); i++) {
		if (!tracks[i]->GetParkedVehicles().empty()) {
			used_tracks++;
		}
	}

	this->_weight_factors[0] = this->_data->GetSortedVehicles().size() - used_tracks;
}

void MaxOptimizer::calculate_goal_2() {
	std::vector<Track*> temp_tracks = this->_data->GetTracks();
	std::vector<Track*> tracks;

	for (int i = 0; i < temp_tracks.size(); i++) {
		if (!temp_tracks[i]->GetParkedVehicles().empty()) {
			tracks.push_back(temp_tracks[i]);
		}
	}

	int t = 0;
	
	for (int i = 0; i < tracks.size() - 1; i++) {
		if (tracks[i]->GetParkedVehicles().back()->GetSchedule() == tracks[i + 1]->GetParkedVehicles()[0]->GetSchedule()) {
			t++;
		}
	}

	this->_goal_values[1] = t;
}

void MaxOptimizer::calculate_number_of_used_tracks_minus_1() {
	std::vector<Track*> tracks = this->_data->GetTracks();

	int used_tracks = 0;

	for (int i = 0; i < tracks.size(); i++) {
		if (!tracks[i]->GetParkedVehicles().empty()) {
			used_tracks++;
		}
	}

	this->_weight_factors[1] = used_tracks - 1;
}

void MaxOptimizer::calculate_sum_of_rewards() {
	std::vector<Track*> tracks = this->_data->GetTracks();

	int sum_of_rewards = 0;
	int evaluated_pairs = 0;

	for (int i = 0; i < tracks.size(); i++) {
		std::vector<Vehicle*> vehicles = tracks[i]->GetParkedVehicles();
		if (vehicles.empty()) {
			continue;
		}
		for (int j = 0; j < vehicles.size() - 1; j++) {
			int vr = vehicles[j + 1]->GetDepartureTime() - vehicles[j]->GetDepartureTime();
			if (vr > 20) {
				sum_of_rewards += 10;
			}
			else if (vr < 10) {
				sum_of_rewards += (-4 * (10 - vr));
			}
			else {
				sum_of_rewards += 15;
			}
			evaluated_pairs++;
		}
	}

	this->_goal_values[2] = (1.0f / (15 * evaluated_pairs)) * sum_of_rewards;
}

float MaxOptimizer::calculate_global_goal() {

	this->calculate_vehicle_pairs();
	this->calculate_weight_factor_1();
	this->calculate_goal_2();
	this->calculate_number_of_used_tracks_minus_1();
	this->calculate_sum_of_rewards();

	float res = this->_goal_values[0] * (1.0f / this->_weight_factors[0]);
	res += this->_goal_values[1] * (1.0f / this->_weight_factors[1]);
	res += this->_goal_values[2];

	return res;
}

bool MaxOptimizer::is_better(float *current_best) {
	
	float x = calculate_global_goal();

	if (x < *current_best) {
		return false;
	}

	*current_best = x;
	return true;
}
