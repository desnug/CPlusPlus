#include <iostream>
using namespace std;

double faktorial(double bil, double hasil)
{
    if (bil == 0)
    {
        //stop rekursif
        cout << '=';
        return hasil;
    }
    else
    {
        hasil *= bil;
        cout << bil;
        if (bil != 1)
            cout << '*';
        // rekursive
        faktorial(bil - 1, hasil);
    }
}

int main()
{
    double bil, hasil = 1;
    cout << "Factorial with rekursive" << endl;
    cout << "Input Number : ";
    cin >> bil;
    cout << bil << "! = ";
    cout << faktorial(bil, hasil);
}
