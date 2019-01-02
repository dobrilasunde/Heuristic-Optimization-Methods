#pragma once

#include <vector>

class Optimizer {
private:
protected:
	std::vector<double> _weight_factors;
	std::vector<double> _goal_values;
	class Data *_data;
public:
	Optimizer(class Data *data);
	Optimizer(class Data *data, std::vector<double> &weight_factors);
	virtual void set_weight_factors(std::vector<double> &weight_factors);
	virtual void set_weight_factor_by_index(double factor, int index);
	virtual void add_weight_factor(double factor);
	virtual void set_goal_values(std::vector<double> &goal_values);
	virtual void set_goal_value_by_index(double value, int index);
	virtual void add_goal_value(double value);
	virtual std::vector<double>& get_weight_factors();
	virtual double get_weight_factor_by_index(int index);
	virtual std::vector<double>& get_goal_values();
	virtual double get_goal_value_by_index(int index);
	virtual double calculate_global_goal() = 0;
};
