#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> P;
typedef long double ld;

void display(map<string, P> mp){
  for(map<string, P>::iterator it = mp.begin() ; it != mp.end() ; it++){
    printf("%3s %5d %5d\n", it->first.c_str(), it->second.first, it->second.second);
  }
}

int main(int argc, char *argv[]){
  ifstream fl1(argv[1]);
  ifstream fl2(argv[2]);

  string tmp;
  string s, t;
  s = t = "";
  while(fl1 >> tmp) s += tmp;
  while(fl2 >> tmp) t += tmp;
  
  map<string, P> mp;
  for(int i = 0 ; i < (int)s.size()-3 ; i++) mp[s.substr(i, 3)].first++;
  for(int i = 0 ; i < (int)t.size()-3 ; i++) mp[t.substr(i, 3)].second++;
  
  display(mp);
  
  int a, b, c;
  a = b = c = 0;
  
  for(map<string, P>::iterator it = mp.begin() ; it != mp.end() ; it++){
    int m1 = it->second.first;
    int m2 = it->second.second;
    
    a += m1 * m1;
    b += m2 * m2;
    c += m1 * m2;
  }
  
  ld ans = (ld)(c) / (sqrt(ld(a)) * sqrt(ld(b)));
  printf("%.2Lf\n", ans);
  
  return 0;
}
