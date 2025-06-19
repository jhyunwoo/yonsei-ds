// library_code.cc

#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;


// Question 3 - Word Transformation Sequence
void question3() {
  ios::sync_with_stdio(false);
cin.tie(nullptr);

  string beginWord, endWord;
  int N;
  if(!(cin>>beginWord>>endWord>>N)) return;

  vector<string> dict(N);
  for(auto &w:dict) {
    cin>>w;

  }

  if(find(dict.begin(),dict.end(),endWord)==dict.end()){
    cout<<0<<"\n";
    return;
  }

  if(find(dict.begin(),dict.end(),beginWord)==dict.end()) {
    dict.push_back(beginWord);
  }

  const int L=beginWord.size(), total=dict.size();

  unordered_map<string,vector<int>> bucket;
  bucket.reserve(total*L);

  auto pat=[&](const string&s,int p){
    string t=s;
    t[p]='*';
    return t;
  };
  for(int i=0;i<total;++i) {
    for(int p=0;p<L;++p) {
      bucket[pat(dict[i],p)].push_back(i);
    }
  }

  int S=find(dict.begin(),dict.end(),beginWord)-dict.begin();

  int T=find(dict.begin(),dict.end(),endWord)-dict.begin();

  queue<int> q;
vector<int> dist(total,-1);
dist[S]=1;
q.push(S);

  while(!q.empty()){
    int u=q.front();
    q.pop();
    if(u==T) break;
    const string &w=dict[u];
    for(int p=0;p<L;++p){
      string key=pat(w,p);
      auto &vec=bucket[key];
      for(int v:vec) if(dist[v]==-1){
        dist[v]=dist[u]+1;
        q.push(v);
      }
      vec.clear();
    }
  }
  cout<<(dist[T]==-1?0:dist[T])<<"\n";
  return;
}


// Question 4 - Minimum Cost to Reach City With Discounts
void question4() {
  ios::sync_with_stdio(false); c
in.tie(nullptr);

  int N, M;
if (!(cin >> N >> M)) return;

  vector<vector<pair<int,int>>> g(N);

  for (int i = 0; i < M; ++i) {
    int u,v,c; cin >> u >> v >> c;
    g[u].push_back({v,c});
    g[v].push_back({u,c});
  }
  int D; cin >> D;

  const long long INF = 4e18;
  vector<vector<long long>> dist(N, vector<long long>(D+1, INF));

  using State = tuple<long long,int,int>; // cost, node, used
  priority_queue<State, vector<State>, greater<State>> pq;
  dist[0][0] = 0;
pq.emplace(0LL,0,0);

  while(!pq.empty()) {
    auto [cost,u,k] = pq.top();
pq.pop();
    if (cost != dist[u][k]) continue;
    for (auto [v,w] : g[u]) {
      // No discount
      if (cost + w < dist[v][k]) {
        dist[v][k] = cost + w;
        pq.emplace(dist[v][k], v, k);
      }
      // With discount
      if (k < D) {
        long long nc = cost + w / 2;
        if (nc < dist[v][k+1]) {
          dist[v][k+1] = nc;
          pq.emplace(nc, v, k+1);
        }
      }
    }
  }
  long long ans = *min_element(dist[N-1].begin(), dist[N-1].end());
  cout << (ans==INF ? -1 : ans) << '\n';
}


// Question 5 - Valve Control: Efficient Pressure Release Scheduling
void question5() {
  int N;
if(!(cin>>N)) return;
struct Info{
string name;
int flow;
vector<string> adj;
};
  vector<Info> v(N);
unordered_map<string,int> idx;
  for(int i=0;i<N;++i){
int k;
cin>>v[i].name>>v[i].flow>>k;
v[i].adj.resize(k);
idx[v[i].name]=i;
for(int j=0;j<k;++j) {
cin>>v[i].adj[j];
}
}
  vector<vector<int>> adj(N);
for(int i=0;i<N;++i) {
for(string &s:v[i].adj) {
adj[i].push_back(idx[s]);
}
}
  const int INF=1e9;
vector<vector<int>> d(N,vector<int>(N,INF));
  for(int i=0;i<N;++i){
d[i][i]=0;
for(int j:adj[i]) {
d[i][j]=1;
}
}
for(int k=0;k<N;++k) {
for(int i=0;i<N;++i) {
for(int j=0;j<N;++j) {
d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
}
}
}
  vector<int> imp;
for(int i=0;i<N;++i){
 if(v[i].flow>0) {
imp.push_back(i);
}
}
const int K=imp.size();
int start=idx["AA"];
  unordered_map<int,int> bitPos;
for(int i=0;i<K;++i) {
bitPos[imp[i]]=i;
}

  // key: |63..21| mask(15) |20..16|time(5) |15..9| node(7)  (full 64‑bit no overlap)
  auto pack=[&](int mask,int t,int u){
return (uint64_t(mask)<<16)|(t<<9)|u;
};
  unordered_map<uint64_t,int> memo;
memo.reserve(1<<21);
  function<int(int,int,int)> dfs=[&](int u,int t,int mask){
uint64_t key=pack(mask,t,u);
auto it=memo.find(key);
if(it!=memo.end()) {
return it->second;
}
int best=0;
    // option 1: open current valve (including AA if flow>0)
    if(v[u].flow>0){
int b=bitPos[u];
if(!(mask&(1<<b)) && t>1){
int gain=(t-1)*v[u].flow;
best=max(best,gain+dfs(u,t-1,mask|(1<<b)));
}
}
    // option 2: move to another important valve shut so far
    // traverse to unopened important valves (excluding current)
    for(int x:imp){
      if(x==u) continue;                          // avoid 0‑distance self‑move
      int b = bitPos[x]; if(mask & (1<<b)) continue;  // already open
      int dist = d[u][x]; if(dist==INF || dist>=t) continue;
      best = max(best, dfs(x, t - dist, mask));
    }
    return memo[key]=best;
};
  cout<<dfs(start,30,0)<<"\n";
}
