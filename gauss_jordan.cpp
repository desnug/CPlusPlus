#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class AugmentedMatrix {
public:
	AugmentedMatrix(int rows, int cols)	//creates new augmented matrix with set dimensions
	{
		amat.assign(rows, vector<double>(cols, 0));	//fills vector of vector of doubles with 0s
	}

	AugmentedMatrix()	//default constructor for unknown dimensions
	{
	}

	~AugmentedMatrix()	//destructor
	{
		amat.clear();	//clear augmented matrix
	}

	void read()	//reads in user input for values of matrix
	{
		cout << "\n\nSeparate entries with spaces. Press 'q' to end; 's' to use sample matrix." << endl;	//prompt input
		while (true) {	//loop until a break occurs
			vector<double> newRow;	//stores entered values as a new row vector

			string input;	//for storing user input
			getline(cin, input);	//writes cin to input
			if (input.find_first_of("q") != string::npos) {	//if a q's position is not no-position (ie it's found)
				break;	//exit loop; stop enering new rows
			}
			if (input.find_first_of("s") != string::npos) { //if a s's position is not no-position (ie it's found)
				amat = { {1, 2, 3, 3}, {2, 3, 2, 3}, {2, 1, 2, 5} };	//hardcoded sample matrix
				break;	//exit loop; stop enering new rows
			}
			stringstream stream(input);	//string stream processes for numbers
			double temp;	//temporary double to hold values from string
			while (stream >> temp)	//while there are still more numbers to store on temp from stream
			{
				//for keeping count of spaces necessary to print matrix
				int digits = (int)floor(log10(temp)) + 1;	//count number of digits in temp
				if (digits > maxDigits) {	//if more digits than previous max
					maxDigits = digits;	//replace maxDigits with new king
				}
				newRow.push_back(temp);	//insert the number into the new row vector
			}
			amat.push_back(newRow);	//insert the new row vector into the matrix
		}
	}

	int rows()	//returns # of rows in augmented matrix
	{
		return amat.size();	//# of vectors under amat
	}

	int cols()	//returns # of columns in augmented matrix
	{
		return amat[0].size();	//# of components of 0th vector of amat
	}

	void rref()	//performs 3 step alogrithm to put amat into reduced row-echelon form
	{
		int i = 0;	//ith equation
		int xj = 0;	//jth (leading) variable
		while (i < rows() && xj < cols() - 1) {	//go through every row and variable (1 less than # of cols for aug matrix)
			if (amat[i][xj] == 0) {	//if xj (leading variable) does not appear in ith equation
				int swapRow = findFirstRow(xj, i);	//find index of row first containing xj (following row i)
				if (swapRow == -1) {	//if no swappable row is found
					xj++;	//move on to next variable, starting over (but test same row again)
					//cout << ", continuing to variable " << xj;	//info
					continue; //go to top of loop
				}
				else {
					swapRows(i, swapRow);	//swap ith row with row in which xj appears
				}
			}
			if (amat[i][xj] != 1) {	//if coefficient of xj is not 1
				divideRowByConst(i, amat[i][xj]);	//divide by coefficient of xj
			}
			for (int row = 0; row < rows(); row++) {	//for every row, above and below
				if (row != i && amat[row][xj] != 0) {	//except current row and row where xj is already eliminated
					subtractMultiple(row, i, amat[row][xj]);	//eliminate xj by subtracting suitable multples of ith equation from others
				}
			}
			i++, xj++;	//increase row and variable counters
		}
	}

	void print()	//prints augmented matrix
	{
		cout << endl;	//start new line
		for (int i = 0; i < rows(); i++) {	//for every row
			for (int j = 0; j < cols(); j++) {	//for every column
				if (j == cols() - 1) {	//if on final, augmented column
					cout << " |" << setw(maxDigits + 5 - 2) << setprecision(3) << amat[i][j];	//print with dash, subtract 2 width for " |"
				}
				else {	//otherwise, for a coefficient
					cout << setw(maxDigits + 5) << setprecision(3) << amat[i][j];	//print with extra space
				}
			}
			cout << endl;	//finish line
		}
	}

	void printSoln()	//prints solutions to system
	{
		cout << endl;	//start new line
		if (isInconst()) {	//if an inconsistency exists
			cout << "Stystem is inconsistent. No such cookie.";	//print message
		}
		else {	//otherwise, a solution exists
			vector<int> vars;	//stores solved variables' indices
			for (int i = 0; i < rows(); i++) {	//for every row
				for (int j = 0; j < cols(); j++) {	//for every column
					if (amat[i][j] != 0) {	//if entry is not 0
						vars.push_back(j);	//insert its column (index) into vars
						break;	//go start new row
					}
				}
			}
			vector<string> soln;	//stores expressions of solved variables
			for (int i = 0; i < vars.size(); i++) {	//for every solved variable index
				string exp = "";	//start with empty expression
				double con = amat[i][cols() - 1];	//for clarity, call the final entry of the row constant
				exp += doubleToString(con);	//first add constant term from augmented matrix
				for (int j = vars[i] + 1; j < cols() - 1; j++) {	//then subtract over the rest of the terms
					double coeff = amat[i][j];	//for clarity, call the entry coefficient
					if (coeff > 0) {	//positive terms become minus
						if (closeEnough(coeff, 1)) {	//if single term
							exp += " - x_" + doubleToString(j);	//ignore 1 in 1 * x
						}
						else {
							exp += " - " + doubleToString(abs(coeff)) + " x_" + doubleToString(j);	//concat coefficients and parameters
						}
					}
					else if (coeff < 0) {	//negative terms become plus (absolute value to drop old negative sign)
						if (closeEnough(coeff, -1)) {	//if single term
							exp += " + x_" + doubleToString(j);	//ignore 1 in 1 * x
						}
						else {
							exp += " + " + doubleToString(abs(coeff)) + " x_" + doubleToString(j);	//concat coefficients and parameters
						}
					}
					//amat[i][j] == 0 ignored to leave behind 0 coefficients
				}
				soln.push_back(exp);	//finally, insert expression to soln
			}

			for (int i = 0; i < vars.size(); i++) {	//for every solved variable
				cout << "x_" << vars[i] << " = " << soln[i] << endl;	//print it with its expression
			}
		}
		cout << endl;
	}


