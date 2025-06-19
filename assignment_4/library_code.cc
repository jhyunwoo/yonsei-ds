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

/*****************************************************************
 * Question 3 – Word Transformation Sequence                *
 *****************************************************************/
void question3() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string beginWord, endWord; int N;
  if (!(cin >> beginWord >> endWord >> N)) return;

  vector<string> dict(N);
  for (auto &w : dict) cin >> w;

  if (find(dict.begin(), dict.end(), endWord) == dict.end()) {
    cout << 0 << "\n"; return; }
  if (find(dict.begin(), dict.end(), beginWord) == dict.end())
    dict.push_back(beginWord);

  const int L = beginWord.size();
  const int total = (int)dict.size();

  unordered_map<string, vector<int>> bucket; bucket.reserve(total * L);
  auto pat = [&](const string &s, int p) { string t = s; t[p] = '*'; return t; };
  for (int i = 0; i < total; ++i)
    for (int p = 0; p < L; ++p)
      bucket[pat(dict[i], p)].push_back(i);

  int S = int(find(dict.begin(), dict.end(), beginWord) - dict.begin());
  int T = int(find(dict.begin(), dict.end(), endWord)  - dict.begin());

  queue<int> q; vector<int> dist(total, -1); dist[S] = 1; q.push(S);
  while (!q.empty()) {
    int u = q.front(); q.pop();
    if (u == T) break;
    const string &w = dict[u];
    for (int p = 0; p < L; ++p) {
      auto &vec = bucket[pat(w, p)];
      for (int v : vec) if (dist[v] == -1) { dist[v] = dist[u] + 1; q.push(v);}
      vec.clear();
    }
  }
  cout << (dist[T] == -1 ? 0 : dist[T]) << "\n";
}

/*****************************************************************
 * Question 4 – Minimum Cost to Reach City With Discounts     *
 *****************************************************************/
void question4() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

  int N, M; if (!(cin >> N >> M)) return;
  vector<vector<pair<int,int>>> g(N);
  for (int i = 0; i < M; ++i) { int u,v,c; cin>>u>>v>>c; g[u].push_back({v,c}); g[v].push_back({u,c}); }
  int D; cin >> D;
  const long long INF = 4e18;
  vector<vector<long long>> dist(N, vector<long long>(D+1, INF));
  using S=tuple<long long,int,int>; priority_queue<S,vector<S>,greater<S>> pq;
  dist[0][0]=0; pq.emplace(0LL,0,0);
  while(!pq.empty()) {
    auto [w,u,k]=pq.top(); pq.pop(); if(w!=dist[u][k]) continue;
    for(auto [v,c]:g[u]) {
      if(w+c<dist[v][k]) { dist[v][k]=w+c; pq.emplace(dist[v][k],v,k);}
      if(k<D && w+c/2<dist[v][k+1]) { dist[v][k+1]=w+c/2; pq.emplace(dist[v][k+1],v,k+1);} }
  }
  long long ans=*min_element(dist[N-1].begin(), dist[N-1].end());
  cout<<(ans==INF?-1:ans)<<"\n";
}

/*****************************************************************
 * Question 5 – Valve Control (patched)                       *
 *  – previous implementation missed optimal paths in certain
 *    topologies (e.g. pure star). Re‑implemented with:
 *      1. BFS pre‑computation of shortest paths between
 *         important valves (+AA)
 *      2. DFS + memo where each move consumes (dist+1) minutes
 *****************************************************************/
void question5() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

  int N; if(!(cin>>N)) return;
  struct Node { string name; int flow; vector<string> neigh; };
  vector<Node> nodes(N);
  unordered_map<string,int> id;
  for(int i=0;i<N;++i) {
    int k; cin>>nodes[i].name>>nodes[i].flow>>k; nodes[i].neigh.resize(k); id[nodes[i].name]=i;
    for(int j=0;j<k;++j) cin>>nodes[i].neigh[j];
  }
  // Build unweighted adjacency (bidirectional guaranteed)
  vector<vector<int>> adj(N);
  for(int i=0;i<N;++i) for(auto &s:nodes[i].neigh) adj[i].push_back(id[s]);

  // List of valves with positive flow + start AA
  int start = id["AA"];
  vector<int> important;        // first element will be AA
  important.push_back(start);
  for(int i=0;i<N;++i) if(nodes[i].flow>0) important.push_back(i);
  const int K = (int)important.size()-1; // positive‑flow count (≤15)

  // BFS from each important valve to compute pairwise shortest distances
  const int M = (int)important.size();
  vector<vector<int>> distMat(M, vector<int>(M, INT_MAX/2));
  auto bfs=[&](int srcIdx){
    int src=important[srcIdx];
    vector<int>d(N,INT_MAX/2); d[src]=0; queue<int>q; q.push(src);
    while(!q.empty()){int u=q.front();q.pop(); for(int v:adj[u]) if(d[v]==INT_MAX/2){d[v]=d[u]+1; q.push(v);} }
    for(int j=0;j<M;++j) distMat[srcIdx][j]=d[important[j]]; };
  for(int i=0;i<M;++i) bfs(i);

  // Memoization: key = (posIdx, mask, time)
  unordered_map<uint64_t,int> memo; memo.reserve(1<<22);
  auto keyPack=[&](int pos,int mask,int t){ return (uint64_t)pos | (uint64_t)mask<<6 | (uint64_t)t<<21;};

  function<int(int,int,int)> dfs = [&](int pos,int mask,int time) -> int {
    if (time <= 0) return 0;
    uint64_t key = keyPack(pos, mask, time);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    int best = 0;

    // 1) 현재 밸브 열기 (pos==0 은 AA 이므로 제외)
    if (pos != 0) {
      int bit = pos - 1; // AA 제외한 위치를 0번 비트부터 매핑
      if (!(mask & (1 << bit)) && time > 1) {
        int gain = (time - 1) * nodes[important[pos]].flow;
        best = max(best, gain + dfs(pos, mask | (1 << bit), time - 1));
      }
    }

    // 2) 다른 밸브로 이동
    for (int nxt = 1; nxt < M; ++nxt) {
      if (nxt == pos) continue;              // 자기 자신은 건너뜀
      int bit = nxt - 1;
      if (mask & (1 << bit)) continue;       // 이미 연 밸브 skip
      int d = distMat[pos][nxt];             // 이동 거리 (분)
      if (d >= time || d == INT_MAX / 2) continue; // 남은 시간 부족
      best = max(best, dfs(nxt, mask, time - d));  // 이동만 하고 개방은 다음 루프에서
    }
    return memo[key] = best;
  };

  cout<<dfs(0,0,30)<<"\n";
}
