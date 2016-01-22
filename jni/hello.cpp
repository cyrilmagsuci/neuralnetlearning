

// Copyright, 2015, by Steven C. Shaffer 
// You may use this code as a basis for your own learning or as the starting 
// place for your own projects (academic or commercial). Do not reproduce 
// or publish in whole or in part without the express permission of the 
// author.


#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <cmath>

const int NUMINPUTNODES = 2; 
const int NUMHIDDENNODES = 2; 
const int NUMOUTPUTNODES = 1; 
const int NUMNODES = NUMINPUTNODES + NUMHIDDENNODES + NUMOUTPUTNODES; 
const int ARRAYSIZE = NUMNODES + 1; // 1-offset to match “node 1”“node 2” etc.
const int MAXITERATIONS = 131072; 
const double E = 2.71828; 
const double LEARNINGRATE = 0.2; 
	
// Function prototypes 
void initialize( double [][ARRAYSIZE], double[], double[], double[]); 
void connectNodes( double[][ ARRAYSIZE], double[]); 
void activateNetwork(double [][ARRAYSIZE], double[], double[]);
void trainingExample( double[], double[]);
double updateWeights( double [][ARRAYSIZE], double[], double[], double[]);
void displayNetwork( double [], double);


int main() 
{ 

	//std::cout << "Hello World!" << std::endl;
	printf(" Neural Network Program\n"); 
	
	double weights[ ARRAYSIZE][ ARRAYSIZE]; 
	double values[ ARRAYSIZE]; 
	double expectedvalues[ ARRAYSIZE]; 
	double thresholds[ ARRAYSIZE]; 
	
	initialize( weights, values, expectedvalues, thresholds); 
	connectNodes( weights, thresholds); 
	
	int counter = 0; 
	
	while (counter < MAXITERATIONS) 
	{ 
		trainingExample( values, expectedvalues); 
		activateNetwork (weights, values, thresholds); 
		
		double sumOfSquaredErrors = updateWeights( weights, values, expectedvalues, thresholds); 
		displayNetwork( values, sumOfSquaredErrors); 
		counter++; 
	} 
	return 0; 
}



void trainingExample( double values[], double expectedValues[]) 
{ 
	static int counter = 0; 
	
	switch (counter % 4) 
	{ 
		case 0: 
			values[ 1] = 1; 
			values[ 2] = 1; 
			expectedValues[ 5] = 0; 
			break; 
		case 1: 
			values[ 1] = 0; 
			values[ 2] = 1; 
			expectedValues[ 5] = 1; 
			break; 
		case 2: 
			values[ 1] = 1; 
			values[ 2] = 0; 
			expectedValues[ 5] = 1; 
			break; 
		case 3: 
			values[ 1] = 0; 
			values[ 2] = 0; 
			expectedValues[ 5] = 0; 
			break; 
	} 
	
	counter++; 

}


void initialize(double weights[][ARRAYSIZE], double values[], double expectedValues[], double thresholds[]) 
{ 
	for (int x = 0; x <= NUMNODES; x ++) 
	{ 
		values[x] = 0.0; 
		expectedValues[x] = 0.0; 
		thresholds[x] = 0.0; 
		
		for (int y = 0; y <= NUMNODES; y ++) 
		{ 
			weights[x][y] = 0.0; 
		} 
	} 
}

void connectNodes(double weights[][ARRAYSIZE], double thresholds[]) 
{ 
	for (int x = 1; x <= NUMNODES; x++) 
	{ 
		for (int y = 1; y <= NUMNODES; y++) 
		{ 
			weights[x][y] = (rand() % 200) / 100.0; 
		} 
	} 
	
	thresholds[3] = rand() / (double) rand(); 
	thresholds[4] = rand() / (double) rand(); 
	thresholds[5] = rand() / (double) rand(); 
	printf("%f %f %f %f %f %f\n %f %f %f\n", 
		weights [1][3], weights[ 1][ 4], weights[ 2][ 3], weights [2][ 4], weights[ 3][ 5], weights[ 4][ 5], thresholds[ 3], thresholds[ 4], thresholds[ 5] ); 

}

void activateNetwork(double weights[][ARRAYSIZE], double values[], double thresholds[]) 
{ 
	
	// for each hidden node 
	for (int h = 1 + NUMINPUTNODES; h < 1 + NUMINPUTNODES + NUMHIDDENNODES; h++) 
	{ 
		double weightedInput = 0.0; 
		
		// add up the weighted input 
		for (int i = 1; i < 1 + NUMINPUTNODES; i++) 
		{ 
			weightedInput += weights[ i][ h] * values[ i]; 
		} 
		
		// handle the thresholds 
		weightedInput += (-1 * thresholds[ h]); 
		
		values[h] = 1.0 / (1.0 + pow( E, -weightedInput));
	
	} 
	
	
	// for each output node 
	for (int o = 1 + NUMINPUTNODES + NUMHIDDENNODES; o < 1 + NUMNODES; o++) 
	{ 
		double weightedInput = 0.0; 
		for ( int h = 1 + NUMINPUTNODES; h < 1 + NUMINPUTNODES + NUMHIDDENNODES; h++) 
		{ 
			weightedInput += weights[h][o] * values[ h]; 
		} 
		
		// handle the thresholds 
		weightedInput += (-1 * thresholds[o]); 
		values[o] = 1.0 / (1.0 + pow( E, -weightedInput));
	}
	
}



double updateWeights( double weights[][ ARRAYSIZE], double values[], double expectedvalues[], double thresholds[]) 
{ 
	double sumOfSquaredErrors = 0.0; 
	
	for (int o = 1 + NUMINPUTNODES + NUMHIDDENNODES; o < 1 + NUMNODES; o++) 
	{ 
		double absoluteerror = expectedvalues[ o] - values[ o]; 
		sumOfSquaredErrors += pow( absoluteerror, 2); 
		double outputErrorGradient = values[ o] * (1.0 - values[ o]) * absoluteerror; 
		
		// update each weighting from the hidden layer 
		for ( int h = 1 + NUMINPUTNODES; h < 1 + NUMINPUTNODES + NUMHIDDENNODES; h++) 
		{ 
			double delta = LEARNINGRATE * values[ h] * outputErrorGradient; 
			weights[ h][ o] += delta; 
			double hiddenErrorGradient = values[ h] * (1 - values[ h]) * outputErrorGradient * weights[ h][ o]; 
			
			for (int i = 1; i < 1 + NUMINPUTNODES; i++) 
			{ 
				double delta = LEARNINGRATE * values[ i] * hiddenErrorGradient; 
				weights[ i][ h] += delta; 
			} 
			
			double thresholdDelta = LEARNINGRATE * -1 * hiddenErrorGradient; 
			thresholds[ h] += thresholdDelta; 
		
		} 
		
		// update each weighting for the theta 
		double delta = LEARNINGRATE * -1 * outputErrorGradient; 
		thresholds[ o] += delta; 
	
	} 
	return sumOfSquaredErrors; 
} 


void displayNetwork( double values[], double sumOfSquaredErrors) 
{ 
	static int counter = 0; 
	
	if (( counter % 4) == 0) 
		printf("----------------------------------------------------\n"); 
		
	printf("% 8.4f |", values[ 1]); 
	printf("% 8.4f |", values[ 2]); 
	printf("% 8.4f |", values[ 5]); 
	printf(" err:% 8.5f\n", sumOfSquaredErrors); 
	
	counter++; 
}
