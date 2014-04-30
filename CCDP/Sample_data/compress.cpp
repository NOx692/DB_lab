#include <bits/stdc++.h>
using namespace std;

const int C = 30; // C is after compressed length
const int MAX = 1000;

struct RGB{
  int r, g, b;
  RGB(int r, int g, int b) : r(r), g(g), b(b){}
};

int H, W;
int L;
vector<RGB> V[MAX];

void input(){
  cin >> H >> W;
  L = H * W;
  int r, g, b;
  for(int i = 0 ; i < H ; i++){
    for(int j = 0 ; j < W ; j++){
      scanf("%d %d %d", &r, &g, &b);
      V[i].push_back(RGB(r, g, b));
    }
  }
  cout << H << ' ' << W << endl;
}

void compress(){
  int CH, CW; // one rectangle length
  CH = H / C + (H % C != 0);
  CW = W / C + (W % C != 0);
  
  cout << CH << ' ' << CW << endl;
  
  vector<RGB> res;

  for(int i = 0 ; i < H - CH ; i += CH){
    for(int j = 0 ; j < W - CW ; j += CW){
      RGB ave(0, 0, 0);
      for(int a = i ; a < i + CH ; a++){
	for(int b = j ; b < j + CW ; b++){
	  cout << a << ' ' << b << endl;	  
	  ave.r += V[a][b].r;
	  ave.g += V[a][b].g;
	  ave.b += V[a][b].b;
	}
      }
      cout << endl;
      ave.r /= (CH * CW);
      ave.g /= (CH * CW);
      ave.b /= (CH * CW);
      res.push_back(ave);
    }
  }  
  
  cout << (int)res.size() << endl;
  
  for(int i = 0 ; i < (int)res.size() ; i++){
    cout << res[i].r << ' ' << res[i].g << ' ' << res[i].b << endl;
  }
}

int main(){
  input();
  compress();
  
  return 0;
}
