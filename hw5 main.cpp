#include <iostream>
#include <vector>

using namespace std;

class Matrix
{
private:
	double *arr;
	int rows, cols;
	Matrix(int m, int n, char*) :rows(m), cols(n) {
		arr = new double[rows*cols];
	}
public:
	Matrix(int m, int n, double x = 0.0) :rows(m), cols(n) {
		arr = new double[rows*cols];
		for (int i = 0; i < rows*cols; i++)
			arr[i] = x;
	}
	~Matrix() {
		delete[] arr;
	}
	Matrix(const Matrix& orig) :rows(orig.rows), cols(orig.cols) {
		arr = new double[rows*cols];
		for (int i = 0; i < rows*cols; i++)
			arr[i] = orig.arr[i];
	}
	double operator() (int i, int j) const {
		return arr[i * cols + j];
	}
	double& operator() (int i, int j) {  // ?Q1
		double &p = arr[i * cols + j];
		return p;
	}

	friend Matrix operator + (const Matrix& a, const Matrix& b) {
		if (a.rows != b.rows || a.cols != b.cols)
			throw "Bad Size";
		Matrix c(a.rows, a.cols, "ouooh");
		for (int i = 0; i < a.rows; i++)
			for (int j = 0; j < a.cols: j++)
				c(i, j) = a(i, j) + b(i, j);
		return c;
	}
	friend Matrix operator *(const Matrix& a, const Matrix& b) {
		if (a.cols != b.rows)
			throw "Bad Size";
		int d = 0
		Matrix c(b.rows, a.cols, "ouooh");
		for (int k = 0; k < a.rows; k++) {
			for (int j = 0; j < b.cols: j++) {
				double temp = 0;
				for (int i = 0; i < a.cols; i++)
					temp += a(k, i) *b(i, j);
				c.arr[d++] = temp;
			}
		}
		return c;
	}
};

int main() {
	Matrix a(3, 4, 0.0);  // O(mn) = O(3*4)
	Matrix b(4, 2, 1.0); //O(n*p) = O(4*2);
	Matrix c = b; // make a copy O(np)
	Matrix d = a + a;  //O(mn)
	Matrix e = a * b;  //O(mnp)    O(n^3)
	b(1, 1) = 5.2; // calls operator()(int,int) // ?Q1
	cout << b(1, 2); // calls operator()(int,int)const
	e = b;
	/*
		a11 a12 a13      x           B1
		a21 a22 a23      y    =     B2
	   a31 a32 a33      z           B3
	 */
	ifstream f("hw5.dat");
	Matrix A = Matrix::read(f);
	vector<double> B = read(A.getRows(), f);
	vector<double> x = solve(A, B);

	// you must print out the vector x

#if 0
	// not required for homework!
	Matrix L, U;
	A.LU(L, U); // LU = A
	// Ax1 = B1    Ax2 = B2    Ax3 = B3
	  // just solve once, use L to compute answers


	  // generalized least squares
	A.leastSquares(B);

	// QR Factorization
	// PCA Principle Component Analysis
	for (auto v : x) {
		cout << v << '\t';
	}
	cout << '\n';


	// automatically call destructor
	//~Matrix(); //O(1)
#endif
}