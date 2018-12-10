#include <iostream>
#include <iomanip>
#include <fstream>
#include <endian.h>

using namespace std;

int main(int argc, char* argv[], char* envp[])
{
	if (argc != 4) {
		cerr << "ERROR: Incorrect number of command line arguments!" << endl;
		cerr << "Syntax: ./Checkpoint <index> <image_file> <label_file>" << endl;
	}

	unsigned int index = atoi(argv[1]);
	string imageDataPath = argv[2];
	string labelDataPath = argv[3];

	if (index < 0) {
		cerr << "ERROR: Invalid index! Must be non-negative value." << endl;
		exit(-1);
	}

	ifstream imageData;
	imageData.open(imageDataPath, ios::binary);
	if (!imageData) {
		cerr << "ERROR: Could not open file: " << imageDataPath << endl;
		exit(-1);
	}

	ifstream labelData;
	labelData.open(labelDataPath, ios::binary);
	if (!labelData) {
		cerr << "ERROR: Could not open file: " << labelDataPath << endl;
		exit(-1);
	}

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
	//      Generate image information
	// * * * * * * * * * * * * * * * * * * * * *

	//Read image magic number
	imageData.read((char*)&imageMagicNumber, sizeof(imageMagicNumber));
	imageMagicNumber = be32toh(imageMagicNumber);
	//printf("image magic: %x\n", imageMagicNumber);

	//Read number of images
	imageData.read((char*)&numImages, sizeof(numImages));
	numImages = be32toh(numImages);
	//printf("num images: %x\n", numImages);
	
	if (index >= numImages) {
		cerr << "ERROR: Index excedes total number of images!" << endl;
		exit(-1);
	}

	imageData.read((char*)&rowsImages, sizeof(rowsImages));
	imageData.read((char*)&colsImages, sizeof(colsImages));
	rowsImages = be32toh(rowsImages);
	colsImages = be32toh(colsImages);
	//printf("image rowsImages: %x\n", rowsImages);
	//printf("image colsImages: %x\n", colsImages);
	
	// * * * * * * * * * * * * * * * * * * * * *
	//      Generate label information
	// * * * * * * * * * * * * * * * * * * * * *

	//Read label magic number
	labelData.read((char*)&labelMagicNumber, sizeof(labelMagicNumber));
	labelMagicNumber = be32toh(labelMagicNumber);
	//printf("label magic: %x\n", labelMagicNumber);

	//Read number of labels
	labelData.read((char*)&numLabels, sizeof(numLabels));
	numLabels = be32toh(numLabels);
	//printf("num labels: %x\n", numLabels);

	// * * * * * * * * * * * * * * * * * * * * *
	//      Find image given by index
	// * * * * * * * * * * * * * * * * * * * * *

	//Seek ahead to the image given by index
	for (unsigned int i = 0; i < rowsImages * colsImages * index; ++i) {
		imageData.ignore();
	}

	//Seek ahead to the label given by index
	for (unsigned int i = 0; i < index; ++i) {
		labelData.ignore();
	}

	unsigned char label = 0;
	labelData.read((char*)&label, sizeof(label));
	//printf("Label: %d\n", label);

	// * * * * * * * * * * * * * * * * * * * * *
	//      Generate PGM file
	// * * * * * * * * * * * * * * * * * * * * *

	//Add label to output
	output << "P2" << endl;
	output << "#Source file: " << imageDataPath << ", Index: " << index << ", Label: " << (int)label << endl;
	output << rowsImages << " " << colsImages << endl;
	output << 255 << endl;

	//Grab the data for the desired image
	for (unsigned int r = 0; r < rowsImages; ++r) {
		for (unsigned int c = 0; c < colsImages; ++c) {
			unsigned char temp = 0;
			imageData.read((char*)&temp, sizeof(temp));
			//printf("%x ", temp);
			output << (int)temp << " ";
		}
		//printf("\n");
	}
}
