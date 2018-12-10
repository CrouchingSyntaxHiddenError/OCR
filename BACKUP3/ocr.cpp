#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <endian.h>
#include "perceptron.h"
#include "neuralnet.h"

using namespace std;

const double LEARNING_RATE = 0.5;
const int NUM_HIDDEN = 35;
const int NUM_OUT = 10;

void openInputStream(ifstream& input, string path)
{
	input.open(path, ios::binary);
	if (!input) {
		cerr << "ERROR: Could not open file: " << path << endl;
		exit(-1);
	}
}

void parseImageHeaders(ifstream& input, unsigned int& magicNum, unsigned int& imageNum, unsigned int& imageRows, unsigned int& imageCols)
{
	//Read image magic number
	input.read((char*)&magicNum, sizeof(magicNum));
	magicNum = be32toh(magicNum);
	//printf("image magic: %x\n", magicNum);

	//Read number of images
	input.read((char*)&imageNum, sizeof(imageNum));
	imageNum = be32toh(imageNum);
	//printf("num images: %x\n", imageNum);
	
	input.read((char*)&imageRows, sizeof(imageRows));
	input.read((char*)&imageCols, sizeof(imageCols));
	imageRows = be32toh(imageRows);
	imageCols = be32toh(imageCols);
	//printf("image imageRows: %x\n", imageRows);
	//printf("image imageCols: %x\n", imageCols);
}

void parseLabelHeaders(ifstream& input, unsigned int& magicNum, unsigned int& labelNum)
{
	//Read label magic number
	input.read((char*)&magicNum, sizeof(magicNum));
	magicNum = be32toh(magicNum);
	//printf("label magic: %x\n", magicNum);

	//Read number of labels
	input.read((char*)&labelNum, sizeof(labelNum));
	labelNum = be32toh(labelNum);
	//printf("num labels: %x\n", labelNum);
}

