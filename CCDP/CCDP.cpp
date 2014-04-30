#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iomanip>
#include <cassert>
#include <cctype>
#include <sstream>
#include <map>
#include <time.h>
using namespace std;

#define MAX 1200
#define INF 20000

#define SET_VALUE 0.60
//#define SIMILAR_VALUE 0.12

const int H = 80;
//const double h = 0.3;
double h;
//double MAX_D;

typedef pair<int, int> PII;
typedef pair<PII, int> PPI;
typedef pair<double, PII> PDP;
typedef pair<PII, PII> PPII;

struct RGB{
  int r, g, b;
  RGB(int r, int g, int b) : r(r), g(g), b(b){}
};

class CCDP{
private:

  vector<double> g, Z;
  int T, x, L;
  
  double d[150+1][MAX]; // 局所距離
  double D[150+1][MAX]; // 累計距離
  double A[150+1][MAX];
  pair<int, int> from[150+1][MAX]; //(x, y)
  int original[150+1][MAX];
  vector<string> result;
  map<int, int> wall; // (何番目, 高さ)
  vector<PPII> similar; // orgのどこからどこまでが類似しているか
  vector< vector<PII> > Similar;
  
  void input(void);
  double dist(double a, double b); // 2つの文字の距離(差)を返す
  void setLength(void);
  void init_d_table(int); // 局所距離の設定
  void init_D_table(void); // 累計距離の設定
  void update_original(int); // original の更新
  void update_local_distance(void); // 局所距離の再計算(orgを使用)
  void calc_A(int l); // A の計算
  string toString(double x); // 小数点2桁に変換 (i.e 0.12 -> 12)
  void update_g_and_Z(int l);
  void CDP(void); // CDP algorithm の適用
  void toSet(void);
  void init(void);
  
  void outputd(int);
  void outputD(int);
  void outputA(int);
  void outputFROM(int);
  void outputWall(int);
  void outputPlotdata(int, int);
  void outputSimilar(int, int);
  
public:
  void run(void);
  double SIMILAR_VALUE;
  vector<RGB> org;
};


void CCDP::outputPlotdata(int s, int g){
  for(int i = s ; i < g ; i++){
    printf("%d %0.2f\n", i, org[i]);
  }
}

void CCDP::outputSimilar(int s, int g){
  if(s < g){
    for(int i = s ; i < g ; i++){
      printf("%.2f", org[i]);
      i == g-1 ? cout << endl : cout << ' ';
    }
    for(int i = s ; i < g ; i++){
      cout << i;
      i == g-1 ? cout << endl : cout << ' ';
    }
  }
  else{
    for(int i = s ; i < (int)org.size() ; i++){
      printf("%.2f ", org[i]);
    }
    for(int i = 0 ; i < g ; i++){
      printf("%.2f", org[i]);
      i == g-1 ? cout << endl : cout << ' ';
    }
    
    for(int i = s ; i < (int)org.size() ; i++){
      cout << i << ' ';
    }
    for(int i = 0 ; i < g ; i++){
      cout << i;
      i == g-1 ? cout << endl : cout << ' ';
    }

  }
}

bool isSame(int x1, int x2, int y1, int y2){
  int Min = min(x1, y1);
  int Max = max(x2, y2);  
  int div = Max - Min;
  /*
  cout << "Min = " << Min << endl;
  cout << "Max = " << Max << endl;
  
  cout << "div = " << div << endl;
  */
  //int div = max(abs(x2-x1), abs(y2-y1));
  if(div == 0) return false;
  int x = max(x1, y1);
  int y = min(x2, y2);
  int s = max(y-x, 0);
  /*
  cout << "x = " << x << endl;
  cout << "y = " << y << endl;
  cout << "s = " << s << endl;
  
  cout << "value = " << (double)s / (double)div << endl;
  */
  if( (double)s / (double)div >= SET_VALUE ) return true;
  else return false;
}

