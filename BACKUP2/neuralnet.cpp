#include <iostream>
#include <iomanip>
#include "neuralnet.h"

network :: network (int in, int hd, int out)
{
	m_hiddenSize = hd;
	m_outputSize = out;

	for (int i = 0; i < hd; ++i) {
		perceptron a;
		m_hiddenLayer.push_back(a);
	}

	for (int i = 0; i < out; ++i) {
		perceptron a;
		m_outputLayer.push_back(a);
	}
}
network :: network (int in, int out)
{
	m_outputSize = out;
	m_hiddenSize = 0;

	for (int i = 0; i < out; ++i) {
		perceptron a;
		m_outputLayer.push_back(a);
	}
}

vector <perceptron> network :: hiddenLayer() const {if (m_hiddenSize != 0) return m_hiddenLayer;}
vector <perceptron> network :: outputLayer() const {return m_outputLayer;}
int network :: hiddenSize() const {return m_hiddenSize;}
int network :: outputSize() const {return m_outputSize;}

void network :: initNetwork(const int& inputSize, const int& seed)
{
	for (int i = 0; i < m_hiddenSize; ++i) {
		//cout << "Randomizing hidden node " << i << endl;
		srandom(time(NULL) / seed);
		m_hiddenLayer[i].generateWeights(inputSize, rand());
		//m_hiddenLayer[i].generateWeights(inputSize, 1);
	}

	for (int i = 0; i < m_outputSize; ++i) {
		srandom(time(NULL) * i);
		m_outputLayer[i].generateWeights(m_hiddenSize, rand());
		//m_outputLayer[i].generateWeights(m_hiddenSize, 1);
	}
}

void network :: learn(const vector<double>& input, const vector<double>& label, const double& learningRate)
{
	vector <double> hiddenValues;
	vector <double> hiddenSigmoidValues;
	// Propogate inputs forward
	for (int h = 0; h < m_hiddenSize; ++h) {
		double w = m_hiddenLayer[h].getResult(input);
		//cout << "		Weight from hidden layer node: " << h << ": " << w << " (" << s(w) << ")" << endl;
		hiddenValues.push_back(w);
		hiddenSigmoidValues.push_back(s(w));
		//cout << "Hidden output for node " << h << ": " << w << " (" << s(w) << ")" << endl;
	}
	//cout << "		 ----- " << endl;

	vector <double> outputValues;
	vector <double> outputSigmoidValues;
	for (int o = 0; o < m_outputSize; ++o) {
		double w = m_outputLayer[o].getResult(hiddenSigmoidValues);
		//cout << "		Weight from output layer node: " << o << ": " << w << " (" << s(w) << ")" << endl;
		outputValues.push_back(w);
		outputSigmoidValues.push_back(s(w));
		//cout << "Output output for node " << o << ": " << w << " (" << s(w) << ")" << endl;
	}
	//cout << "		 ----- " << endl;

	//Get output errors (deltas)
	vector <double> outputDeltas;
	for (int o = 0; o < m_outputSize; ++o) {
		double difference = label[o] - outputSigmoidValues[o];
		double delta = ds(outputValues[o]) * difference;
		//cout << "		Difference from output: " << label[o] << " - " << outputSigmoidValues[o] << " = " << difference << endl;
		//cout << "		" << setw(24) << left << "Delta: " << ds(outputValues[o]) << " * " << difference << " = " << delta << endl;
		outputDeltas.push_back(delta);
	}
	//cout << "		 ----- " << endl;

	// Propogate deltas backward
	vector <double> propDeltas;
	for (int h = 0; h < m_hiddenSize; ++h) {
		double delta = 0;
		for (int i = 0; i < m_outputSize; ++i) {
			vector <double> outputWeights = m_outputLayer[i].weights();
			double deltaVal = outputWeights[h] * outputDeltas[i];
			//cout << "		Propagated delta for node " << h << " from output " << i << ": " << outputWeights[h] << " * " << outputDeltas[i] << " = " << deltaVal << endl;
			delta += deltaVal;
		}
		//cout << "		Total propagated change: " << delta << endl;
		delta *= ds(hiddenValues[h]);
		//cout << "		Change after gradient descent: " << delta << endl;
		//cout << "Propagated deltas for hidden node: " << h << ": " << delta << endl;
		propDeltas.push_back(delta);
		//cout << "		 ----- " << endl;
	}

	//Change weights in output
	for (int o = 0; o < m_outputSize; ++o) {
		bool changed = false;
		//cout << "		Updating weights of output node " << o << endl;
		vector <double> weight = m_outputLayer[o].weights();
		for (int w = 0; w < weight.size(); ++w) {
			double change = (learningRate * hiddenSigmoidValues[w] * outputDeltas[o]);
			weight[w] += change;
		}

		m_outputLayer[o].weights(weight);
	}

	//Change weights in hidden layer
	for (int h = 0; h < m_hiddenSize; ++h) {
		bool changed = false;
		//cout << "		Updating weights of hidden node " << h << endl;
		vector <double> weight = m_hiddenLayer[h].weights();
		for (int w = 0; w < weight.size(); ++w) {
			double change = (learningRate * s(input[w]) * propDeltas[h]);
			//cout << "			Changing weight " << weight[w] << " by: " << learningRate << " * " << hiddenSigmoidValues[h] << " * " << propDeltas[h] << " = " << change << endl;
			weight[w] += change;
			//cout << "			New weight: " << weight[w] << endl;
		}

		m_hiddenLayer[h].weights(weight);
		vector <double> Newweight = m_hiddenLayer[h].weights();

		for (int w = 0; w < weight.size(); ++w) {
			if (weight[w] != Newweight[w]) {
				changed = true;
			}
		}
			/*
		if (changed)
			cout << "Weights changed!" << endl;
		else
			cout << "Weights not changed??" << endl;
			*/
	}
}

double network :: evaluate(const vector<double>& input)
{
	vector <double> hiddenResults;
	for (int i = 0; i < m_hiddenSize; ++i) {
		hiddenResults.push_back(s(m_hiddenLayer[i].getResult(input)));
	}

	vector <double> outputResults;
	for (int i = 0; i < m_outputSize; ++i) {
		outputResults.push_back(s(m_outputLayer[i].getResult(hiddenResults)));
	}

	int index = 0;
	//for (int i = 0; i < outputResults.size(); ++i) {
	for (int i = 0; i < m_outputSize; ++i) {
		//cout << "	Weight for: " << i << ": " << outputResults[i] << endl;
		if (outputResults[i] > outputResults[index])
			index = i;
	}

	return index;
}
