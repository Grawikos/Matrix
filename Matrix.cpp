#include "Matrix.h"
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

struct Matrix::rcmatrix {
	double** elements;
	unsigned int width;
	unsigned int hight;
	unsigned int n;

	rcmatrix(const double** nelements, unsigned int nwidth, unsigned int nhight)
	{
		n = 1;
		width = nwidth;
		hight = nhight;
		elements = new double*[hight];
		for (unsigned int i = 0; i < hight; ++i) {
			elements[i] = new double[width];
		}

		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < hight; j++) {
				elements[j][i] = nelements[j][i];
			}
		}
	}

	~rcmatrix()
	{
		for (unsigned int i = 0; i < hight; i++) {
			delete[] elements[i];
		}
		delete[] elements;
	}

	rcmatrix* detach()
	{
		if (n == 1)
			return this;
		rcmatrix* t = new rcmatrix((const double**)elements, width, hight);
		n--;
		return t;
	}

  private:
	rcmatrix(const rcmatrix&) = delete;
	rcmatrix& operator=(const rcmatrix&) = delete;
};

Matrix::Matrix()
{
	data = new rcmatrix(nullptr, 0, 0);
}

Matrix::Matrix(const Matrix& m)
{
	m.data->n++;
	this->data = m.data;
}

Matrix::Matrix(const double** c, int rows, int cols)
{
	data = new rcmatrix(c, rows, cols);
}

Matrix::Matrix(const string& s)
{
	int width = 0, hight = 0, currentWidth = 0;
	for (char c : s) {
		if (c == ' ')
			currentWidth++;
		if (c == '\n') {
			if (!width)
				width = ++currentWidth;
			else if (currentWidth && (width != currentWidth + 1)) {
				// throw Size();
				printf("size!");
			}
			if (currentWidth) {
				hight++;
				currentWidth = 0;
			}
		}
	}
	if(s.back() != '\n'){
		hight++;
	}
	if(!width)
		width = ++currentWidth; //here
	double** el = new double*[hight];
	for (int i = 0; i < hight; i++) {
		el[i] = new double[width]();
	}
	int pos = 0, size = 0, row = 0, col = 0;
	for (char c : s) {
		if (c == ' ') {
			string subS = s.substr(pos, size);
			el[col][row] = stod(subS);
			row++;
			pos += size += 1;
			size = 0;
		}
		else if (c == '\n' && size) {
			string subS = s.substr(pos, size);
			el[col][row] = stod(subS);
			pos += size += 1;
			row = 0;
			size = 0;
			col++;
		}
		else if (c == '\n')
			continue;
		else
			size++;
	}
	if(size){
		string subS = s.substr(pos, size);
		el[col][row] = stod(subS);
	}
	
	data = new rcmatrix((const double**)el, width, hight);
	
	for (int i = 0; i < hight; i++) {
			delete[] el[i];
		}
		delete[] el;
}

Matrix::Matrix(ifstream& inputFile){
	string line;
    double** elements = nullptr;
    int numRows = 0;
    int maxCols = 0;

    while (getline(inputFile, line)) {
        istringstream iss (line);
        double value;
        int numCols = 0;

        while (iss >> value) {
            numCols++;
        }

        if (numCols > maxCols) {
            maxCols = numCols;
        }

        numRows++;
    }

    inputFile.close();
    inputFile.open("example.txt");

    elements = new double*[numRows];
    for (int i = 0; i < numRows; i++) {
        elements[i] = new double[maxCols]();
    }

    int row = 0;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        int col = 0;
        double value;

        while (iss >> value) {
            elements[row][col] = value;
            col++;
        }

        row++;
    }
	data = new rcmatrix((const double**)elements, maxCols, numRows);
	
	for (int i = 0; i < numRows; i++) {
			delete[] elements[i];
		}
		delete[] elements;
}

Matrix::~Matrix()
{
	if (--data->n == 0)
		delete data;
}

ostream& operator<<(ostream& out, const Matrix& m)
{
	for (unsigned int i = 0; i < m.data->hight; i++) {
		for (unsigned int j = 0; j < m.data->width; j++) {
			out << m.data->elements[i][j] << ", ";
		}
		out << "\n";
	}
	return out;
}

Matrix& Matrix::operator=(cont Matrix& m){
	this->data = m.data;
	m.data->n++;
}
