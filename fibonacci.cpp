#include <stdio.h>
#include <iostream>
using namespace std;

int fibonacci(int m)
{
    if (m == 0 || m == 1)
    {
        return m;
    }
    else
    {
        return (fibonacci(m - 1) + fibonacci(m - 2));
    }
}

int main()
{
 
    int n, m = 0;
    cout << "Input Fibonacci Sequences :  ";
    cin >> n;
    cout << "Fibonacci Sequences: ";
    
    for (int i = 1; i <= n; i++)
    {
        cout << fibonacci(m) << " ";
        m++;
        
    }
    cout << "\nfibonacci to-" << n << " Result: " << fibonacci(m++) << endl;
    return 0;
}
