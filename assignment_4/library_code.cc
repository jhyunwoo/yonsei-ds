#include <algorithm>            // <algorithm> : 정렬 및 다양한 알고리즘 함수 제공
#include <array>                 // <array> : 고정 길이 배열 컨테이너
#include <bitset>                // <bitset> : 비트 연산용 컨테이너
#include <cmath>                 // <cmath> : 수학 함수 (pow, sqrt 등)
#include <cstdint>               // <cstdint> : 고정 폭 정수형 (int64_t 등)
#include <functional>            // <functional> : 함수 객체 및 바인더
#include <iostream>              // <iostream> : 입출력 스트림 (cin, cout)
#include <map>                   // <map> : 균형 이진 탐색 트리 기반 연관 컨테이너
#include <numeric>               // <numeric> : 누적 합, inner_product 등
#include <queue>                 // <queue> : 큐/우선순위 큐 컨테이너 어댑터
#include <set>                   // <set> : 균형 BST 기반 집합 컨테이너
#include <stack>                 // <stack> : 스택 컨테이너 어댑터
#include <string>                // <string> : 문자열 클래스
#include <tuple>                 // <tuple> : 튜플 자료형
#include <unordered_map>         // <unordered_map> : 해시 기반 연관 컨테이너
#include <unordered_set>         // <unordered_set> : 해시 기반 집합 컨테이너
#include <utility>               // <utility> : pair, move 등 유틸리티
#include <vector>                // <vector> : 가변 길이 배열 컨테이너
using namespace std;             // std 네임스페이스를 전역으로 사용

/*****************************************************************
 * Question 3 – Word Transformation Sequence                    *
 *****************************************************************/
void question3() {
  ios::sync_with_stdio(false);    // C I/O 와 C++ I/O 동기화 해제 (속도 향상)
  cin.tie(nullptr);               // cin 과 cout의 tie 해제 (속도 향상)

  string beginWord, endWord; int N;               // 시작 단어, 끝 단어, 사전 크기
  if (!(cin >> beginWord >> endWord >> N)) return; // 입력 실패 시 함수 종료

  vector<string> dict(N);                         // 사전 단어들을 저장할 벡터
  for (auto &w : dict) cin >> w;                  // N개의 단어 입력

  // endWord 가 사전에 없다면 변환 불가 → 0 출력 후 종료
  if (find(dict.begin(), dict.end(), endWord) == dict.end()) {
    cout << 0 << "\n"; return; }
  // beginWord 가 사전에 없다면 변환 출발점을 위해 삽입
  if (find(dict.begin(), dict.end(), beginWord) == dict.end())
    dict.push_back(beginWord);

  const int L = beginWord.size();                 // 모든 단어의 길이 (문제 가정)
  const int total = (int)dict.size();             // 사전 총 단어 수 (삽입 후)

  unordered_map<string, vector<int>> bucket;      // 패턴("*" 치환) ↦ 단어 인덱스 목록
  bucket.reserve(total * L);                      // 리해싱 최소화 위해 예약
  auto pat = [&](const string &s, int p) {        // 인덱스 p 글자를 '*' 로 치환하여 패턴 생성
    string t = s; t[p] = '*'; return t; };
  // 모든 단어에 대해 각 위치를 '*'로 치환한 패턴을 버킷에 삽입
  for (int i = 0; i < total; ++i)
    for (int p = 0; p < L; ++p)
      bucket[pat(dict[i], p)].push_back(i);

  int S = int(find(dict.begin(), dict.end(), beginWord) - dict.begin()); // 시작 인덱스
  int T = int(find(dict.begin(), dict.end(), endWord)  - dict.begin());  // 목표 인덱스

  queue<int> q;                       // BFS 큐
  vector<int> dist(total, -1);        // 방문 여부 및 변환 횟수 기록 (-1 : 미방문)
  dist[S] = 1;                        // 시작 단어는 길이 1 단계로 간주 (문제 조건)
  q.push(S);                          // 큐에 시작 인덱스 삽입

  while (!q.empty()) {                // BFS 수행
    int u = q.front(); q.pop();       // 현재 노드 꺼내기
    if (u == T) break;                // 목표 단어 도달 시 종료
    const string &w = dict[u];        // 현재 단어 참조
    for (int p = 0; p < L; ++p) {     // 매 위치별로 '*'-패턴 탐색
      auto &vec = bucket[pat(w, p)];  // 같은 패턴을 공유하는 인접 단어 인덱스 리스트
      for (int v : vec)
        if (dist[v] == -1) {          // 아직 방문하지 않은 단어라면
          dist[v] = dist[u] + 1;      // 거리 갱신 (변환 횟수 +1)
          q.push(v);                  // 큐에 삽입하여 이후 탐색
        }
      vec.clear();                    // 중복 확장을 막기 위해 리스트 비움 (간선 1회만 사용)
    }
  }
  cout << (dist[T] == -1 ? 0 : dist[T]) << "\n"; // 결과 출력 (불가 시 0)
}

