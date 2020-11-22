#include <iostream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <set>
#include <cmath>

using namespace std;

const int chanceOfMutation = 20;

int getRandom(int number)
{
	return rand() % number;
}

int distanceBetweenPoints(pair<int, int> & a, pair<int, int> & b)
{
	int a_X = a.first, a_Y = a.second;
	int b_X = b.first, b_Y = b.second;

	return sqrt(pow(abs(a_X - b_X), 2) + pow(abs(a_Y - b_Y), 2));
}

int getFittest(vector <pair<int, int>> & route)
{
	int fittest = 0;

	for(int i = 0; i < route.size() - 1; i++)
    {
		fittest += distanceBetweenPoints(route.at(i), route.at(i + 1));
	}
	return fittest;
}

bool compareRoutes(vector<pair<int, int>> & route_A, vector <pair<int, int>> & route_B)
{
	return (getFittest(route_A) < getFittest(route_B));
}

vector<pair<int, int>> crossover(vector<pair<int, int>> & parent1, vector<pair<int, int>> & parent2)
{
	int size = parent1.size();
	int j = getRandom(size - 1) + 1;
	int i = getRandom(j);

	vector<pair<int, int>> child(size, pair<int, int>());
	set<pair<int, int>> visited;

	for(int k = i; k <= j; k++)
    {
		pair<int, int> temp = parent1.at(k);
		visited.insert(temp);
		child.at(k) = temp;
	}

	int curPosition = j + 1, k = j + 1;

	while(visited.size() != size)
    {
		if(k == size)
        {
			k = 0;
        }

		if(curPosition == size)
        {
			curPosition = 0;
        }

		pair<int, int> temp = parent2.at(k);

		if(visited.find(temp) == visited.end())
        {
			visited.insert(temp);
			child.at(curPosition) = temp;
			curPosition++;
		}

		k++;
	}

	return child;
}

void mutate(vector<pair<int, int>> & child)
{
	int firstIndex = getRandom(child.size());
	int secondIndex = getRandom(child.size());

	if(firstIndex != secondIndex)
    {
		pair<int, int> temp = child.at(firstIndex);
		child.at(firstIndex) = child.at(secondIndex);
		child.at(secondIndex) = temp;
	}
}

void evolve(vector<vector <pair<int, int>>> & curPopulation)
{
	sort(curPopulation.begin(), curPopulation.end(), compareRoutes);

	int counter = curPopulation.size() / 2;

	for(int i = 0; i < counter; i++)
    {
		curPopulation.pop_back();
    }
}

int main()
{
	int N;
	cin >> N;

	srand(time(nullptr));

	vector <pair<int, int>> cities;
	vector<vector <pair<int, int>>> population;

	for(int i = 0; i < N; i++)
    {
		int x = getRandom(100 * 2) - 100;
		int y = getRandom(100 * 2) - 100;
		cities.push_back(make_pair(x, y));
	}

	for(int i = 0; i < N; i++)
    {
		random_shuffle(cities.begin(), cities.end());
		population.push_back(cities);
	}

	int counter = 0;
	sort(population.begin(), population.end(), compareRoutes);

	while(counter < N * 2)
    {
        vector <pair<int, int>> route = *population.begin();

		cout<< getFittest(route)<<"\n";

		int candidatesNumber = 0.3 * N + 2;

		for(int i = 0; i < N; i++)
        {
			int index1 = getRandom(candidatesNumber);
			int index2 = getRandom(candidatesNumber);

			while(index2 == index1)
            {
				index2 = getRandom(candidatesNumber);
            }

			vector <pair<int, int>> chromosome1 = population.at(index1);
			vector <pair<int, int>> chromosome2 = population.at(index2);
			vector <pair<int, int>> child = crossover(chromosome1, chromosome2);

			int newChanceOfMutation = getRandom(100);

			if(newChanceOfMutation <= chanceOfMutation)
			{
				mutate(child);
			}

			population.push_back(child);
		}

		evolve(population);
		counter++;
	}
    return 0;
}
