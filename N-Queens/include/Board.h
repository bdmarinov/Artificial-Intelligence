#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <ctime>
#include <vector>

class Board
{
    public:
        Board(int newN)
        {
            N = newN;
            diagonalLength = 2 * N - 1;

            queenValues = new int[N];

            rowConflicts = new int[N]();
            conflictsD1 = new int[diagonalLength]();
            conflictsD2 = new int[diagonalLength]();
            conflicts = new int[N]();

            setValues();
            calculateConflicts();
        }

        void solve()
        {
            if(hasConflicts())
            {
                int steps = 0;
                int stepsLimit = 2 * N;

                while(steps < stepsLimit)
                {
                    int maxColumn = getColumnWithMaxConflicts();
                    if(maxColumn == - 1)
                    {
                       break;
                    }

                    setNewQueenRowValues(maxColumn);

                    if(!hasConflicts())
                    {
                        std::cout<<"Solution found\n";
                        break;
                    }

                    steps++;
                    if(steps == stepsLimit && hasConflicts())
                    {
                        steps = 0;
                        nulify();
                    }
                }
            }
        }

        void print()
        {
            for(int i = 0; i < N; i++)
            {
                for(int j = 0; j < N; j++)
                {
                    if(queenValues[j] == i)
                    {
                        std::cout<<"* ";
                    }
                    else
                    {
                        std::cout<<"_ ";
                    }
                }
                std::cout<<"\n";
            }
        }

        ~Board()
        {
            delete[] queenValues;
            delete[] rowConflicts;
            delete[] conflictsD1;
            delete[] conflictsD2;
            delete[] conflicts;
        }

    private:
        int N, diagonalLength;
        int* conflictsD1, *conflictsD2, *rowConflicts, *queenValues, *conflicts;

        void setValues()
        {
            for(int i = 0; i < N; i++)
            {
                queenValues[i] = rand() % N;
            }
        }

        void setConflicts()
        {
            for(int i = 0; i < diagonalLength; i++)
            {
                if(i < N)
                {
                    rowConflicts[i] = 0;
                    conflicts[i] = 0;
                }
                conflictsD1[i] = conflictsD2[i] = 0;
            }
        }

        void calculateConflicts()
        {
            for(int i = 0; i < N; i++)
            {
                int j = queenValues[i];

                conflictsD1[N - (j - i + 1)]++;

                conflictsD2[j + i]++;

                rowConflicts[j]++;
            }
        }

        int substract(int a, int b)
        {
            return (N - (a - b + 1));
        }

        void nulify()
        {
            setValues();
            setConflicts();
            calculateConflicts();
        }

        bool hasConflicts() const
        {
            for(int i = 0; i < diagonalLength; i++)
            {
                if(i < N)
                {
                    if(rowConflicts[i] > 1)
                    {
                        return true;
                    }
                }
                if(conflictsD1[i] > 1)
                {
                    return true;
                }
                if(conflictsD2[i] > 1)
                {
                    return true;
                }
            }
            return false;
        }


        int sumConflicts(int rowConf, int confD1, int confD2)
        {
            if(rowConf > 0)
            {
                rowConf--;
            }
            if(confD1 > 0)
            {
                confD1--;
            }
            if(confD2 > 0)
            {
                confD2--;
            }
            return rowConf + confD1 + confD2;
        }

        int calculateConflictsForCol(int column)
        {
            return sumConflicts(rowConflicts[queenValues[column]], conflictsD1[substract(queenValues[column], column)], conflictsD2[queenValues[column] + column]);
        }

        int calculateConflictsFor2Cols(int column_1, int column_2)
        {
            return rowConflicts[column_1] + conflictsD1[N - (column_1 - column_2 + 1)] + conflictsD2[column_1 + column_2];
        }

        void setNewQueenRowValues(int maxColumn)
        {
            int oldRow = queenValues[maxColumn];
            int newRow = getRowWithMinConflicts(maxColumn);

            queenValues[maxColumn] = newRow;

            rowConflicts[oldRow]--;
            conflictsD1[N - (oldRow - maxColumn + 1)]--;
            conflictsD2[oldRow + maxColumn]--;

            rowConflicts[newRow]++;
            conflictsD1[N - (newRow - maxColumn + 1)]++;
            conflictsD2[newRow + maxColumn]++;
        }

        int getRowWithMinConflicts(int column)
        {
            int lowest = 2147483647;
            //int lowest = 10 * diagonalLength;

            for(int i = 0; i < N; i++)
            {
                int colConflicts = calculateConflictsFor2Cols(i, column);
                //int colConflicts = rowConflicts[i] + conflictsD1[N - (i - column + 1)] + conflictsD2[i + column];

                if(colConflicts < lowest)
                {
                    lowest = colConflicts;
                }
                conflicts[i] = colConflicts;
            }

            std::vector<int> minConflicts;

            for(int i = 0; i < N; i++)
            {
                if(conflicts[i] == lowest)
                {
                    minConflicts.push_back(i);
                }
            }
            int index = rand() % minConflicts.size();
            return minConflicts[index];
        }

        int getColumnWithMaxConflicts()
        {
            int highest = -2147483647;
            //int highest = -1;

            for(int i = 0; i < N; i++)
            {
                //int colConflicts = calculateConflictsForCol(i);

                int colConflicts = rowConflicts[queenValues[i]] + conflictsD1[N - (queenValues[i] - i + 1)] + conflictsD2[queenValues[i] + i] - 3;

                if(colConflicts > highest)
                {
                    highest = colConflicts;
                }

                conflicts[i] = colConflicts;
            }

            if(highest == 0)
            {
                return -1;
            }

            std::vector<int> maxConflicts;

            for(int i = 0; i < N; i++)
            {
                if(conflicts[i] == highest)
                {
                    maxConflicts.push_back(i);
                }
            }
            int index = rand() % maxConflicts.size();
            return maxConflicts[index];
        }
};

#endif // BOARD_H
