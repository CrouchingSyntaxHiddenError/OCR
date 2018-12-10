#include "perceptron.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
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
	//cout << v1.size() << " " << v2.size() << endl;
	if (v1.size() != v2.size())
		return -1;
	double val = 0;
	for (unsigned int i = 0; i < v1.size(); ++i) {
		val += (v1[i] * v2[i]);
	}
	return val;
}

//perceptron :: perceptron(const double& learn, const double& label, const int& weightSize)
perceptron :: perceptron()
{
	m_output = 0;
	m_weights.clear();
}

double perceptron :: output() const {return m_output;}
vector <double> perceptron :: weights() const {return m_weights;}
void perceptron :: output(const double& output) {m_output = output;}
void perceptron :: weights(const vector <double>& weights) 
{
	m_weights.clear();
	for (unsigned int i = 0; i < weights.size(); ++i)
		m_weights.push_back(weights[i]);
	//vector <double> newWeights(weights);
	//m_weights = newWeights;
	//cout << "Weights updated!" << endl;
}
void perceptron :: generateWeights(const int& size, const int& seed)
{
	m_weights.clear();
	for (int i = 0; i < size; ++i) {
		srandom(time(NULL) * seed + i);
		double sign = rand() % 2;
		if (sign) 
			m_weights.push_back(-(rand()/(double)RAND_MAX));
		else
			m_weights.push_back(rand()/(double)RAND_MAX);
		//m_weights.push_back(0.5);
	}
	//usleep(1000);
}

/*
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

		double error = label - output;
		for (int i = 0; i < m_weights.size(); ++i) {
			m_weights[i] += m_learningRate * error * image[i];
		}
	m_threshold = m_weights[m_weights.size()-1];
	cout << endl << endl;
	for (int i = 0; i < m_weights.size(); ++i) {
		cout << m_weights[i] << " ";
	}
}
*/

double perceptron :: getResult(const vector <double>& image) 
{
	/*
	cout << " -------- " << endl;
	for (int i = 0; i < m_weights.size(); ++i) {
		cout << setw(10) << left << "	" << m_weights[i] << " " << image[i] << endl;
	}
	cout << " -------- " << endl;
	*/
	//cout << "	" << image.size() << " " << m_weights.size() << endl;
	return dot(image, m_weights);
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
