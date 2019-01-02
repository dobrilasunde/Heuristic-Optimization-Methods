#include "Data.h"
#include "Optimizer.h"

Optimizer::Optimizer(Data *data) : _data(data) {
}

Optimizer::Optimizer(Data *data, std::vector<double>& weight_factors) : _data(data), 
	_weight_factors(weight_factors) {
}

void Optimizer::set_weight_factors(std::vector<double>& weight_factors) {
	this->_weight_factors = weight_factors;
}

void Optimizer::set_weight_factor_by_index(double factor, int index) {
	this->_weight_factors[index] = factor;
}

void Optimizer::add_weight_factor(double factor) {
	this->_weight_factors.push_back(factor);
}

void Optimizer::set_goal_values(std::vector<double>& goal_values) {
	this->_goal_values = goal_values;
}

void Optimizer::set_goal_value_by_index(double value, int index) {
	this->_goal_values[index] = value;
}

void Optimizer::add_goal_value(double value) {
	this->_goal_values.push_back(value);
}

std::vector<double>& Optimizer::get_weight_factors() {
	return this->_weight_factors;
}

double Optimizer::get_weight_factor_by_index(int index) {
	return this->_weight_factors[index];
}

std::vector<double>& Optimizer::get_goal_values() {
	return this->_goal_values;
}

double Optimizer::get_goal_value_by_index(int index) {
	return this->_goal_values[index];
}
