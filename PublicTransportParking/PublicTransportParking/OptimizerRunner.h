#pragma once

#include <vector>
#include <string>

class OptimizerRunner {
private:
	class MinOptimizer *_min_opt;
	class MaxOptimizer *_max_opt;
	class Data *_data;
	std::vector<std::string> _best_result;
	float _best_value_min;
	float _best_value_max;
	int _iter;
public:
	OptimizerRunner(class MinOptimizer *min_opt, class MaxOptimizer *max_opt, class Data *data);
	class MinOptimizer* get_min_optimizer();
	class MaxOptimizer* get_max_optimizer();
	class Data* get_data();
	std::vector<std::string>& get_best_result();
	float get_best_value_min();
	float get_best_value_max();
	void set_best_result();
	void start_optimizing();
	void print_to_file(const std::string &filename);
};