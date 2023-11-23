#include "Matrix.h"
//#include "Cref.h"
#include <iostream>

using namespace std;

void testCpyAsign(Matrix mf)
{
	cout << "Test copy, asing, <<\n";
	Matrix m1, m2("1 1\n2 2"), m3("3 3"), m4("1 1 1\n4 4 4\n8 8 8");
	Matrix m5(m2); // copy
	cout << "mf\n"
		 << mf << "m1\n"
		 << m1 << "\nm2\n"
		 << m2 << "\nm3\n"
		 << m3 << "\nm4\n"
		 << m4 << "\nm5\n"
		 << m5 << endl;
	cout << "oerations: m1 = m4, m2 = m3, m3 = m4, m4 = mf, mf = m5\n";
	m2 = m3; // smaller to bigger
	m3 = m4; // bigger to smaller
	m1 = m4; // empty
	m4 = mf;
	mf = m5;
	cout << "After operations:\n";
	cout << "m1\n"
		 << m1 << "m2\n"
		 << m2 << "m3\n"
		 << m3 << "m4\n"
		 << m4 << "m5\n"
		 << m5 << "mf\n"
		 << mf << endl;
	cout << "Try creating matrix with wrong element\n1.1 -22\n3 g";
	try{
		Matrix mfail("1.1 -22\n3 g");
	}catch(Matrix::NaN){
		cout << "\nNaN() caught\n";
	}
}

void testAryth(Matrix mf)
{
	cout << "\nArythmetic operations:\n";
	Matrix m1, m2("1 1\n2 2"), m3("3 3\n-2 1"), m4("1 1 1\n4 4 4\n8 8 8"),
		m5("-1 -5\n2 3\n4 2.2");
	cout << "Addition\n";
	cout << m2 << " + \n" << m3 << " = \n" << m2 + m3;
	cout << "Substraction\n";
	cout << m2 << " - \n" << m3 << " = \n" << m2 - m3;
	cout << "Multiplication\n";
	cout << m2 << " * \n" << m3 << " = \n" << m2 * m3;
	cout << "File operations (mf + m5):\n"
		 << mf << " + \n"
		 << m5 << "=\n"
		 << mf + m5;
	Matrix m21(m2), m22(m2), m23(m2);
	m21 += m3;
	m22 -= m3;
	m23 *= m3;
	cout << "Same operations, with assignment:\n";
	cout << "m2 += m3  \n" << m21;
	cout << "Substraction\n";
	cout << "m2 -= m3  \n" << m22;
	cout << "Multiplication\n";
	cout << "m2 *= m3  \n" << m23;
}

void testWriteRead()
{
	Matrix m1, m2("1 1\n2 2");
	cout << m1 << '\n' << m2 << endl;
	double d = m2(0, 0);
	m2(0, 0) = 999;
	cout << d << "\n" << m2 << endl;
	try {
		m1(1, 1) = 5;
	}
	catch (Matrix::Size) {
		cout << "Size() caught" << endl;
	}
}

void testBoolean()
{
	cout << "Boolean test\n";
	Matrix m1("1 1\n2 2"), m2(m1), m3("1 1\n2 2\n3 3"), m4("0 0\n0 0");
	cout << "m1\n"
		 << m1 << "m2\n"
		 << m2 << "m3\n"
		 << m3 << "m4\n"
		 << m4 << endl;
	cout << "m1 == m2: " << (m1 == m2) << endl;
	cout << "m1 != m2: " << (m1 != m2) << endl;
	cout << "m1 == m3: " << (m1 == m3) << endl;
	cout << "m1 != m3: " << (m1 != m3) << endl;
	cout << "m1 == m4: " << (m1 == m4) << endl;
	cout << "m1 != m4: " << (m1 != m4) << endl;
}

void testOUflow(){
	cout << "\nTest Overflow/Underflow:\n";
	Matrix m1("-1 1\n2 2"), m2("5 5\n-5 5");
	m1(0, 0) += numeric_limits<double>::max();
	m1(0, 1) -= numeric_limits<double>::max();
	try{
		m1 += m2;
	}catch(Matrix::Overflow){
		cout << "Overflow() caught\n";
	}
	try{
		m1 -= m2;
	}catch(Matrix::Underflow){
		cout << "Uverflow() caught\n";
	}
	try{
		m1 *= m2;
	}catch(Matrix::Overflow){
		cout << "Overflow() caught\n";
	}
	
}

int main(int argc, char* argv[])
{

	string filename = "example.txt";
	ifstream inputFile(filename);
	if (!inputFile.is_open()) {
		cerr << "Failed to open the file." << endl;
		return 1;
	}
	Matrix mf(inputFile);
	inputFile.close();

	testCpyAsign(mf);
	testAryth(mf);
	testWriteRead();
	testBoolean();
	testOUflow();

	return 0;
}
