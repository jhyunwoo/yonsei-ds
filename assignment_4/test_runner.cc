// =============================================
// extreme_tests.cc – 극한 상황 단위 테스트 모음 (수정판)
// -------------------------------------------------
// 기존 버전에서 발견된 두 가지 문제를 해결했습니다.
//  1) Word Ladder 긴 체인 테스트: 사전에 존재하는 더 짧은 경로 가능성을
//     고려하지 않아 실패 → 레퍼런스 BFS 로 기대값을 동적으로 계산.
//  2) Star Network 입력 생성 시 "AA" 가 중복 등록되어 알고리즘 동작을
//     왜곡 → 이제 "AB", "AC", ... 식 고유 이름으로 생성.
// -------------------------------------------------
// 모든 테스트는 GoogleTest 프레임워크를 사용하며, 표준 입출력을
// 문자열 스트림으로 리다이렉트해 실행·검증합니다.
// =============================================

#include <gtest/gtest.h>
#include "library_code.cc"   // 시험 대상 코드

#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>
#include <sstream>
#include <streambuf>
#include <string>
#include <tuple>
#include <vector>

// ---------------- 공통 헬퍼 -----------------
inline std::string strip(std::string s) {
  auto lf = std::find_if_not(s.begin(), s.end(), ::isspace);
  auto rt = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();
  return (lf >= rt) ? "" : std::string(lf, rt);
}

static std::string runWithInput(const std::string &in, void (*f)()) {
  std::stringstream input(in);
  std::stringstream output;
  auto *cin_buf  = std::cin.rdbuf();
  auto *cout_buf = std::cout.rdbuf();
  std::cin.rdbuf(input.rdbuf());
  std::cout.rdbuf(output.rdbuf());

  auto st = std::chrono::high_resolution_clock::now();
  (*f)();
  auto ed = std::chrono::high_resolution_clock::now();
  std::cin.rdbuf(cin_buf);
  std::cout.rdbuf(cout_buf);

  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(ed - st);
  EXPECT_LT(ms.count(), 5000) << "시간 초과 (" << ms.count() << " ms)";

  return strip(output.str());
}

// ============================================================
// Question 3 – Word Transformation Sequence (극한 케이스)
// ============================================================
namespace {

bool oneDiff(const std::string &a, const std::string &b) {
  int diff = 0;
  for (size_t i = 0; i < a.size(); ++i)
    if (a[i] != b[i] && ++diff > 1) return false;
  return diff == 1;
}

// 느려도 괜찮은 N^2 BFS 참고 구현 (테스트 기대값 계산 용)
int wordLadderRef(std::string beginWord,
                  std::string endWord,
                  std::vector<std::string> dict) {
  if (std::find(dict.begin(), dict.end(), endWord) == dict.end()) return 0;
  if (std::find(dict.begin(), dict.end(), beginWord) == dict.end())
    dict.push_back(beginWord);
  int n = (int)dict.size();
  auto id = [&](const std::string &s) {
    return int(std::find(dict.begin(), dict.end(), s) - dict.begin());
  };
  int S = id(beginWord), T = id(endWord);
  std::vector<int> dist(n, -1);
  std::queue<int> q;
  dist[S] = 1;
  q.push(S);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    if (u == T) return dist[u];
    for (int v = 0; v < n; ++v) {
      if (dist[v] == -1 && oneDiff(dict[u], dict[v])) {
        dist[v] = dist[u] + 1;
        q.push(v);
      }
    }
  }
  return 0;
}

TEST(Question3Extreme, MinInputSize) {
  std::string in  = "a b 1\n"  // beginWord, endWord, N
                    "b\n";
  std::string out = runWithInput(in, question3);
  EXPECT_EQ(out, "2");
}

TEST(Question3Extreme, MaxDictionaryNoPath) {
  std::string begin(10, 'a');
  std::string end(10, 'b');
  const int N = 5000;
  std::ostringstream oss;
  oss << begin << ' ' << end << ' ' << N << '\n';
  oss << end << '\n';                   // endWord 포함
  for (int i = 1; i < N; ++i)
    oss << std::string(10, 'c') + char('a' + i % 26) << '\n';
  std::string in = oss.str();
  std::string out = runWithInput(in, question3);
  EXPECT_EQ(out, "0");
}

TEST(Question3Extreme, LongestPossibleChain) {
  const int L = 10, N = 5000;
  std::string begin(L, 'a');
  std::vector<std::string> dict;
  dict.reserve(N);
  std::string cur = begin;
  for (int i = 0; i < N; ++i) {
    cur[i % L] = char('b' + (i % 24)); // 한 글자씩 순환 변경
    dict.push_back(cur);
  }
  std::string end = dict.back();

  std::ostringstream oss;
  oss << begin << ' ' << end << ' ' << N << '\n';
  for (auto &w : dict) oss << w << '\n';
  std::string input = oss.str();

  // 레퍼런스로 정확한 최단 길이 계산
  int expected = wordLadderRef(begin, end, dict);
  std::string out = runWithInput(input, question3);
  EXPECT_EQ(out, std::to_string(expected));
}

} // namespace (Question 3)