/*****************************************************************
 * Question 4 – Minimum Cost to Reach City With Discounts       *
 *****************************************************************/
void question4() {
  ios::sync_with_stdio(false); cin.tie(nullptr); // 입출력 최적화

  int N, M; if (!(cin >> N >> M)) return;        // 도시 수 N, 도로 수 M 입력
  vector<vector<pair<int,int>>> g(N);            // 인접 리스트: (이웃, 비용)
  for (int i = 0; i < M; ++i) {                  // M 개의 도로 정보 입력
    int u,v,c; cin>>u>>v>>c;                     // 양끝 도시 u,v 와 비용 c
    g[u].push_back({v,c});                       // 무방향 그래프 → 양쪽에 간선 삽입
    g[v].push_back({u,c}); }
  int D; cin >> D;                               // 할인 쿠폰 최대 사용 횟수 D 입력

  const long long INF = 4e18;                    // 충분히 큰 값 → 무한대 대체
  vector<vector<long long>> dist(N, vector<long long>(D+1, INF)); // dist[도시][쿠폰 사용 횟수]
  using S=tuple<long long,int,int>;              // (총비용, 현재도시, 사용한쿠폰)
  priority_queue<S,vector<S>,greater<S>> pq;     // 최소 힙 다익스트라
  dist[0][0]=0; pq.emplace(0LL,0,0);             // 시작 도시 0, 쿠폰 0개 사용, 비용 0
  while(!pq.empty()) {
    auto [w,u,k]=pq.top(); pq.pop();             // 최저 비용 상태 추출
    if(w!=dist[u][k]) continue;                  // 이미 더 나은 경로가 있으면 skip
    for(auto [v,c]:g[u]) {                       // 모든 인접 도시 탐색
      // 1) 쿠폰 사용 안 함
      if(w+c<dist[v][k]) {                       // 더 짧은 경로 발견 시 갱신
        dist[v][k]=w+c; pq.emplace(dist[v][k],v,k);}
      // 2) 쿠폰 사용 (비용 절반, 정수 나눗셈으로 문제 정의에 따라 c/2)
      if(k<D && w+c/2<dist[v][k+1]) {            // 쿠폰 남아 있고 더 낫다면 갱신
        dist[v][k+1]=w+c/2; pq.emplace(dist[v][k+1],v,k+1);} }
  }
  long long ans=*min_element(dist[N-1].begin(), dist[N-1].end()); // 도착 도시 N-1 최솟값
  cout<<(ans==INF?-1:ans)<<"\n";                   // 도달 불가 시 -1, 가능 시 비용 출력
}

/*****************************************************************
 * Question 5 – Valve Control (patched)                         *
 *  – previous implementation missed optimal paths in certain    *
 *    topologies (e.g. pure star). Re-implemented with:          *
 *      1. BFS pre-computation of shortest paths between         *
 *         important valves (+AA)                                *
 *      2. DFS + memo where each move consumes (dist+1) minutes   *
 *****************************************************************/