void CCDP::toSet(){
  bool used[10000];
  memset(used, false, sizeof(used));
  
  for(int i = 0 ; i < (int)similar.size() ; i++){
    if(used[i]) continue;    
    used[i] = true;
    vector<PII> tmp;

    tmp.push_back(similar[i].first);
    tmp.push_back(similar[i].second);
    
    int ax1 = similar[i].first.first;
    int ax2 = similar[i].first.second;
    int bx1 = similar[i].second.first;
    int bx2 = similar[i].second.second;    
    for(int j = 0 ; j < (int)similar.size() ; j++){
      if(used[j]) continue;
      int cx1 = similar[j].first.first;
      int cx2 = similar[j].first.second;
      int dx1 = similar[j].second.first;
      int dx2 = similar[j].second.second; 
   
      if(isSame(ax1, ax2, cx1, cx2) || isSame(ax1, ax2, dx1, dx2) ||
	 isSame(bx1, bx2, cx1, cx2) || isSame(bx1, bx2, dx1, dx2)){
	used[j] = true;
	
	if(!isSame(ax1, ax2, cx1, cx2) && !isSame(bx1, bx2, cx1, cx2)){
	  tmp.push_back(PII(cx1, cx2));
	}      
	if(!isSame(ax1, ax2, dx1, dx2) && !isSame(bx1, bx2, dx1, dx2)){
	  tmp.push_back(PII(dx1, dx2));
	}
      }
    }
    Similar.push_back(tmp);
  }

}

/****************************************************************/

void CCDP::init(){
  for(int i = 0 ; i < H+1 ; i++)
    for(int j = 0 ; j < MAX ; j++)
      d[i][j] = D[i][j] = A[i][j] = original[i][j] = 0.0;
  
  result.clear();
  wall.clear();
  similar.clear();
  Similar.clear();
  org.clear();
  g.clear();
  Z.clear();
}

double CCDP::dist(double a, double b){
  double ret = abs(a - b);
  return ret;
}    

void CCDP::setLength(){
  T = H;
  x = org.size() - H;
  L = T + x;
}

void CCDP::init_d_table(int l){
  memset(d, 0, sizeof(d));
  
  l--;
  int tmp = l+T;
  for(int i = 0 ; i < T ; i++){
    for(int j = 0 ; j < x ; j++){
      d[i][j] = dist(g[i], Z[j]);
      //cout << "g = " << g[i] << ", Z = " << Z[j] << endl;
      //cout << "j = " << j << endl;
      //cout << d[i][j] << endl;
      if(wall[(tmp+j)%L] == 0) continue;
      if(i <= wall[(tmp+j)%L]) d[i][j] = INF;
    }
  }
}

void CCDP::init_D_table(void){
  for(int i = 0 ; i < T ; i++){
    for(int j = 0 ; j < x ; j++){
      if(i == 0){
	D[i][j] = 3*d[i][j];
	//cout << "d = " << d[i][j] << ", " << "D = " << D[i][j] << endl;
	from[i][j] = pair<int, int>(j, i);
      }
      else{
	D[i][j] = INF;
	from[i][j] = pair<int, int>(-1, -1);
      }
    }
  }
}

void CCDP::update_local_distance(void){
  for(int i = 0 ; i < T ; i++){
    for(int j = 0 ; j < x; j++){
      d[i][j] = dist(g[i], Z[j]);
    }
  }
}


bool compare(PDP a, PDP b){
  if(a.first == b.first){
    return a.second.first > b.second.first;
  }
  return a.first < b.first;
}

