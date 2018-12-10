#include "perceptron.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

double s(const double& x)
{
	return (1.0/(1.0+exp(-x)));
}

double ds(const double& x)
{
	return (s(x)*(1-s(x)));
}

double dot(const vector<double>& v1, const vector<double>& v2)
{
	if (v1.size() != v2.size())
		return -1;
	double val = 0;
	for (int i = 0; i < v1.size(); ++i) {
		val += (v1[i] * v2[i]);
	}
	return val;
}

perceptron :: perceptron(const double& learn, const double& label, const int& weightSize)
{
	m_output = 0;
	m_learningRate = learn;
	m_label = label;
	for (int i = 0; i < weightSize; ++i) {
		srandom(time(NULL) * i);
		double sign = rand() % 2;
		if (sign) 
			m_weights.push_back(-(rand()/(double)RAND_MAX));
		else
			m_weights.push_back(rand()/(double)RAND_MAX);
	}
	m_threshold = m_weights[m_weights.size()];
}

double perceptron :: output() const {return m_output;}
double perceptron :: learningRate() const {return m_learningRate;}
double perceptron :: label() const {return m_label;}
vector <double> perceptron :: weights() const {return m_weights;}
void perceptron :: output(const double& output) {m_output = output;}
void perceptron :: learningRate(const double& learningRate) {m_learningRate = learningRate;}
void perceptron :: label(const double& label) {m_label = label;}
void perceptron :: weights(const vector <double>& weights) {m_weights = weights;}

void perceptron :: train(const vector <double>& image, const double& label)
{
	//double output = s(getResult(image));
	//double correct = s(label);
	double output = getResult(image);

	cout << image.size() << " " << m_weights.size() << endl;
	//cout << "OUTPUT: " << output << " v " << correct << " CORRECT" << endl;
	cout << "DOT: " << getResult(image) << endl;
	cout << "OUTPUT: " << output << endl;
	//cout << "THRESHOLD: " << m_weights[m_weights.size()-1] << endl;
	//if (output >= correct) {
	//if (output >= m_threshold && label == m_label) {
	//if (output >= m_weights[m_weights.size()-1] && label == m_label) {
	if (output > 0 && m_label == label) {
		cout << "	CORRECT!" << endl;
	}
	else if (output >= 0 && m_label != label) {
		cout << "	INCORRECT! Fixing..." << endl;
		for (int i = 0; i < m_weights.size(); ++i) 
			m_weights[i] -= m_learningRate * image[i];
		//m_threshold = m_weights[m_weights.size()];
	}
		
	else if (output < 0 && m_label == label) {
		cout << "	INCORRECT! Fixing..." << endl;
		for (int i = 0; i < m_weights.size(); ++i) 
			m_weights[i] += m_learningRate * image[i];
		//m_threshold = m_weights[m_weights.size()];
	}

		/*
		double error = label - output;
		for (int i = 0; i < m_weights.size(); ++i) {
			m_weights[i] += m_learningRate * error * image[i];
		}
		*/
	/*
	m_threshold = m_weights[m_weights.size()-1];
	cout << endl << endl;
	for (int i = 0; i < m_weights.size(); ++i) {
		cout << m_weights[i] << " ";
	}
	*/
}
double perceptron :: getResult(const vector <double>& image) {return dot(image, m_weights);}
void perceptron :: learn(const vector <double>& image, const double& label)
{
	double result = s(getResult(image));
}
double perceptron :: evaluate(const vector <double>& image, const double& label)
{
	/*
	double output = s(getResult(image));
	m_output = output;
	return output;
	double correct = s(label);
	*/
	double output = getResult(image);
	return output;
	/*
	if (output > 0)
		return true;
	else
		return false;
	*/
}
