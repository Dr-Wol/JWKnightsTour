#include <iostream>
#include <iomanip>
#include <array>
#include <string>
#include <tuple>
#include <algorithm>
#include <Windows.h>
using namespace std;

//
//	The Knights Tour
//	By Jan-Willem Jozic 500623980
//



template<int N>
class Tour
{
public:
	array<pair<int, int>, 8> moves;
	array<array<int, N>, N> data;

	// The movement types the Knight can make
	Tour()
	{
		moves[0] = make_pair(2, 1);
		moves[1] = make_pair(1, 2);
		moves[2] = make_pair(-1, 2);
		moves[3] = make_pair(-2, 1);
		moves[4] = make_pair(-2, -1);
		moves[5] = make_pair(-1, -2);
		moves[6] = make_pair(1, -2);
		moves[7] = make_pair(2, -1);
	}

	//
	array<int, 8> sortMoves(int x, int y) //const
	{
		array<tuple<int, int>, 8> counts;
		for (int i = 0; i < 8; ++i)
		{
			int dx = get<0>(moves[i]);
			int dy = get<1>(moves[i]);

			int c = 0;
			for (int j = 0; j < 8; ++j)
			{
				int x2 = x + dx + get<0>(moves[j]);
				int y2 = y + dy + get<1>(moves[j]);

				if (x2 < 0 || x2 >= N || y2 < 0 || y2 >= N)
					continue;
				if (data[y2][x2] != 0)
					continue;

				c++;
			}

			counts[i] = make_tuple(c, i);
		}

		// Shuffle to randomly break ties
		random_shuffle(counts.begin(), counts.end());

		// Lexicographic sort
		sort(counts.begin(), counts.end());

		array<int, 8> out;
		for (int i = 0; i < 8; ++i)
			out[i] = get<1>(counts[i]);
		return out;
	}


	// The Function that tests the next move and compensates if the next move wont work.
	void solve(string start)
	{
		
		for (int v = 0; v < N; ++v)
		for (int u = 0; u < N; ++u)
			data[v][u] = 0;

		int x0 = start[0] - 'a';
		int y0 = start[1] - '1';
		data[y0][x0] = 1;

		array<tuple<int, int, int, array<int, 8>>, N*N> order;
		order[0] = make_tuple(x0, y0, 0, sortMoves(x0, y0));

		int n = 0;
		while (n < N*N - 1)
		{
			int x = get<0>(order[n]);
			int y = get<1>(order[n]);

			bool ok = false;
			for (int i = get<2>(order[n]); i < 8; ++i)
			{
				int dx = moves[get<3>(order[n])[i]].first;
				int dy = moves[get<3>(order[n])[i]].second;

				if (x + dx < 0 || x + dx >= N || y + dy < 0 || y + dy >= N)
					continue;
				if (data[y + dy][x + dx] != 0)
					continue;

				++n;
				get<2>(order[n]) = i + 1;
				data[y + dy][x + dx] = n + 1;
				order[n] = make_tuple(x + dx, y + dy, 0, sortMoves(x + dx, y + dy));
				ok = true;
				break;
			}

			if (!ok) // Failed. Backtrack.
			{
				data[y][x] = 0;
				--n;
			}
		}
	}

	template<int N>
	friend ostream& operator<<(ostream &out, const Tour<N> &b);
};

// 
template<int N>
ostream& operator<<(ostream &out, const Tour<N> &b)
{
	for (int v = 0; v < N; ++v)
	{
		for (int u = 0; u < N; ++u)
		{
			if (u != 0) out << ",";
			{
				out << "[";
				out << setw(3) << b.data[v][u];
				out << "]";
			}
		}
		out << endl;
	}
	return out;
}

//
// The Main function
// The number in Tour<N> determines the size of the board 
// The letter and number in the string solve("wh") is for the starting point the Width and Height
// To ensure the program doesn't automatically shut the command prompt the do/while loop demands you press enter before it will
//

int main(int argc, char **argv)
{
	string location;
	cout << '\n' << "Welcome to the Knights Tour, " << '\n';
	Sleep(1000);
	
	cout << '\n' << "The board size will be " << 8 << " by " << 8 << " Now, select a starting position." << '\n' << "The starting position is defined like a real chessboard" << '\n';
	Sleep(500);
	cout << "Assign the start location by its starting Width," << '\n' << "going a or b or c up to h" << '\n' << '\n';
	Sleep(500);
	cout << "Then follow this up by deciding the Height" << '\n' << "Using a number between 1 and 8" << '\n' << '\n';
	Sleep(500);
	cout << "The end result should be something like: \'a1\' or \'c4\'" << '\n' << '\n'
		<< "After this, press Enter to let the knight run his tour" << '\n';
	cout << "Remember, you start from the top left," << '\n' << "a-h is for the Horizontal position," << '\n' << "1-8 is Vertical" << '\n' << '\n'; 

	cin >> location;

	

	Tour<8> board;
	board.solve(location);
	cout << board << endl;

	cout << '\n' << "The knights tour is over. Press the ESC button to exit the program";

	do {
		
	} while (GetAsyncKeyState(VK_ESCAPE) == 0);

	return 0;
}