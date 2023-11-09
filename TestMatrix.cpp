#include "Matrix.h"													
#include <iostream>

using namespace std;
/*
void testCpy(){
	cout << "Test copy, asing, <<\n";
	Matrix m1, m2 = "1 1\n2 2", m3 = "3 3", 
		   m4 = {{0, 0, 0}, {4, 4, 4}, {8, 8, 8}}
	Matrix m5(m2);														//copy
	cout << m1 <<"\n"<< m2 <<"\n"<< m3 <<"\n"<<m4<<"\n"<< m5 <<endl;
	m2 = m3;															//smaller to bigger
	m3 = m4;															//bigger to smaller
	m1 = m4;															//empty
	cout << "After operations:\n";
	cout << m1 <<"\n"<< m2 <<"\n"<< m3 <<"\n"<<m4<<"\n"<< m5 <<endl;
}

void testAryth(){
	cout << "Arythmetic operations:\n";
	Matrix m1 = {{1, 1}, {2, 2}}, m2 = {{3, 4}, {-5, -6}},
		   m3 = {0, 0};
	cout << "m1:\n"<< m1 <<"m2:\n"<< m2 <<"m3:\n"<< m3 <<"\n";
	cout << "m1+m2 = "<< m1 + m2;
	cout << "m1 + m3 = "<< m1 + m3;
	
	
}
*/

int main(int argc, char* argv[]){
//	testCpy();
//	testAryth();

	string filename = "example.txt";
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
    }
	
	string content = "2 3 4\n1.3 4.98 5";
	
	Matrix m1(content);
	Matrix m2(inputFile);
	cout << m1 << '\n' << m2;
	
	inputFile.close();

	return 0;
}