void CCDP::update_original(int l){
  l--;
  int Min = INF;
  /*
    v は累積距離の値と、座標(y, x)を入れている
  */
  
  vector<PDP> v; // (value, (y, x))

  for(int i = T/2+T/4 ; i < T ; i++)
    for(int j = 0 ; j < x ; j++) v.push_back(PDP(D[i][j], PII(i, j)));
  sort(v.begin(), v.end(), compare);
  
  vector<PPI> INF_POINT; // ( (x1, x2), y)
  Min = v[0].first;
  for(int i = 0 ; i < (int)v.size() ; i++)
    {
      if(Min != v[i].first) break; // 最小値のみを取り出す
      
      int y = v[i].second.first;
      int x = v[i].second.second;
      if(from[y][x].first == -1) continue;
      
      int x1 = from[y][x].first;
      int x2 = x;
      
      /*
	originalを更新する場所が被ってないかの判定
      */      
      bool flag = true;
      
      for(int j = 0 ; j < (int)INF_POINT.size() ; j++)
	{
	  int x3 = INF_POINT[j].first.first;
	  int x4 = INF_POINT[j].first.second;      
	  if( (x1 <= x3 && x3 <= x2) || (x1 <= x4 && x4 <= x2) )
	    //if(x2 >= x3 && x1 <= x4)
	    {
	      flag = false;
	      break;
	    }
	}
      if(flag) INF_POINT.push_back(PPI(PII(x1, x2), y));
    }
  
  sort(INF_POINT.begin(), INF_POINT.end());  
  
  //cout << INF_POINT.size() << endl;
  for(int i = 0 ; i < (int)INF_POINT.size() ; i++)
    {
      int p1 = INF_POINT[i].first.first;
      int p2 = INF_POINT[i].first.second;
      int y = INF_POINT[i].second;
      int point1 = (H+p1+l) % L;
      int point2 = (H+p2+l) % L;      
      
      PII sim1, sim2;
      sim1.first = l;
      sim1.second = l + INF_POINT[i].second;
      
      sim2.first = (p1+H)%L;
      sim2.second = (p2+H)%L;

      int x1 = sim1.first;
      int x2 = sim1.second;
      int x3 = sim2.first;
      int x4 = sim2.second;

      if( (x1 <= x3 && x3 <= x2) || (x1 <= x4 && x4 <= x2) ) continue; 
      if(A[y][p2] < h){
	//printf("A = %.2f   h = %f\n", A[y][p2], h);
	//cout << "A = " << A[y][p2] << ", h = " << h << endl;
	//cout << "sim1 = " << sim1.first << ' ' << sim1.second << endl;
	//cout << "sim2 = " << sim2.first << ' ' << sim2.second << endl;
	//cout << endl;
	similar.push_back(PPII(sim1, sim2));
      }
      wall[point1] = max(wall[point1], INF_POINT[i].second);
      wall[point2] = max(wall[point2], INF_POINT[i].second);
    }
}

void CCDP::CDP(void){
  for(int i = 1 ; i < T ; i++){
    for(int j = 1 ; j < x ; j++){
      int Min = INF;
      int D1, D2, D3;
      D1 = D2 = D3 = INF;
      
      if(i-1 >= 0 && j-2 >= 0)
	D1 = D[i-1][j-2] + 2*d[i][j-1] + d[i][j];
      if(i-1 >= 0 && j-1 >= 0)
	D2 = D[i-1][j-1] + 3*d[i][j];
      if(i-2 >= 0 && j-1 >= 0)
	D3 = D[i-2][j-1] + 3*d[i-1][j] + 3*d[i][j];        
      
      if(D1 <= D2 && D1 <= D3 && D1 < INF){
	if(i-1 < 0 || j-2 < 0) continue;
	Min = D1;
	from[i][j] = from[i-1][j-2];
      }
      else if(D2 <= D1 && D2 <= D3 && D2 < INF){
	if(i-1 < 0 || j-1 < 0) continue;
	Min = D2;
	from[i][j] = from[i-1][j-1];
      }
      else if(D3 <= D1 && D3 <= D2 && D3 < INF){
	if(i-2 < 0 || j-1 < 0) continue;
	Min = D3;
	from[i][j] = from[i-2][j-1];
      }
      else{
	Min = INF;
	from[i][j] = pair<int, int>(-1, -1);
      }
      D[i][j] = Min;
    }
  }
}

void CCDP::calc_A(int l){
  l--;
  for(int i = 0 ; i < T ; i++){
    for(int j = 0 ; j < x ; j++){
      A[i][j] = (double)(D[i][j] / (double)(3*(i+1)));

      //printf("D = %f, div = %d, ans = %f\n", D[i][j], 3*(i+1), A[i][j]);      
    }
    //cout << endl;
  }
}

