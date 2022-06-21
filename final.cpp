#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <fstream>
#include "qbMatrix.h"
#include "qbVector.h"
#include "qbPCA.h"
#include <itpp/itsignal.h>
#include <cstdio>
#define _CRT_SECURE_NO_WARNINGS
using namespace itpp;
using namespace std;

// Creating a namespace
namespace wisdom{
	class PCA{
		public:
		int nrSamples=20000;
		int nrComp=16;
		void m1(){
			string rowData,number;
			stringstream rowDataStream;
			vector<vector<double>> Data(nrSamples,vector<double>(nrComp));
			int numRows = 0;
			int numCols = 0;
			ifstream inputFile("eegData.csv");
			if (inputFile.is_open()){		
				while (!inputFile.eof()){
					getline(inputFile, rowData);
					rowDataStream.clear();
					rowDataStream.str(rowData);
					numCols = 0;
					while (rowDataStream.good()){
						getline(rowDataStream,number, ',');
						if (!number.empty()) {
							double v=stod(number);
							Data[numRows][numCols]=v;
						}
						numCols += 1;
						if(numCols==nrComp)
							break;
					}
					numRows += 1;
					if(numRows==nrSamples)
						break;
				}
				inputFile.close();
			}
			vector<double> testData;
			for(int i=0;i<nrSamples;i++){
				for(int j=0;j<nrComp;j++)
					testData.push_back(Data[i][j]);
			}
			qbMatrix2<double> X (numRows, numCols,testData);
			qbMatrix2<double> eigenvectors;
			int testResult2 = qbPCA::qbPCA(X, eigenvectors);
			qbMatrix2<double> V, part2;
			eigenvectors.Separate(V, part2, 2);
			qbMatrix2<double> newX = (V.Transpose() * X.Transpose()).Transpose();
			vector<vector<double>> result(nrSamples,vector<double>(2));
			for(int i=0;i<nrSamples;i++){
				for(int j=0;j<2;j++)
					result[i][j]=newX.GetElement(i,j);
			}
			ofstream outputFile("Result1.csv");
			if (outputFile.is_open()){
				for (int i=0; i<newX.GetNumRows(); ++i)
					outputFile << newX.GetElement(i, 0) << "," << newX.GetElement(i, 1) << endl;
				outputFile.close();
			}
		}
	};
	class ICA{
		public:
		int nrSamples = 1000; 
		int nrIC = 16;
		void m2(){
			FILE* fpin = NULL;
    			float tmp = 0.0;
    			string rowData,number;
    			stringstream rowDataStream;
    			vector<vector<double>> Data(nrSamples, vector<double>(nrIC));
    			int numRows = 0;
    			int numCols = 0;
    			ifstream inputFile("eegData.csv");
    			if (inputFile.is_open()){
        			while (!inputFile.eof()){
            				getline(inputFile, rowData);
            				rowDataStream.clear();
            				rowDataStream.str(rowData);
            				numCols = 0;
            				while (rowDataStream.good()){
                				getline(rowDataStream, number, ',');
                				if (!number.empty()){
                    					double v = stod(number);
                    					Data[numRows][numCols] = v;
                				}
                				numCols += 1;
                				if (numCols == nrIC)
                    					break;
            				}
            			numRows += 1;
            			if (numRows == nrSamples)
                			break;
        			}
        			inputFile.close();
    			}
    			mat X = zeros(nrIC, nrSamples);
    			for (int i = 0; i < nrSamples; i++){
        			for (int j = 0; j < 16; j++)
    	 	 	      		X(j, i) = Data[i][j];
            		}
    			Fast_ICA my_fastica(X);
    			my_fastica.set_nrof_independent_components(nrIC);
    			bool result = my_fastica.separate();
    			vector<vector<double>> res(nrSamples, vector<double>(nrIC));
    			if (result){
        			my_fastica.get_mixing_matrix();
        			my_fastica.get_separating_matrix();
        			mat b=my_fastica.get_independent_components();
        			//cout<<my_fastica.get_independent_components()<<endl;
        			for (int i=0;i<nrSamples;i++){
					for(int j=0;j<nrIC;j++)
						res[i][j]=b(j,i);
				}
        			ofstream outputFile("Result2.csv");
				if(outputFile.is_open()){
					for (int i=0;i<nrSamples;i++){
						for(int j=0;j<(nrIC-1);j++)
							outputFile << res[i][j] <<",";
						outputFile << res[i][nrIC-1] << endl;
					}
					outputFile.close();
				}	
    			}
    			else{
        			cout << "Algorithm failed" << endl;
    			}
		}
	};
}


int main(){
	wisdom::PCA obj1;
	obj1.m1();
	wisdom::ICA obj2;
	obj2.m2();
	return 0;
}

