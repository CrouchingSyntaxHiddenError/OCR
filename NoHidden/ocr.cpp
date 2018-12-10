#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <endian.h>
#include "perceptron.h"

using namespace std;

const double LEARNING_RATE = 0.05;

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
	output.open("img.pgm");
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
	unsigned int imageSize = rowsImages * colsImages;

	// * * * * * * * * * * * * * * * * * * * * *
	//      Store all image and label data
	// * * * * * * * * * * * * * * * * * * * * *

	vector <vector <double>> imageValues;
	vector <double> labelValues;

	cout << "Storing data..." << endl;
	for (unsigned int i = 0; i < numImages; ++i) {
		vector <double> image;
		for (unsigned int r = 0; r < rowsImages; ++r) {
			for (unsigned int c = 0; c < colsImages; ++c) {
				unsigned char temp = 0;
				imageData.read((char*)&temp, sizeof(temp));
				//image.push_back((double)temp/255.0);
				image.push_back((double)temp);
			}
		}
		image.push_back(-1);
		unsigned char label = 0;
		labelData.read((char*)&label, sizeof(label));

		//if (label == 0)
		//	printf("Found image: %d, storing label %f\n", i, (double)label);
		labelValues.push_back((double)label);
		imageValues.push_back(image);
	}
	cout << "Data stored!" << endl;
	imageData.close();
	labelData.close();

	// * * * * * * * * * * * * * * * * * * * * *
	//      Build 10 perceptrons
	// * * * * * * * * * * * * * * * * * * * * *

	vector <perceptron> layer;
	for (int i = 0; i < 10; ++i) {
		perceptron a(LEARNING_RATE, i, imageValues[0].size());
		layer.push_back(a);
	}
	//perceptron zero(LEARNING_RATE, 3, imageValues[0].size());
	for (int e = 0; e < epochs; ++e) {
		for (int i = 0; i < numImages; ++i) {
			cout << "IMAGE: " << i << ", LABEL: " << labelValues[i] << endl;
			layer[labelValues[i]].train(imageValues[i], labelValues[i]);
			//zero.train(imageValues[i], labelValues[i]);
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

	cout << "Storing data..." << endl;
	for (unsigned int i = 0; i < numImages; ++i) {
		vector <double> image;
		for (unsigned int r = 0; r < rowsImages; ++r) {
			for (unsigned int c = 0; c < colsImages; ++c) {
				unsigned char temp = 0;
				imageData.read((char*)&temp, sizeof(temp));
				//image.push_back((double)temp/255.0);
				image.push_back((double)temp);
			}
		}
		image.push_back(-1);
		unsigned char label = 0;
		labelData.read((char*)&label, sizeof(label));

		//if (label == 0)
		//	printf("Found image: %d, storing label %f\n", i, (double)label);
		labelValues.push_back((double)label);
		imageValues.push_back(image);
	}
	cout << "Data stored!" << endl;
	imageData.close();
	labelData.close();

	double correct = 0;
	double incorrect = 0;
	for (int i = 0; i < numImages; ++i) {
		vector <double> results;
		cout << "IMAGE :" << i << " LABEL: " << labelValues[i] << endl;
		for (int l = 0; l < layer.size(); ++l) {
			double result = layer[l].evaluate(imageValues[i], labelValues[i]);
			//cout << imageValues[i].size() << " " << layer[l].weights().size() << endl;
			//cout << "RESULT OF IMAGE: " << i << " WITH PERCPTRON: " << l << " --- " << result << endl;
			results.push_back(result);
		}
		//bool result = zero.evaluate(imageValues[i], labelValues[i]);
		cout << " --- ";
		double max = results[0];
		int index = 0;
		for (int l = 0; l < results.size(); ++l) {
			if (results[l] > max) {
				max = results[l];
				index = l;
			}
		}
		cout << index;
		if (index != labelValues[i]) {
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
	//      Generate PGM file
	// * * * * * * * * * * * * * * * * * * * * *
/*
	//Add label to output
	output << "P2" << endl;
	output << "#Weight vector for zero perceptron" << endl; 
	output << rowsImages << " " << colsImages << endl;
	output << 255 << endl;

	//Grab the data for the desired image
	vector <double> weight = zero.weights();
	double min = weight[0];
	double max = weight[0];
	for (unsigned int r = 0; r < imageSize; ++r) {
		if (r % 28 == 0)
			cout << endl;
		cout << setw(10) << left << weight[r] << " ";
		if (weight[r] < min)
			min = weight[r];
		if (weight[r] > max)
			max = weight[r];
	}

	for (unsigned int r = 0; r < weight.size(); ++r) {
		if (r % 28 == 0)
			output << endl;
		double temp = weight[r];
		//temp += min;
		//temp *= 128/(max + min);
		//temp += 128;
		//if (temp < 0)
		//	temp = 0;
		//if (temp > 255) 
		//	temp = 255;
		//printf("%x ", temp);
		output << setw(5) << left << floor(s(temp)*255) << " ";
	}
*/	
}