private:
	vector<vector<double>> amat;	//augmented matrix
	int maxDigits = 1;	//maximum # of digits in amat, default at 1
	double epsilon = 0.001;	//tolerance for rounding

	void divideRowByConst(int i, double c)	//divides every component of row vector by a scalar
	{
		for (int j = 0; j < cols(); j++) {	//for every entry in row
			amat[i][j] /= c;	//divide entry by c
		}
		if (c > 1) { //if constant divisor was greater than 1
			cout << "Divide row " << i << " by " << c;	//info
		}
		else if (c < 1) { //if constant divisor was less than 1
			cout << "Multiply row " << i << " by " << 1 / c;	//info
		}
		print();	//info
	}

	void subtractMultiple(int row1, int row2, double c)	//form: row1 - c * row2
	{
		for (int j = 0; j < cols(); j++) {	//for every entry of row1
			amat[row1][j] -= c * amat[row2][j];	//subtract c multiples of row2 from corresponding row1 entry
		}
		if (c > 0) {	//if constant multiple was positive
			if (closeEnough(c, 1)) {	//if single subraction, change grammar
				cout << "Subtract row " << row2 << " from row " << row1;	//info
			}
			else {	//otherwise, normal multiplication
				cout << "Subtract " << abs(c) << " times row " << row2 << " from row " << row1;	//info
			}

		}
		else if (c < 0) {	//if constant multiple was negative
			if (closeEnough(c, -1)) {	//if single addition, change grammar
				cout << "Add row " << row2 << " to row " << row1;	//info
			}
			else {//otherwise, normal multiplication
				cout << "Add " << abs(c) << " times row " << row2 << " to row " << row1;	//info
			}
		}
		print();	//info
	}

	void swapRows(int i1, int i2)	//swaps position of tow rows
	{
		amat[i1].swap(amat[i2]);	//vector swap
		cout << "Swap row " << i1 << " with row " << i2;	//info
		print();	//info
	}

	int findFirstRow(int xj, int start)	//find first row containing xj, after the 'start' row
	{
		for (int i = start + 1; i < rows(); i++) {
			if (amat[i][xj] != 0) {
				//cout << "First instance of variable " << xj << " was found on row " << i;
				return i;
			}
		}
		//cout << "No instance of variable " << xj << " was found";
		return -1;	//not found
	}

	bool isInconst() {	//checks whether system has a contradiction (eg 0 = 6)
		for (int i = 0; i < rows(); i++) {	//for every row
			for (int j = 0; j < cols(); j++) {	//for all coefficients
				if (j < cols() - 1 && amat[i][j] != 0) {	//if any nonzero coeffcient is found
					//cout << "row " << i << " is ok" << endl;
					break;	//row is fine, check next row
				}
				if (j == cols() - 1 && amat[i][j] != 0) {	//if the constant term is nonzero
					//cout << "inconst row " << i;
					return true;	//inconsistent system, since all coefficients were 0, but constant term was nonzero
				}
			}
		}
		return false;	//if no false statements encountered, system is not inconsistent
	}

	bool closeEnough(double a, double b) {
		return (abs(a - b) <= epsilon);
	}

	string doubleToString(double d) {	//outputs of string of the given double, using the minimum precision (ie no trailing 0's)
		//calculate minimum precision
		int p = 0;	//start with 0 decimal places required
		double temp = d;	//use temporary double to leave original intact
		while (!closeEnough(temp, round(temp))) {	//while the remaining decimal is too big
			temp *= 10;	//multiply by 10 in order to move tenths place up to ones, and leave remainder for rechecking
			p++;	//increase required precision
		}

		//output double to string using specified precision
		stringstream stream;	//string stream to format double
		stream << fixed << setprecision(p) << d;	//put d to string, with p decimals places
		string s = stream.str();	//output of stream to s
		return s;	//send s back
	}
};

int main()
{
	//intro text
	cout << "Welcome to Samuel's Gauss-Jordan Elimination program!";	//welcome message
	cout << "\n\nThis program is designed to solve a system of linear equations, in the general form Ax = b,";	//info
	cout << "\nwhere A is an mxn matrix, x is a column vector with n entries, and b is a column vector with m entries.";	//info
	cout << "\nPlease enter the augmented matrix representing A|b. (The final entry in a row will be assumed to be an entry of b.)";	//info


	AugmentedMatrix aug = AugmentedMatrix();	//creates new matrix of undetermined dimensions
	aug.read();	//reads in user entries
	cout << "\n\nYour augmented matrix\n---------------------";	//message result, for typo checking
	aug.print();	//prints matrix

	cout << "\n\nReducing...\n";	//message for reducing algorithm
	aug.rref();	//manipulates augmented matrix until in reduced row-echelon form, printing steps along the way

	cout << "\n\nReduced Row-Echelon Form\n------------------------";	//message for rref result
	aug.print();	//prints matrix

	cout << "\n\nSolutions\n---------";	//message for system solution
	aug.printSoln();	//prints solution

	//closing text
	cout << "\nWere you using this on you homework...? *tsk* *tsk*" << endl;

	//exit text
	system("pause");
}
