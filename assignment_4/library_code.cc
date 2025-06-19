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

/*
 * ------------------------------------------------------------
 * Question 3 – Word Transformation Sequence (단어 변환 시퀀스)
 * beginWord에서 endWord로, 사전 단어(wordList)만 사용하며
 * 매 단계마다 단어가 정확히 한 글자만 다르도록 변환한다.
 * 가장 짧은 변환 시퀀스의 단어 수를 출력하고, 없으면 0을 출력.
 * ------------------------------------------------------------
 */
void question3() {
  ios::sync_with_stdio(false); // C I/O와 C++ I/O 동기화 해제 → 성능 향상
  cin.tie(nullptr);            // cin 과 cout 묶음 해제 → 성능 향상

  string beginWord, endWord;
  int N;
  // 입력이 실패하면 함수 종료 (EOF 등)
  if (!(cin >> beginWord >> endWord >> N)) return;

  // 단어 목록을 벡터에 저장
  vector<string> dict(N);
  for (auto &w : dict) {
    cin >> w;
  }

  // endWord가 사전에 없으면 변환 불가 → 바로 0 출력 후 종료
  if (find(dict.begin(), dict.end(), endWord) == dict.end()) {
    cout << 0 << "\n";
    return;
  }

  // beginWord가 사전에 없으면 BFS 그래프 노드로 포함하기 위해 추가
  if (find(dict.begin(), dict.end(), beginWord) == dict.end()) {
    dict.push_back(beginWord);
  }

  const int L = beginWord.size(); // 단어 길이 (모든 단어 동일 길이)
  const int total = dict.size();   // 사전 단어 총 수 (beginWord 포함했을 수도 있음)

  /*
   * ‘*’ 와일드카드를 사용해 한 글자만 다른 단어를 O(1)처럼 찾는 trick.
   * key   : "h*t" 처럼 특정 위치 글자를 '*' 로 치환한 문자열
   * value : 해당 key를 공유하는 단어의 인덱스 리스트
   */
  unordered_map<string, vector<int>> bucket;
  bucket.reserve(total * L);  // 해시맵 리사이즈로 성능 최적화

  // 람다: 주어진 단어 s 와 위치 p 에 대해 p 번째 문자를 '*' 로 바꾼 문자열 반환
  auto pat = [&](const string &s, int p) {
    string t = s;
    t[p] = '*';
    return t;
  };

  // 모든 단어에 대해 모든 위치에 와일드카드를 적용해 bucket 생성
  for (int i = 0; i < total; ++i) {
    for (int p = 0; p < L; ++p) {
      bucket[pat(dict[i], p)].push_back(i);
    }
  }

  // beginWord, endWord의 인덱스 구하기
  int S = find(dict.begin(), dict.end(), beginWord) - dict.begin();
  int T = find(dict.begin(), dict.end(), endWord) - dict.begin();

  // BFS 초기화
  queue<int> q;
  vector<int> dist(total, -1); // 방문 여부 + 거리(단어 개수)
  dist[S] = 1;                 // 시작 단어도 시퀀스에 포함하므로 1부터 시작
  q.push(S);

  // BFS 루프: O(N·L·26) 대신 O(N·L) 수준으로 탐색 가능
  while (!q.empty()) {
    int u = q.front();
    q.pop();

    // endWord 도달 시 조기 종료
    if (u == T) break;

    const string &w = dict[u];

    // u 단어와 한 글자 차이인 모든 단어 탐색
    for (int p = 0; p < L; ++p) {
      string key = pat(w, p);     // 패턴 key 생성
      auto &vec = bucket[key];    // 해당 key의 후보 인덱스 벡터 (참조로 받아 직접 비움)
      for (int v : vec) {
        if (dist[v] == -1) {      // 미방문이면 거리 갱신 후 큐에 삽입
          dist[v] = dist[u] + 1;
          q.push(v);
        }
      }
      vec.clear();                // 재방문 방지(중복 간선 제거) → 전체 O(N·L) 보장
    }
  }

  // 결과 출력: dist[T]가 -1 이면 경로 없음 → 0
  cout << (dist[T] == -1 ? 0 : dist[T]) << "\n";
}

