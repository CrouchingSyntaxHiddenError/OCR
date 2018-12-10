#ifndef PERCEPTRON_H
#define PERCEPTRON_H
#include <vector>
#include <cmath>

using namespace std;

double s(const double& x);
double ds(const double& x);

class perceptron
{
	private:
		double m_output;
		double m_learningRate;
		double m_label;
		double m_threshold;
		vector <double> m_weights;

	public:
		perceptron(const double& learn, const double& thresh, const int& weightSize);

		double output() const;
		double learningRate() const;
		double label() const;
		vector <double> weights() const;

		void output(const double& output);
		void learningRate(const double& learningRate);
		void label(const double& label);
		void weights(const vector <double>& weights);

		void train(const vector <double>& image, const double& label);
		double getResult(const vector <double>& image);
		void learn(const vector <double>& image, const double& label);
		double evaluate(const vector <double>& image, const double& label);
};

#endif
