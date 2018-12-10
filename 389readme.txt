John Doe
CMSC 389: Artificial Intelligence
Project 3: Optical Character Recognition

 ---- Introduction ----

	To compile the final program, type:

		compile ocr.cpp perceptron.cpp neuralnet.cpp -o OCR

	To run the checkpoint program, type:

		./OCR <num of epochs> <trainingImageData> <trainingLabelData> <testingImageData> <testingLabelData>

	Where:
		<num of epochs> is the number of epochs you wish to train with
		<trainingImageData> is the file containing the training image information
		<trainingLabelData> is the file containing the training label information
		<testingImageData> is the file containing the testing image information
		<testingLabelData> is the file containing the testing label information

 ---- Information ----

		The final version of this project runs a full multilayer neural network that classifies handwritten digits from the MNIST database with varying degrees of correctness (depending on the parameters of the program). 

		In the current version of the program, the only parameter that the user can change directly from the command line is the number of epochs. The other parameters (such as the learning rate and the number of hidden nodes) can be changed from ocr.cpp by modifying the constant global variables at the top of the file.

		The version of the program in this folder does not work if there is no hidden layer. Instead, there is a subfolder called "NoHidden" which includes a version that works with no hidden layer.

		Output data is always written to the file "data.out", which records both the parameters of different instances of the OCR program, as well as the resulting degrees of correctness. With this information, one can interpret the results of different parameterizations.

 ---- Proof of Correctness ----

		The necessary data structures for this project are separated between "perceptron.h" and "neuralnet.h" (each of which has their own implementation file). Each perceptron (perceptron.h) has a vector of doubles representing the weights of the previous layer. Each network (neuralnet.h) has two vectors of perceptrons, one for a hidden layer and one for the output layer. Right now, the program only supports one hidden layer. 

		The network class has a method called train, which trains the network on a given image, label, and learning rate. The backpropagation algorithm from the book is implemented in this method, which computes the changes in weight using gradient descent. Based on the numerous test cases (shown in data.out), the trend follows the data from the book (see comparisons below).

 ---- Comparisons ----

		The most significant parameter in our program is the number of nodes in the hidden layer. If there are no nodes (and thus no hidden layer), then the results for the program are generally very poor (~10%).

		A parameter with a great weight in terms of change is the number of epochs the program is run. As the program trains on a set of 60,000 images, then increasing the number of epochs will result in each node's weight being adjusted 60,000 times more (at least). Thus, we will, on average, see a higher degree of correctness between two configurations with identical parameters save for the number of epochs.

		One might naively assume that a higher learning rate will correspond with a higher degree of accuracy. However, in out tests, we see that progressively higher learning rates (0.2 v 1.3 v 2.3) result in progressively lower degrees of accuracy (84% v 82% v 46%). This supports the data in our textbook, where this trend is shown.
