#include <bits/stdc++.h>
using namespace std;

const int C = 30; // C is after compressed length

struct RGB{
  int r, g, b;
  RGB(int r, int g, int b) : r(r), g(g), b(b){}
};

int H, W;
int L;
vector<RGB> V;

void input(){
  cin >> H >> W;
  L = H * W;
  int r, g, b;
  for(int i = 0 ; i < L ; i++){
    scanf("%d %d %d", &r, &g, &b);
    V.push_back(RGB(r, g, b));
  }
  cout << H << ' ' << W << endl;
}

void compress(){
  int CH, CW; // one rectangle length
  CH = H / C + (H % C != 0);
  CW = W / C + (W % C != 0);
  
  cout << CH << ' ' << CW << endl;
}

int main(){
  input();
  compress();
  
  return 0;
}
