#pragma once

#include <vector>

class Optimizer {
private:
protected:
	std::vector<float> _weight_factors;
	std::vector<float> _goal_values;
	class Data *_data;
	float _worst_value;
public:
	Optimizer(class Data *data);
	Optimizer(class Data *data, std::vector<float> &weight_factors);
	virtual void set_weight_factors(std::vector<float> &weight_factors);
	virtual void set_weight_factor_by_index(float factor, int index);
	virtual void add_weight_factor(float factor);
	virtual void set_goal_values(std::vector<float> &goal_values);
	virtual void set_goal_value_by_index(float value, int index);
	virtual void add_goal_value(float value);
	virtual std::vector<float>& get_weight_factors();
	virtual float get_weight_factor_by_index(int index);
	virtual std::vector<float>& get_goal_values();
	virtual float get_goal_value_by_index(int index);
	virtual int get_worst_value();
	virtual float calculate_global_goal() = 0;
	virtual bool is_better(float *current_best) = 0;
};