int main(int argc, char* argv[], char* envp[])
{
	if (argc != 6) {
		cerr << "ERROR: Incorrect number of command line arguments!" << endl;
		cerr << "Syntax: ./Checkpoint <epochs> <train_image_file> <train_label_file> <test_image_file> <test_label_file>" << endl;
	}

	unsigned int epochs = atoi(argv[1]);
	string imageDataPath = argv[2];
	string labelDataPath = argv[3];

	if (epochs < 0) {
		cerr << "ERROR: Invalid number of epochs! Must be non-negative value." << endl;
		exit(-1);
	}

	ifstream imageData;
	openInputStream(imageData, imageDataPath);

	ifstream labelData;
	openInputStream(labelData, labelDataPath);

	ofstream output;
	output.open("data.out", ofstream::app);
	if (!output) {
		cerr << "ERROR: Could not create output file img.pgm!" << endl;
		exit(-1);
	}

	unsigned int imageMagicNumber = 0;
	unsigned int labelMagicNumber = 0;
	unsigned int numImages = 0;
	unsigned int numLabels = 0;
	unsigned int rowsImages = 0;
	unsigned int colsImages = 0;

	// * * * * * * * * * * * * * * * * * * * * *
	//           Generate information
	// * * * * * * * * * * * * * * * * * * * * *

	parseImageHeaders(imageData, imageMagicNumber, numImages, rowsImages, colsImages);
	parseLabelHeaders(labelData, labelMagicNumber, numLabels);
	//unsigned int imageSize = rowsImages * colsImages;

	// * * * * * * * * * * * * * * * * * * * * *
	//      Store all image and label data
	// * * * * * * * * * * * * * * * * * * * * *

	vector <vector <double>> imageValues;
	vector <vector <double>> labelVectors;
	vector <double> labelValues;

	cout << "Storing data..." << endl;
	for (unsigned int i = 0; i < numImages; ++i) {
		vector <double> image;
		vector <double> label;
		for (unsigned int r = 0; r < rowsImages; ++r) {
			for (unsigned int c = 0; c < colsImages; ++c) {
				unsigned char temp = 0;
				imageData.read((char*)&temp, sizeof(temp));
				image.push_back((double)temp/255.0);
				//image.push_back((double)temp);
			}
		}
		image.push_back(-1);
		unsigned char temp = 0;
		labelData.read((char*)&temp, sizeof(temp));
		double labelVal = temp;
		//cout << labelVal << endl;

		for (int i = 0; i < 10; ++i) {
			label.push_back(0);
		}
		label[labelVal] = 1;
		//for (int i = 0; i < 10; ++i) {
		//	cout << label[i] << " ";
		//}
		//cout << endl;

		imageValues.push_back(image);
		labelValues.push_back((double)temp);
		labelVectors.push_back(label);
	}
	cout << "Data stored!" << endl;
	imageData.close();
	labelData.close();

	// * * * * * * * * * * * * * * * * * * * * *
	//      Build neural network
	// * * * * * * * * * * * * * * * * * * * * *

	srandom(time(NULL));
	network neuralNet(imageValues[0].size(), NUM_HIDDEN, NUM_OUT);
	cout << "Initializing network..." << endl;
	neuralNet.initNetwork(imageValues[0].size(), rand());
	//neuralNet.initNetwork(imageValues[0].size(), 1);

	for (unsigned int e = 0; e < epochs; ++e) {
		cout << "Epcoh: " << e << "..." << endl;
		for (unsigned int i = 0; i < numImages; ++i) {
			if (i % 10000 == 0) {
				cout << "Epoch " << e << ": Trained network on " << i << " images..." << endl;
			}
			//cout << "	Training on image " << i << " (" << labelValues[i] << ")..." << endl;
			neuralNet.learn(imageValues[i], labelVectors[i], LEARNING_RATE);
		}
	}

	imageDataPath = argv[4];
	labelDataPath = argv[5];

	openInputStream(imageData, imageDataPath);
	openInputStream(labelData, labelDataPath);

	parseImageHeaders(imageData, imageMagicNumber, numImages, rowsImages, colsImages);
	parseLabelHeaders(labelData, labelMagicNumber, numLabels);

	imageValues.clear();
	labelValues.clear();
	labelVectors.clear();

	cout << "Storing data..." << endl;
	for (unsigned int i = 0; i < numImages; ++i) {
		vector <double> image;
		vector <double> label;
		for (unsigned int r = 0; r < rowsImages; ++r) {
			for (unsigned int c = 0; c < colsImages; ++c) {
				unsigned char temp = 0;
				imageData.read((char*)&temp, sizeof(temp));
				image.push_back((double)temp/255.0);
				//image.push_back((double)temp);
			}
		}
		image.push_back(-1);
		unsigned char temp = 0;
		labelData.read((char*)&temp, sizeof(temp));
		double labelVal = temp;

		for (int i = 0; i < 10; ++i) {
			label.push_back(0);
		}
		label[labelVal] = 1;

		//if (label == 0)
		//	printf("Found image: %d, storing label %f\n", i, (double)label);
		labelValues.push_back(temp);
		imageValues.push_back(image);
		labelVectors.push_back(label);
	}
	cout << "Data stored!" << endl;
	imageData.close();
	labelData.close();

	double correct = 0;
	double incorrect = 0;
	for (unsigned int i = 0; i < numImages; ++i) {
		double result = neuralNet.evaluate(imageValues[i]);
		cout << "IMAGE :" << i << " LABEL: " << labelValues[i] << endl;
		cout << " --- ";
		cout << result;
		if (result != labelValues[i]) {
			cout << " INCORRECT! " << endl;
			++incorrect;
		}
		else {
			cout << " CORRECT! " << endl;
			++correct;
		}
	}

	cout << "FINAL RESULT: " << endl << "	CORRECT:	" << (correct/(double)numImages)*100 << "%" << endl << "	INCORRECT:	" << (incorrect/(double)numImages)*100 << "%" << endl;


	// * * * * * * * * * * * * * * * * * * * * *
	//      Generate output file
	// * * * * * * * * * * * * * * * * * * * * *
	//Add label to output
	output << "Results from running OCR:" << endl;
	output << "	Number of hidden nodes:" << NUM_HIDDEN << endl; 
	output << "	Number of epochs: " << epochs << endl;
	output << "	Learning rate: " << LEARNING_RATE << endl;
	output << "	Results:" << endl;
	output << "		Number correct:		" << correct << "/" << numImages << " (" << correct/(double)numImages*100 << "%)" << endl;
	output << "		Number incorrect:	" << incorrect << "/" << numImages << " (" << incorrect/(double)numImages*100 << "%)" << endl;
	output << "	 ------ " << endl;
}
