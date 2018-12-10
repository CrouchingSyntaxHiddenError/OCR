#ifndef NEURAL_H
#define NEURAL_H
#include <vector>
#include <cmath>
#include "perceptron.h"

class network
{
	private:
		vector <double> m_input;
		vector <perceptron> m_hiddenLayer;
		vector <perceptron> m_outputLayer;
		int m_hiddenSize;
		int m_outputSize;
		//vector <vector<double>> m_IHWeights;
		//vector <vector<double>> m_HOWeights;

	public:
		network(int in, int hd, int out);
		network(int in, int out);

		vector <double> input() const;
		vector <perceptron> hiddenLayer() const;
		vector <perceptron> outputLayer() const;
		int hiddenSize() const;
		int outputSize() const;
		//vector <vector <double>> IHWeights() const;
		//vector <vector <double>> HOWeights() const;

		void input(const vector<double>& input);
		void hiddenLayer(const vector<perceptron>& hiddenlayer);
		void outputLayer(const vector<perceptron>& outputLayer);
		//void IHWeights(const vector<vector<double>>& weights);
		//void HOWeights(const vector<vector<double>>& weights);


};

#endif
