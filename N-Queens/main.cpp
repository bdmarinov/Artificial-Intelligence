#include <iostream>
#include <cstdlib>
#include "Board.h"

using namespace std;

int main()
{
    int n;
    cin >> n;
    if (n < 4)
    {
        cout << "No solution exists for n < 4" << endl;
        return 0;
    }
    else
    {
        clock_t start = clock();

        srand(time(NULL));
        Board board(n);
        board.solve();

        clock_t end = clock();

        if (n <= 25) {
            board.print();
        }
        cout << "Time to find solution: " << (end - start) / (double) CLOCKS_PER_SEC << endl;
    }

    return 0;
}