void CCDP::update_g_and_Z(int l){
  l--;
  g.clear();
  Z.clear();
  
  vector<double> tmpg, tmpZ;
  for(int i = l ; i < l+H ; i++){
    tmpg.push_back(org[i]);
  }
  
  for(int i = l+H ; i < (int)org.size() ; i++){
    tmpZ.push_back(org[i]);
  }
  
  for(int i = 0 ; i < l ; i++){
    tmpZ.push_back(org[i]);
  }
  
  g = tmpg;
  Z = tmpZ;
  
  /*
  if(l+H < L){
    for(int i = l ; i < (int)org.size() ; i++) g.push_back(org[i]);
    int size = g.size();
    for(int i = 0 ; i < H-size ; i++) g.push_back(org[i]);
    for(int i = H-size ; i < l ; i++) Z.push_back(org[i]);
  }
  else{
    for(int i = l ; i < l+H ; i++) g.push_back(org[i]);
    for(int i = l+H ; i < (int)org.size() ; i++) Z.push_back(org[i]);
    for(int i = 0 ; i < l ; i++) Z.push_back(org[i]);
  }
  */
}

void CCDP::input(){
  double d;
  h = 0;
  
  for(int i = 0 ; i < org.size() ; i++){
    h += org[i];
  }

  h /= org.size();
  h *= SIMILAR_VALUE;
  
  
  g.resize(H);
  //cout << org.size() << endl;
  //cout << H << endl;
  
  Z.resize((int)org.size()-H);
  
  for(int i = 0 ; i < H ; i++) g[i] = org[i];
  for(int i = H ; i < (int)org.size() ; i++) Z[i-H] = org[i];
  
  /*
  cout << "ZZZZZ" << endl;
  for(int i = 0 ; i < Z.size() ; i++){
    cout << i << " " << Z[i] << endl;
  }
  cout << "zzzzz" << endl;
  */
}

void CCDP::run(void){

  input();

  //cout << "h = " << h << endl;
  setLength();
  //cout << "L = " << L << endl;
  
  /*
    Time complexity
    L * 
    
    If that 
    
   */
  
  for(int l = 1 ; l <= L-H ; l++){
    init_d_table(l);
    init_D_table();    
    CDP();
    
    calc_A(l);
    
    update_original(l);
    //outputWall(l);
    update_g_and_Z(l);
      
  }

  toSet();
  
  for(int i = 0 ; i < (int)Similar.size() ; i++){
    for(int j = 0 ; j < (int)Similar[i].size() ; j++){
      for(int k = j+1 ; k < (int)Similar[i].size() ; k++){
	if(isSame(Similar[i][j].first, Similar[i][j].second,
		  Similar[i][k].first, Similar[i][k].second)){
	  Similar[i].erase(Similar[i].begin()+k);
	  j = 0;
	}
      }
    }
  }
  
  for(int i = 0 ; i < (int)Similar.size() ; i++){
    sort(Similar[i].begin(), Similar[i].end());
  }
  
  cout << SIMILAR_VALUE << ' ' << Similar.size() << endl;
  //cout << "Similar size = " << Similar.size() << endl;
  
  /*
  for(int i = 0 ; i < Similar.size() ; i++){    
    for(int j = 0 ; j < Similar[i].size() ; j++){      
      //outputPlotdata(Similar[i][j].first.first, Similar[i][j].first.second);
      cout << Similar[i][j].first << ' ' << Similar[i][j].second << endl;
      if(j != Similar[i].size()-1) cout << "@" << endl;
      //outputPlotdata(Similar[i][j].second.first, Similar[i][j].second.second);
    }
    cout << "-------------" << " = " << i << " = " << endl;
    //cout << "#" << endl;
  }
  */
}

int main(){

  //clock_t c1, c2;
  //c1 = clock();
  
  vector<RGB> org;
  int r, g, b;
  while(cin >> r >> g >> b){
    org.push_back(RGB(r, g, b));
  }
  
  cout << "org size = " << org.size() << endl;

  for(double SIMILAR_VALUE = 1.00 ; SIMILAR_VALUE <= 1.00 ; SIMILAR_VALUE += 0.01){
    CCDP ccdp;
    ccdp.org = org;
    ccdp.SIMILAR_VALUE = SIMILAR_VALUE;
    ccdp.run();
  }
  //c2 = clock();
  //printf("time = %f[s]\n", (double)(c2-c1)/CLOCKS_PER_SEC);
  
  return 0;
}
