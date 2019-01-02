#include "Data.h"
#include "Optimizer.h"

Optimizer::Optimizer(Data *data) : _data(data) {
}

Optimizer::Optimizer(Data *data, std::vector<float>& weight_factors) : _data(data), 
	_weight_factors(weight_factors) {
}

void Optimizer::set_weight_factors(std::vector<float>& weight_factors) {
	this->_weight_factors = weight_factors;
}

void Optimizer::set_weight_factor_by_index(float factor, int index) {
	this->_weight_factors[index] = factor;
}

void Optimizer::add_weight_factor(float factor) {
	this->_weight_factors.push_back(factor);
}

void Optimizer::set_goal_values(std::vector<float>& goal_values) {
	this->_goal_values = goal_values;
}

void Optimizer::set_goal_value_by_index(float value, int index) {
	this->_goal_values[index] = value;
}

void Optimizer::add_goal_value(float value) {
	this->_goal_values.push_back(value);
}

std::vector<float>& Optimizer::get_weight_factors() {
	return this->_weight_factors;
}

float Optimizer::get_weight_factor_by_index(int index) {
	return this->_weight_factors[index];
}

std::vector<float>& Optimizer::get_goal_values() {
	return this->_goal_values;
}

float Optimizer::get_goal_value_by_index(int index) {
	return this->_goal_values[index];
}

float Optimizer::calculate_global_goal() {
	
	float res = 0.0f;

	for (int i = 0; i < this->_goal_values.size(); i++) {
		res += this->_goal_values[i] * (1.0f / this->_weight_factors[i]);
	}

	return res;
}
