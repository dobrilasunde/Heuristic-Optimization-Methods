#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include "Track.h"
#include "Data.h"
#include "Vehicle.h"
#include "Node.h"
#include "Optimizer.h"
#include "MinOptimizer.h"
#include "MaxOptimizer.h"
#include "OptimizerRunner.h"

OptimizerRunner::OptimizerRunner(MinOptimizer * min_opt, MaxOptimizer * max_opt, Data * data) {
	this->_min_opt = min_opt;
	this->_max_opt = max_opt;
	this->_data = data;
	this->_best_value_min = this->_min_opt->get_worst_value();
	this->_best_value_max = this->_max_opt->get_worst_value();
}

MinOptimizer* OptimizerRunner::get_min_optimizer() {
	return this->_min_opt;
}

MaxOptimizer* OptimizerRunner::get_max_optimizer() {
	return this->_max_opt;
}

Data* OptimizerRunner::get_data() {
	return this->_data;
}

std::vector<std::string>& OptimizerRunner::get_best_result() {
	return this->_best_result;
}

float OptimizerRunner::get_best_value_min() {
	return this->_best_value_min;
}

float OptimizerRunner::get_best_value_max() {
	return this->_best_value_max;
}

void OptimizerRunner::set_best_result() {
	this->_best_result.clear();
	for (int i = 0; i < this->_data->GetTracks().size(); i++) {
		std::string temp_str = "";
		for (int j = 0; j < this->_data->GetTracks()[i]->GetParkedVehicles().size(); j++) {
			temp_str += std::to_string(this->_data->GetTracks()[i]->GetParkedVehicles()[j]->GetVehicleID()) + " ";
		}
		temp_str = temp_str.substr(0, temp_str.size() - 1);
		this->_best_result.push_back(temp_str);
	}
}

int OptimizerRunner::find_best_index(int index, float temp_best_value_min, float temp_best_value_max,
	int &best_index_min, int &best_index_max) {
	float min_v = this->_min_opt->calculate_global_goal();
	float max_v = this->_max_opt->calculate_global_goal();

	bool compare_min = false, compare_max = false;
	if (min_v < temp_best_value_min) {
		compare_min = true;
	}
	if (max_v > temp_best_value_max) {
		compare_max = true;
	}

	if (compare_max && compare_min) {
		if (fabs(max_v + this->_best_value_max) > fabs(min_v + this->_best_value_min)) {
			best_index_max = index;
			return 1;
		}
		else {
			best_index_min = index;
			return -1;
		}
	}
	else if (compare_max) {
		best_index_max = index;
		return 1;
	}
	else if (compare_min) {
		best_index_min = index;
		return -1;
	}
	return 0;
}