// ============================================================
// Question 4 – Minimum Cost With Discounts
// ============================================================
namespace {

long long minCostRef(int N, int D,
                     const std::vector<std::tuple<int, int, int>> &edges) {
  std::vector<std::vector<std::pair<int, int>>> g(N);
  for (auto [u, v, c] : edges) {
    g[u].push_back({v, c});
    g[v].push_back({u, c});
  }
  const long long INF = 4e18;
  std::vector<std::vector<long long>> dist(N,
        std::vector<long long>(D + 1, INF));
  using S = std::tuple<long long, int, int>;
  std::priority_queue<S, std::vector<S>, std::greater<S>> pq;
  dist[0][0] = 0;
  pq.emplace(0LL, 0, 0);
  while (!pq.empty()) {
    auto [w, u, k] = pq.top(); pq.pop();
    if (w != dist[u][k]) continue;
    for (auto [v, c] : g[u]) {
      if (w + c < dist[v][k]) { dist[v][k] = w + c; pq.emplace(dist[v][k], v, k);}
      if (k < D && w + c / 2 < dist[v][k + 1]) {
        dist[v][k + 1] = w + c / 2; pq.emplace(dist[v][k + 1], v, k + 1);
      }
    }
  }
  long long ans = *std::min_element(dist[N - 1].begin(), dist[N - 1].end());
  return ans == INF ? -1 : ans;
}

TEST(Question4Extreme, LineGraphMax) {
  const int N = 1000, C = 100000, D = 500;
  std::ostringstream oss;
  oss << N << ' ' << N - 1 << '\n';
  for (int i = 0; i < N - 1; ++i)
    oss << i << ' ' << i + 1 << ' ' << C << '\n';
  oss << D << '\n';
  long long expected = 500LL * (C / 2) + 499LL * C; // 999 간선
  std::string out = runWithInput(oss.str(), question4);
  EXPECT_EQ(out, std::to_string(expected));
}

TEST(Question4Extreme, Unreachable) {
  std::string in = "4 1\n0 1 5\n0\n"; // 할인 0
  std::string out = runWithInput(in, question4);
  EXPECT_EQ(out, "-1");
}

TEST(Question4Extreme, MinimalGraph) {
  std::string in = "2 1\n0 1 0\n0\n";
  std::string out = runWithInput(in, question4);
  EXPECT_EQ(out, "0");
}

} // namespace (Question 4)

// ============================================================
// Question 5 – Valve Scheduling (Star Network 등)
// ============================================================
namespace {

// 고유 밸브 이름 시퀀스 생성기 (AA 제외)
std::string valveName(int idx) {
  // idx = 0  →  "AB", idx = 1 → "AC" … 'Z' 이후 'BA', 'BB' 순
  int a = idx / 25;          // 25 = ('Z'-'B'+1)
  int b = idx % 25 + 1;      // +1 → 'B' 이상
  char c1 = 'A' + a;
  char c2 = 'A' + b;
  if (c1 == 'A' && c2 == 'A') ++c2; // 안전 장치
  return std::string{ c1, c2 };
}

// AA 중심 star 그래프 입력 생성 (K 양수 유량 밸브)
std::string makeStarInput(int K) {
  int N = K + 1;
  std::ostringstream oss;
  oss << N << '\n';
  // AA 정의
  oss << "AA 0 " << K;
  for (int i = 0; i < K; ++i) oss << ' ' << valveName(i);
  oss << '\n';
  // 각 리프 밸브
  for (int i = 0; i < K; ++i) {
    oss << valveName(i) << " 30 1 AA\n";
  }
  return oss.str();
}

// 간단 수식으로 최적 압력 계산 (star 그래프)
long long starOptimal(int K) {
  long long time = 0, total = 0;
  for (int i = 0; i < K && time + 2 <= 30; ++i) {
    time += (i == 0 ? 2 : 3);      // 1) 이동 + open, 이후 왕복 + open
    if (time > 30) break;
    total += 30LL * (30 - time);   // 남은 시간만큼 압력
  }
  return total;
}

TEST(Question5Extreme, ZeroFlowLargeGraph) {
  std::ostringstream oss;
  oss << 70 << '\n';
  for (int i = 0; i < 70; ++i) {
    char a = 'A' + (i / 26);
    char b = 'A' + (i % 26);
    oss << a << b << " 0 0\n";
  }
  std::string out = runWithInput(oss.str(), question5);
  EXPECT_EQ(out, "0");
}

TEST(Question5Extreme, StarNetworkAllFlows) {
  int K = 5;
  std::string in = makeStarInput(K);
  std::string out = runWithInput(in, question5);
  long long expected = starOptimal(K);
  EXPECT_EQ(out, std::to_string(expected));
}

} // namespace (Question 5)

// ------------------------------------------------------------
// NOTE: main() 은 test_runner.cc 에 이미 정의되어 있습니다.
// ------------------------------------------------------------

// ------------------------------------------------------------
// main 은 test_runner 에서 정의되어 있으므로 생략.
// (이 파일 단독 빌드 시 아래 주석 해제)
// ------------------------------------------------------------
 int main(int argc, char **argv) {
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
