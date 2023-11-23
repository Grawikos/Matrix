#include "Matrix.h"
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>

using namespace std;

struct Matrix::rcmatrix {
	double** elements;
	unsigned int width;
	unsigned int height;
	unsigned int n;

	rcmatrix() : width(1), height(1)
	{
		n = 1;
		elements = new double*[height];
		for (unsigned int i = 0; i < height; ++i) {
			elements[i] = new double[width];
		}
		elements[0][0] = 0;
	}
	rcmatrix(unsigned int width, unsigned int height)
	{
		n = 1;
		this->width = width;
		this->height = height;
		elements = new double*[height];
		for (unsigned int i = 0; i < height; ++i) {
			elements[i] = new double[width];
		}
		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < height; j++) {
				elements[j][i] = 0;
			}
		}
	}
	rcmatrix(const double** nelements, unsigned int nwidth,
			 unsigned int nheight)
	{
		n = 1;
		width = nwidth;
		height = nheight;
		elements = new double*[height];
		for (unsigned int i = 0; i < height; ++i) {
			elements[i] = new double[width];
		}

		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < height; j++) {
				elements[j][i] = nelements[j][i];
			}
		}
	}

	~rcmatrix()
	{
		for (unsigned int i = 0; i < height; i++) {
			delete[] elements[i];
		}
		delete[] elements;
	}

	rcmatrix* detach()
	{
		if (n == 1)
			return this;
		rcmatrix* t = new rcmatrix((const double**)elements, width, height);
		n--;
		return t;
	}

  private:
	rcmatrix(const rcmatrix&) = delete;
	rcmatrix& operator=(const rcmatrix&) = delete;
};

Matrix::Matrix()
{
	data = new rcmatrix();
}

Matrix::Matrix(unsigned int i, unsigned int j)
{
	data = new rcmatrix(i, j);
}

Matrix::Matrix(const Matrix& m)
{
	m.data->n++;
	this->data = m.data;
}

Matrix::Matrix(const double** c, unsigned int rows, unsigned int cols)
{
	data = new rcmatrix(c, rows, cols);
}

void Matrix::findDimentions(unsigned int& width, unsigned int& height, const string& s){
	unsigned int currentWidth = 0;
	for (char c : s) {
		if (c == ' ')
			currentWidth++;
		else if (c == '\n') {
			if (!width)
				width = ++currentWidth;
			else if (currentWidth && (width != currentWidth + 1)) {
				throw Size();
			}
			if (currentWidth) {
				height++;
				currentWidth = 0;
			}
		}
		else if((c >= '0' && c <= '9') || c == '-' || c == '.')
			continue;
		else
			throw NaN();
	}
	if (s.back() != '\n') {
		height++;
	}
	if (!width)
		width = ++currentWidth; // here
}

Matrix::Matrix(const string& s)
{
	unsigned int width = 0, height = 0, pos = 0, size = 0, row = 0, col = 0;
	findDimentions(width, height, s);
	double** el = new double*[height];
	for (unsigned int i = 0; i < height; i++) {
		el[i] = new double[width]();
	}
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
	if (size) {
		string subS = s.substr(pos, size);
		el[col][row] = stod(subS);
	}
	data = new rcmatrix((const double**)el, width, height);
	for (unsigned int i = 0; i < height; i++) {
		delete[] el[i];
	}
	delete[] el;
}

void Matrix::findDimentionsFile(unsigned int& numRows, unsigned int& maxCols, ifstream& inputFile){
	string line;
	while (getline(inputFile, line)) {
		istringstream iss(line);
		double value;
		unsigned int numCols = 0;

		while (iss >> value) {
			numCols++;
		}
		if (line.size() > 0)
			numRows++;

		if (!maxCols)
			maxCols = numCols;

		if (numCols != maxCols)
			throw Size();
	}

	inputFile.close();
	
}