void OptimizerRunner::start_optimizing(const std::string& file_to_load) {
	this->_iter = 0;
	this->_best_value_max = _max_opt->get_worst_value();
	this->_best_value_min = _min_opt->get_worst_value();
	float temp_best_value_min, temp_best_value_max;
	int nothing_happened = 0;
	time_t start_time = time(NULL);
	bool measure_time = true, print_60 = true;
	std::vector<int> taboo_list;
	int taboo_time = this->_data->GetVehicles().size() / 3, current_taboo_time = 0;

	while (nothing_happened < 2 * this->_data->GetTracks().size() * this->_data->GetVehicles().size()) {
		this->_iter++;

		int swaps = 0;
		bool cond = false;

		/* Try to swap an unsorted vehicle with a sorted one. */
		if (this->_data->GetUnsortedVehicles().size() > 0) {
			for (int i = 0; i < this->_data->GetSortedVehicles().size(); i++) {
				for (int j = 0; j < this->_data->GetUnsortedVehicles().size(); j++) {
					if (this->_data->SwapUnsortedVehicle(this->_data->GetUnsortedVehicles()[j], this->_data->GetSortedVehicles()[i])) {
						std::cout << "Managed to swap an unsorted vehicle!" << std::endl;
						cond = true;
						break;
					}
				}
				if (cond) {
					break;
				}
			}
		}

		/* ########### */

		if (current_taboo_time >= taboo_time && !taboo_list.empty()) {
			taboo_list.erase(taboo_list.begin());
			current_taboo_time--;
		}

		temp_best_value_min = this->_min_opt->get_worst_value();
		temp_best_value_max = this->_max_opt->get_worst_value();

		int random_index = rand() % this->_data->GetSortedVehicles().size();
		while (true) {
			if (find(taboo_list.begin(), taboo_list.end(), random_index) != taboo_list.end()) {
				random_index = rand() % this->_data->GetSortedVehicles().size();
			}
			else {
				taboo_list.push_back(random_index);
				break;
			}
		}

		Vehicle *v = this->_data->GetSortedVehicles()[random_index];

		/* Try to insert a sorted vehicle into a different track. */

		Track *original_track = this->_data->GetRoot()->FindChild(v->GetTrackID())->GetTrack();
		original_track->UnparkVehicle(v);
		int best_track_index_min = -1, best_track_index_max = -1;
		int min_or_max = 0;

		for (int i = 0; i < this->_data->GetTracks().size(); i++) {
			if (v->GetTrackID() == this->_data->GetTracks()[i]->GetID()) {
				continue;
			}
			Node *temp_node = this->_data->GetTracks()[i]->GetNode();
			if (temp_node->AddVehicleToTrack(v, false)) {
	
				temp_node->GetTrack()->SortVehiclesInTrack();
				
				int x = this->find_best_index(i, temp_best_value_min, temp_best_value_max, best_track_index_min, best_track_index_max);

				if (x > 0) {
					min_or_max++;
				}
				else if (x < 0) {
					min_or_max--;
				}

				this->_data->GetTracks()[i]->UnparkVehicle(v);
			}

		}

		if (min_or_max > 0 && best_track_index_max != -1) {
			original_track->UnparkVehicle(v);
			this->_data->GetTracks()[best_track_index_max]->GetNode()->AddVehicleToTrack(v);
			this->_data->GetTracks()[best_track_index_max]->SortVehiclesInTrack();
			temp_best_value_max = this->_max_opt->calculate_global_goal();
			if (temp_best_value_max > this->_best_value_max) {
				this->_best_value_max = temp_best_value_max;
				this->set_best_result();
				nothing_happened = 0;
			}
		}
		else if (min_or_max <= 0 && best_track_index_min != -1) {
			original_track->UnparkVehicle(v);
			this->_data->GetTracks()[best_track_index_min]->GetNode()->AddVehicleToTrack(v);
			this->_data->GetTracks()[best_track_index_min]->SortVehiclesInTrack();
			temp_best_value_min = this->_min_opt->calculate_global_goal();
			if (temp_best_value_min < this->_best_value_min) {
				this->_best_value_min = temp_best_value_min;
				this->set_best_result();
				nothing_happened = 0;
			}
		}

		/* ############ */

		int best_index_min = -1, best_index_max = -1;
		min_or_max = 0;

		/* Try to swap two sorted vehicles. */

		for (int i = 0; i < this->_data->GetSortedVehicles().size(); i++) {
			if (v->GetTrackID() == this->_data->GetSortedVehicles()[i]->GetTrackID()) {
				continue;
			}
			if (find(taboo_list.begin(), taboo_list.end(), i) != taboo_list.end()) {
				continue;
			}
			if (this->_data->SwapVehicles(v, this->_data->GetSortedVehicles()[i])) {
				swaps++;
				
				int x = this->find_best_index(i, temp_best_value_min, temp_best_value_max, best_index_min, best_index_max);

				if (x > 0) {
					min_or_max++;
				}
				else if (x < 0) {
					min_or_max--;
				}

				this->_data->SwapVehicles(this->_data->GetSortedVehicles()[i], v);
			}
		}

		if (min_or_max > 0 && best_index_max != -1) {
			this->_data->SwapVehicles(v, this->_data->GetSortedVehicles()[best_index_max]);
			temp_best_value_max = this->_max_opt->calculate_global_goal();
			if (temp_best_value_max > this->_best_value_max) {
				this->_best_value_max = temp_best_value_max;
				this->set_best_result();
				nothing_happened = 0;
			}
			else {
				nothing_happened++;
			}
		}
		else if (min_or_max <= 0 && best_index_min != -1) {
			this->_data->SwapVehicles(v, this->_data->GetSortedVehicles()[best_index_min]);
			temp_best_value_min = this->_min_opt->calculate_global_goal();
			if (temp_best_value_min < this->_best_value_min) {
				this->_best_value_min = temp_best_value_min;
				this->set_best_result();
				nothing_happened = 0;
			}
			else {
				nothing_happened++;
			}
		}
		else {
			nothing_happened++;
		}

		/* ########## */
		
		/* Try to insert an unsorted vehicle */

		if (this->_data->GetUnsortedVehicles().size() > 0) {
			if (this->_data->InsertFirstUnsorted()) {
				std::cout << "Managed to insert an unsorted vehicle!" << std::endl;
			}
		}

		time_t checkpoint = time(NULL) - start_time;
		if (measure_time) {
			if (checkpoint >= 60 && print_60) {
				std::cout << std::endl;
				this->_data->print_data(true);
				std::string tmp = "res-1m-i";
				tmp += file_to_load[8];
				tmp += ".txt";
				this->print_to_file(tmp);
				print_60 = false;
			}
			else if (checkpoint >= 300) {
				std::cout << std::endl;
				this->_data->print_data(true);
				measure_time = false;
				std::string tmp = "res-5m-i";
				tmp += file_to_load[8];
				tmp += ".txt";
				this->print_to_file(tmp);
			}
		}
		current_taboo_time++;
	}
	std::cout << "All done! Time: " << (time(NULL) - start_time)/60.0f << " minutes."<< std::endl;
	this->_data->print_data(true);
	std::string tmp = "res-n-i";
	tmp += file_to_load[8];
	tmp += ".txt";
	this->print_to_file(tmp);
}

void OptimizerRunner::print_to_file(const std::string &filename) {
	std::ofstream f;
	f.open(filename);
	for (int i = 0; i < this->_best_result.size(); i++) {
		f << this->_best_result[i] << std::endl;
	}

	f << "\n";
	f << "Max function: " << this->_best_value_max << "\n";
	f << "Min function: " << this->_best_value_min << "\n";
	f << "Iterations: " << this->_iter << "\n";
	f.close();
}