/*
 * ------------------------------------------------------------
 * Question 4 – Minimum Cost to Reach City With Discounts (할인 최단경로)
 * D 개의 ‘할인권’으로 간선 비용을 절반(정수 나눗셈)으로 만들 수 있는
 * 다익스트라 변형 문제. 상태 = (도시, 사용한 할인 수)
 * ------------------------------------------------------------
 */
void question4() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;            // N 도시 수, M 고속도로 수
  if (!(cin >> N >> M)) return; // 입력 실패 시 종료

  // 인접 리스트: (이웃 정점, 비용)
  vector<vector<pair<int, int>>> g(N);

  for (int i = 0; i < M; ++i) {
    int u, v, c;
    cin >> u >> v >> c;
    g[u].push_back({v, c}); // 무방향
    g[v].push_back({u, c});
  }

  int D; // 사용할 수 있는 할인권 수
  cin >> D;

  const long long INF = 4e18; // 충분히 큰 값

  // dist[u][k] = 도시 u 에 할인 k 개 사용하고 도착할 때의 최소 비용
  vector<vector<long long>> dist(N, vector<long long>(D + 1, INF));

  using State = tuple<long long, int, int>; // (현재 비용, 도시, 사용한 할인 수)
  priority_queue<State, vector<State>, greater<State>> pq; // 최소 힙

  dist[0][0] = 0;    // 시작 도시 0, 할인 0 개 사용
  pq.emplace(0LL, 0, 0);

  // 다익스트라 루프
  while (!pq.empty()) {
    auto [cost, u, k] = pq.top();
    pq.pop();

    // 이미 더 짧은 경로로 방문한 상태면 스킵
    if (cost != dist[u][k]) continue;

    // 모든 인접 간선 탐색
    for (auto [v, w] : g[u]) {
      // 1) 할인 없이 이동
      if (cost + w < dist[v][k]) {
        dist[v][k] = cost + w;
        pq.emplace(dist[v][k], v, k);
      }

      // 2) 할인 사용하여 이동 (정수 나눗셈)
      if (k < D) {
        long long nc = cost + w / 2; // 할인 적용 비용
        if (nc < dist[v][k + 1]) {
          dist[v][k + 1] = nc;
          pq.emplace(nc, v, k + 1);
        }
      }
    }
  }

  // 목적지 N-1 까지의 모든 할인 사용량 중 최소 비용
  long long ans = *min_element(dist[N - 1].begin(), dist[N - 1].end());
  cout << (ans == INF ? -1 : ans) << '\n';
}

/*
 * ------------------------------------------------------------
 * Question 5 – Valve Control: Efficient Pressure Release Scheduling
 * 30 분 동안 밸브를 열고 이동해 총 압력 방출량을 최대화하는 문제.
 * 1) Floyd–Warshall로 모든 쌍 최단거리 전처리
 * 2) 유량>0 밸브만 ‘중요 밸브’로 압축
 * 3) DFS + 메모이제이션(bitmask + 남은 시간)으로 최적 압력 계산
 * ------------------------------------------------------------
 */
