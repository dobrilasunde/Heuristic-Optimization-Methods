#pragma once

class MinOptimizer : public Optimizer {
private:
public:
	MinOptimizer(Data *data);
	MinOptimizer(Data *data, std::vector<double> &weight_factors);
	void calculate_number_of_track_pairs();
	void calculate_number_of_used_tracks();
	void calculate_unused_space();
	void calculate_weight_factor_3();
	double calculate_global_goal();
};