void question5() {
  ios::sync_with_stdio(false); cin.tie(nullptr);  // 입출력 최적화

  int N; if(!(cin>>N)) return;                   // 밸브(노드) 개수 N 입력
  struct Node { string name; int flow; vector<string> neigh; }; // 노드 정보 구조체
  vector<Node> nodes(N);                          // 전체 노드 목록
  unordered_map<string,int> id;                   // 노드 이름 → 인덱스 매핑
  for(int i=0;i<N;++i) {
    int k; cin>>nodes[i].name>>nodes[i].flow>>k;  // 이름, 유량, 이웃 수 k 입력
    nodes[i].neigh.resize(k); id[nodes[i].name]=i;// 네이버 배열 크기 설정 + id 매핑
    for(int j=0;j<k;++j) cin>>nodes[i].neigh[j];  // k 개의 이웃 이름 입력
  }
  // Build unweighted adjacency (bidirectional guaranteed)
  vector<vector<int>> adj(N);                     // 인접 리스트 (인덱스 기반)
  for(int i=0;i<N;++i)
    for(auto &s:nodes[i].neigh)
      adj[i].push_back(id[s]);                    // 문자열 이웃을 인덱스로 변환

  // List of valves with positive flow + start AA
  int start = id["AA"];                          // 시작 노드 "AA" 의 인덱스
  vector<int> important;                          // 중요 밸브 인덱스 리스트
  important.push_back(start);                     // 첫 번째 요소는 항상 AA
  for(int i=0;i<N;++i) if(nodes[i].flow>0) important.push_back(i); // 양수 유량 노드 추가
  const int K = (int)important.size()-1;          // 양수 유량 밸브 수 (≤15, 비트마스크 가능)

  // BFS from each important valve to compute pairwise shortest distances
  const int M = (int)important.size();            // 중요 노드 총수 (AA 포함)
  vector<vector<int>> distMat(M, vector<int>(M, INT_MAX/2)); // 짧은 거리 행렬 초기화
  auto bfs=[&](int srcIdx){                       // 단일 원본 BFS 람다
    int src=important[srcIdx];                   // 원본 노드 인덱스
    vector<int>d(N,INT_MAX/2); d[src]=0;         // 거리 배열 초기화
    queue<int>q; q.push(src);
    while(!q.empty()){
      int u=q.front();q.pop();
      for(int v:adj[u])
        if(d[v]==INT_MAX/2){d[v]=d[u]+1; q.push(v);} // 미방문 노드 거리 갱신
    }
    for(int j=0;j<M;++j) distMat[srcIdx][j]=d[important[j]]; // 결과 저장
  };
  for(int i=0;i<M;++i) bfs(i);                    // 모든 중요 노드에서 BFS 수행

  // Memoization: key = (posIdx, mask, time)
  unordered_map<uint64_t,int> memo; memo.reserve(1<<22); // DP 캐시
  auto keyPack=[&](int pos,int mask,int t){              // 64-bit 키 인코딩
    return (uint64_t)pos | (uint64_t)mask<<6 | (uint64_t)t<<21;}; // 비트필드: pos(6) + mask(15) + time

  function<int(int,int,int)> dfs = [&](int pos,int mask,int time) -> int {
    if (time <= 0) return 0;                       // 시간이 없으면 더 이상 압력 획득 불가
    uint64_t key = keyPack(pos, mask, time);       // 상태 키 생성
    auto it = memo.find(key);                      // 캐시 조회
    if (it != memo.end()) return it->second;       // 이미 계산된 값 반환

    int best = 0;                                  // 최대 압력 결과 변수

    // 1) 현재 밸브 열기 (pos==0 은 AA 이므로 제외)
    if (pos != 0) {
      int bit = pos - 1;                           // AA 제외한 밸브를 0번 비트부터 매핑
      if (!(mask & (1 << bit)) && time > 1) {      // 아직 열지 않았고, 열 시간(1분) 확보
        int gain = (time - 1) * nodes[important[pos]].flow; // (남은시간-1)*유량 = 압력 증가량
        best = max(best, gain + dfs(pos, mask | (1 << bit), time - 1)); // 밸브 열고 재귀
      }
    }

    // 2) 다른 밸브로 이동
    for (int nxt = 1; nxt < M; ++nxt) {            // 중요 밸브(AA 제외) 순회
      if (nxt == pos) continue;                    // 자기 자신 건너뜀
      int bit = nxt - 1;                           // 비트 인덱스 계산
      if (mask & (1 << bit)) continue;             // 이미 연 밸브는 skip
      int d = distMat[pos][nxt];                   // 현재 → nxt 이동 거리
      if (d >= time || d == INT_MAX / 2) continue; // 이동할 시간 부족 or 연결 없음
      best = max(best, dfs(nxt, mask, time - d));  // 이동만 하고 밸브 열기는 이후 단계에서 처리
    }
    return memo[key] = best;                       // 캐시 저장 후 반환
  };

  cout<<dfs(0,0,30)<<"\n";                       // 30분 내 최대 압력 출력
}
