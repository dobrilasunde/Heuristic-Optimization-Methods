#pragma once

#include <vector>

class MaxOptimizer : public Optimizer {
private:
protected:
public:
	MaxOptimizer(Data *data);
	MaxOptimizer(Data *data, std::vector<float> &weight_factors);
	void calculate_vehicle_pairs();
	void calculate_weight_factor_1();
	void calculate_goal_2();
	void calculate_number_of_used_tracks_minus_1();
	void calculate_sum_of_rewards();
	float calculate_global_goal() override;
	bool is_better(float current_best);
};