void question5() {
  int N;                // 밸브 수
  if (!(cin >> N)) return;

  struct Info {
    string name;       // 밸브 이름 (두 글자 대문자)
    int flow;          // 분당 유량
    vector<string> adj; // 인접 밸브 이름 목록
  };

  vector<Info> v(N);             // 모든 밸브 정보
  unordered_map<string, int> idx; // 이름 → 정점 인덱스 매핑

  // 입력 파싱
  for (int i = 0; i < N; ++i) {
    int k;                       // 인접 밸브 개수
    cin >> v[i].name >> v[i].flow >> k;
    v[i].adj.resize(k);
    idx[v[i].name] = i;          // 이름 매핑
    for (int j = 0; j < k; ++j) {
      cin >> v[i].adj[j];
    }
  }

  // 인접 리스트를 인덱스 기반으로 변환
  vector<vector<int>> adj(N);
  for (int i = 0; i < N; ++i) {
    for (string &s : v[i].adj) {
      adj[i].push_back(idx[s]);
    }
  }

  /*
   * 1단계) 최단 거리 (분) 계산 – 모든 정점 쌍 Floyd–Warshall (N ≤ 70)
   *       이동만 1 분 소요이므로 간선 가중치 = 1
   */
  const int INF = 1e9;
  vector<vector<int>> d(N, vector<int>(N, INF));
  for (int i = 0; i < N; ++i) {
    d[i][i] = 0;
    for (int j : adj[i]) d[i][j] = 1;
  }
  for (int k = 0; k < N; ++k)
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        d[i][j] = min(d[i][j], d[i][k] + d[k][j]);

  /*
   * 2단계) 중요 밸브(유량>0)만 추려서 bitmask 압축
   */
  vector<int> imp;               // 중요한 밸브 인덱스 목록
  for (int i = 0; i < N; ++i)
    if (v[i].flow > 0) imp.push_back(i);
  const int K = imp.size();      // 중요 밸브 수 (K ≤ 15)

  int start = idx["AA"];        // 시작 지점 인덱스

  // 밸브 인덱스 → bitmask 내 비트 위치 mapping
  unordered_map<int, int> bitPos;
  for (int i = 0; i < K; ++i) bitPos[imp[i]] = i;

  /*
   * 3단계) DFS + 메모이제이션
   *   상태 (u, t, mask)
   *     u    : 현재 위치 (정점 인덱스)
   *     t    : 남은 시간 (분)
   *     mask : 이미 연 중요 밸브 집합 (K 비트)
   *   반환값 : 상태에서 앞으로 얻을 수 있는 최대 추가 압력량
   *   전체 답 = dfs(start, 30, 0)
   */

  // 상태를 64비트 키로 패킹해 unordered_map 메모
  auto pack = [&](int mask, int t, int u) {
    //  |63..16| mask(≤ 15비트) |15..9| t(5비트, 0~30) |8..0| u(9비트, N≤512)
    return (uint64_t(mask) << 16) | (uint64_t(t) << 9) | uint64_t(u);
  };

  unordered_map<uint64_t, int> memo; // 메모이제이션 테이블
  memo.reserve(1 << 21);             // 대략 충분한 버킷 수 확보

  // 재귀 DFS 정의 (lambda → std::function)
  function<int(int, int, int)> dfs = [&](int u, int t, int mask) -> int {
    uint64_t key = pack(mask, t, u);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second; // 캐시 히트

    int best = 0; // 현 상태에서 얻을 수 있는 최대 압력

    // 1) 현재 밸브를 아직 열지 않았고, 열 시간이 남아 있다면 열기 선택
    if (v[u].flow > 0) {
      int b = bitPos[u];                 // 비트 위치
      if (!(mask & (1 << b)) && t > 1) {
        int gain = (t - 1) * v[u].flow;  // 이번 분(1분 소모) 이후 남은 시간×유량
        best = max(best, gain + dfs(u, t - 1, mask | (1 << b)));
      }
    }

    // 2) 아직 열리지 않은 다른 중요 밸브로 이동 후 계속 진행
    for (int x : imp) {
      if (x == u) continue;             // 자기 자신은 건너뜀
      int b = bitPos[x];
      if (mask & (1 << b)) continue;    // 이미 열린 밸브는 제외
      int dist = d[u][x];
      if (dist == INF || dist >= t) continue; // 시간 부족하면 스킵
      best = max(best, dfs(x, t - dist, mask));
    }

    return memo[key] = best;            // 결과 캐싱 후 반환
  };

  // 초기 호출: 현재 위치 AA, 남은 시간 30, 연 밸브 없음
  cout << dfs(start, 30, 0) << "\n";
}
