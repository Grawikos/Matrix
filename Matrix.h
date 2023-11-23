#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#pragma once

using namespace std;

class Matrix {
  private:
	struct rcmatrix;
	rcmatrix* data;

	bool checkIndexes(Matrix&, unsigned int, unsigned int);
	double dotProductRC(Matrix& lhs, Matrix& rhs, unsigned int, unsigned int);
	void findDimentions(unsigned int&, unsigned int&, const string&);
	void findDimentionsFile(unsigned int&, unsigned int&, ifstream&);

  public:
	class Size {};
	class NaN {};
	class Overflow {};
	class Underflow {};
	class Cref;
	Matrix();
	Matrix(unsigned int, unsigned int);
	Matrix(const Matrix& m);
	Matrix(const double**, unsigned int rows, unsigned int cols);
	Matrix(const std::string& s);
	Matrix(std::ifstream& s);
	~Matrix();
	friend std::ostream& operator<<(std::ostream&, const Matrix&);
	Matrix& operator=(const Matrix& m);

	double read(unsigned int, unsigned int);
	void write(unsigned int, unsigned int, double);
	Cref operator()(unsigned int, unsigned int);

	Matrix& operator+=(Matrix& m);
	Matrix& operator-=(Matrix& m);
	Matrix& operator*=(Matrix& m);
	Matrix operator+(Matrix& m);
	Matrix operator-(Matrix& m);
	Matrix operator*(Matrix& m);

	bool operator==(Matrix& m);
	bool operator!=(Matrix& m);
};

class Matrix::Cref {
	friend class Matrix;
	Matrix& m;
	int row;
	int col;
	Cref(Matrix& mm, unsigned int rrow, unsigned int ccol)
		: m(mm), row(rrow), col(ccol){};

  public:
	operator double() const
	{
		return m.read(row, col);
	}
	Matrix::Cref& operator=(double d)
	{
		m.write(row, col, d);
		return *this;
	}
	Matrix::Cref& operator=(const Cref& ref)
	{
		return operator=((double)ref);
	}
	void operator+=(double d)
	{
		double val = m.read(row, col);
		if ((val < 0.0) == (d < 0.0) &&
			abs(val) > numeric_limits<double>::max() - abs(d)) {
			throw Overflow();
		}
		m.write(row, col, val + d);
	}
	void operator-=(double d)
	{
		double val = m.read(row, col);
		if ((val < 0.0) != (d < 0.0) &&
			abs(val) > numeric_limits<double>::max() - abs(d)) {
			throw Underflow();
		}
		m.write(row, col, val - d);
	}
	void operator*=(double d)
	{
		double val = m.read(row, col);
		if (abs(val) > numeric_limits<double>::max() / abs(d)) {
			throw Overflow();
		}
		m.write(row, col, val * d);
	}
	
	void operator+(double d){
		*this += d;
	}
	void operator-(double d){
		*this -= d;
	}
	void operator*(double d){
		*this *= d;
	}	
};

//#endif