Matrix::Matrix(ifstream& inputFile)
{
	string line;
	unsigned int numRows = 0, maxCols = 0;
	findDimentionsFile(numRows, maxCols, inputFile);
	inputFile.open("example.txt");
	double** elements = new double*[numRows];
	for (unsigned int i = 0; i < numRows; i++) {
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

	for (unsigned int i = 0; i < numRows; i++) {
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
	for (unsigned int i = 0; i < m.data->height; i++) {
		for (unsigned int j = 0; j < m.data->width; j++) {
			out << m.data->elements[i][j] << ", ";
		}
		out << "\n";
	}
	return out;
}

bool Matrix::checkIndexes(Matrix& m, unsigned int row, unsigned int col)
{
	if (row < m.data->height && col < m.data->width)
		return true;
	return false;
}

Matrix& Matrix::operator=(const Matrix& m)
{
	m.data->n++;
	if (--data->n == 0)
		delete data;
	this->data = m.data;
	return *this;
}

double Matrix::read(unsigned int row, unsigned int col)
{
	return data->elements[row][col];
}

void Matrix::write(unsigned int row, unsigned int col, double val)
{
	data = data->detach();
	data->elements[row][col] = val;
}

Matrix::Cref Matrix::operator()(unsigned int row, unsigned int col)
{
	if (checkIndexes(*this, row, col))
		return Matrix::Cref(*this, row, col);
	throw Size();
}

Matrix& Matrix::operator+=(Matrix& m)
{
	if (!checkIndexes(*this, m.data->height - 1, m.data->width - 1))
		throw Size();
	rcmatrix* newData =
		new rcmatrix((const double**)data->elements, data->width, data->height);

	for (unsigned int i = 0; i < m.data->height; i++) {
		for (unsigned int j = 0; j < m.data->width; j++) {
			if ( (newData->elements[i][j] < 0.0) == (m.data->elements[i][j] < 0.0)
				&& abs( m.data->elements[i][j] ) > numeric_limits<double>::max() - abs( newData->elements[i][j] ) ) {
					delete newData;
					throw Overflow();
			}
			newData->elements[i][j] += m.data->elements[i][j];
		}
	}
	if (--data->n == 0)
		delete data;
	data = newData;
	return *this;
}

Matrix& Matrix::operator-=(Matrix& m)
{
	if (!checkIndexes(*this, m.data->height - 1, m.data->width - 1))
		throw Size();
	rcmatrix* newData =
		new rcmatrix((const double**)data->elements, data->width, data->height);
	for (unsigned int i = 0; i < m.data->height; i++) {
		for (unsigned int j = 0; j < m.data->width; j++) {
			if ( (newData->elements[i][j] < 0.0) != (m.data->elements[i][j] < 0.0)
				&& abs( m.data->elements[i][j] ) > numeric_limits<double>::max() - abs( newData->elements[i][j] ) ) {
					delete newData;
					throw Underflow();
			}
			newData->elements[i][j] -= m.data->elements[i][j];
		}
	}
	if (--data->n == 0)
		delete data;
	data = newData;

	return *this;
}

double Matrix::dotProductRC(Matrix& lhs, Matrix& rhs, unsigned int row,
							unsigned int col)
{
	double result = 0;
	for (unsigned int i = 0, j = 0; i < lhs.data->width; i++, j++) {
		if (abs(lhs.data->elements[row][i]) > numeric_limits<double>::max() / abs(rhs.data->elements[j][col])) {
					throw Overflow();
			}
		
		double prod = lhs.data->elements[row][i] * rhs.data->elements[j][col];
		if (( prod < 0.0) == (result < 0.0)
				&& abs(prod) > numeric_limits<double>::max() - abs(result) ) {
					throw Overflow();
			}
		result += prod;
	}
	return result;
}

Matrix& Matrix::operator*=(Matrix& m)
{
	if (this->data->width != m.data->height)
		throw Size();
	rcmatrix* newMatrix = new rcmatrix(this->data->height, m.data->width);
	for (unsigned int i = 0; i < newMatrix->height; i++) {
		for (unsigned int j = 0; j < newMatrix->width; j++) {
			try{
				newMatrix->elements[i][j] = dotProductRC(*this, m, i, j);
			}catch(Matrix::Overflow){
				delete newMatrix;
				throw;
			}
		}
	}
	if (--data->n == 0)
		delete data;

	data = newMatrix;
	return *this;
}
Matrix Matrix::operator+(Matrix& m)
{
	Matrix newM(*this);
	return newM += m;
}
Matrix Matrix::operator-(Matrix& m)
{
	Matrix newM(*this);
	return newM -= m;
}
Matrix Matrix::operator*(Matrix& m)
{
	Matrix newM(*this);
	return newM *= m;
}

bool Matrix::operator==(Matrix& m)
{
	if (!checkIndexes(*this, m.data->width - 1, m.data->height - 1))
		return false;
	for (unsigned int i = 0; i < m.data->height; i++) {
		for (unsigned int j = 0; j < m.data->width; j++) {
			if (!(this->data->elements[i][j] == m.data->elements[i][j]))
				return false;
		}
	}
	return true;
}

bool Matrix::operator!=(Matrix& m)
{
	if (!checkIndexes(*this, m.data->width - 1, m.data->height - 1))
		return true;
	for (unsigned int i = 0; i < m.data->height; i++) {
		for (unsigned int j = 0; j < m.data->width; j++) {
			if (!(this->data->elements[i][j] == m.data->elements[i][j]))
				return true;
		}
	}
	return false;
}
