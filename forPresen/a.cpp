#include <iostream>
 
using namespace std;
 
int main() {
  int score[2] = {};
  string str[2];
  int n;
   
  cin >> n;
   
  while(n--) {
    cin >> str[0] >> str[1];
    if(str[0] < str[1]) score[1]+=3;
    else if(str[0] == str[1]) { score[0]++; score[1]++; }
    else score[0]+=3;
  }
   
  cout << score[0] << " " << score[1] << endl;
   
  return 0;
}
