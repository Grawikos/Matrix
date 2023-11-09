#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#pragma once


class Matrix {
	private:
		struct rcmatrix;
		rcmatrix* data;		
	public:
		Matrix();
		Matrix(const Matrix& m);
		Matrix(const double**, int rows, int cols);
		Matrix(const std::string& s);
		Matrix(std::ifstream& s);
		~Matrix();
		friend std::ostream& operator<<(std::ostream&, const Matrix&);
				
		Matrix& operator=(const Matrix& m);

};






//#endif
