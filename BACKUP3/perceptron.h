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
		vector <double> m_weights;
		double m_output;

	public:
		//perceptron(const double& learn, const double& thresh, const int& weightSize);
		perceptron();

		double output() const;
		vector <double> weights() const;

		void output(const double& output);
		void weights(const vector <double>& weights);
		void generateWeights(const int& size, const int& seed);

		//void train(const vector <double>& image, const double& label);
		double getResult(const vector <double>& image);
		double evaluate(const vector <double>& image, const double& label);
};

#endif
