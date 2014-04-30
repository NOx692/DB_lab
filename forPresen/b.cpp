#include <iostream>
#include <string>
using namespace std;
 
int main()
{
  int n;
  cin >> n;
  int score[2] = { 0, 0 };
  for (int i=0; i<n; i++)
  {
    string cards[2];
    cin >> cards[0] >> cards[1];
    if ( cards[0] == cards[1] )
    {
      score[0]++;
      score[1]++;
    }
    else if ( cards[0] > cards[1] )
    {
      score[0] += 3;
    }
    else
    {
      score[1] += 3;
    }
  }
  cout << score[0] << ' ' << score[1] << endl;
}
