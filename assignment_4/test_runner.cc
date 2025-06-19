// extra_tests.cc – Extreme / Edge 케이스 전용 GoogleTest 스위트
//  * run_with_input() : 개행·공백 모두 제거해서 플랫폼 의존성 해소
//  * Question3 / Question4 / Question5 스트레스 테스트 포함
//  * 표준 헤더만 사용 (bits/stdc++.h 제거)

#include <gtest/gtest.h>
#include "library_code.cc"   // 과제 구현 파일

#include <algorithm>
#include <chrono>
#include <cctype>
#include <iostream>
#include <numeric>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

/******************* 공용 유틸 – 표준입출력 래핑 *********************/
namespace {
// in  : 테스트 입력 문자열
// func: questionX() 진입점
// time_limit_ms: 허용 시간 (ms)
template <typename F>
std::string run_with_input(const std::string& in, F func, int time_limit_ms = 1000) {
    std::stringstream input(in), output;
    std::streambuf* cin_buf  = std::cin.rdbuf(input.rdbuf());
    std::streambuf* cout_buf = std::cout.rdbuf(output.rdbuf());

    auto t0 = std::chrono::high_resolution_clock::now();
    func();
    auto t1 = std::chrono::high_resolution_clock::now();

    std::cin.rdbuf(cin_buf);
    std::cout.rdbuf(cout_buf);

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    EXPECT_LT(ms, time_limit_ms) << "시간 초과 (" << ms << " ms)";

    std::string out = output.str();
    out.erase(std::remove(out.begin(), out.end(), '\r'), out.end()); // CR 제거
    // leading & trailing whitespace 제거
    auto not_ws = [](int ch){ return !std::isspace(ch); };
    out.erase(out.begin(), std::find_if(out.begin(), out.end(), not_ws));
    out.erase(std::find_if(out.rbegin(), out.rend(), not_ws).base(), out.end());
    return out;
}
} // namespace

/************************* Question 3 *****************************/
TEST(Question3Extreme, MaxDictionary_LongChain) {
    // beginWord → endWord 5 단어 경로 (길이 4 변환), N = 5000
    std::string begin = "aaaaaaaaaa";
    std::string end   = "bbbbaaaaaa"; // 앞 4글자 차이

    // 실제 경로 (중간 단어 3개)
    std::vector<std::string> path = {
        "baaaaaaaaa", // diff pos 0
        "bbaaaaaaaa", // diff pos 1
        "bbbaaaaaaa"  // diff pos 2
    };

    std::vector<std::string> words;
    words.reserve(5000);
    words.insert(words.end(), path.begin(), path.end());
    words.push_back(end);

    // 잡음 단어 4,995개
    for (int i = 0; words.size() < 5000; ++i) {
        std::string w = begin;
        w[i % 10] = char('c' + (i % 20));
        words.push_back(w);
    }

    // deterministic shuffle
    std::shuffle(words.begin(), words.end(), std::mt19937(42));

    std::ostringstream in;
    in << begin << ' ' << end << ' ' << words.size() << '\n';
    for (size_t i = 0; i < words.size(); ++i)
        in << words[i] << (i + 1 == words.size() ? '\n' : ' ');

    std::string out = run_with_input(in.str(), question3);
    EXPECT_EQ(out, "5");
}

TEST(Question3Extreme, NoPossiblePathHugeN) {
    std::string begin = "abcdefghij", end = "klmnopqrst";
    int N = 5000;
    std::ostringstream in;
    in << begin << ' ' << end << ' ' << N << '\n';
    for (int i = 0; i < N; ++i) {
        std::string w = begin;
        w[0] = char('u' + (i % 6));
        in << w << (i + 1 == N ? '\n' : ' ');
    }
    std::string out = run_with_input(in.str(), question3);
    EXPECT_EQ(out, "0");
}

/************************* Question 4 *****************************/
static long long dijkstra_ref(const std::vector<std::vector<std::tuple<int,int>>>& adj, int D) {
    const long long INF = 4e18;
    int n = adj.size();
    std::vector<std::vector<long long>> dist(n, std::vector<long long>(D + 1, INF));
    using State = std::tuple<long long,int,int>; // cost,u,used
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    dist[0][0] = 0; pq.emplace(0,0,0);
    while (!pq.empty()) {
        auto [c,u,d] = pq.top(); pq.pop();
        if (c != dist[u][d]) continue;
        if (u == n - 1) return c;
        for (auto [v,w] : adj[u]) {
            if (c + w < dist[v][d]) {
                dist[v][d] = c + w; pq.emplace(dist[v][d], v, d);
            }
            if (d < D && c + w / 2 < dist[v][d + 1]) {
                dist[v][d + 1] = c + w / 2; pq.emplace(dist[v][d + 1], v, d + 1);
            }
        }
    }
    return -1;
}

TEST(Question4Extreme, MaxN_MaxD_RandomSparse) {
    constexpr int N = 1000, M = 1000, D = 500;
    std::vector<std::vector<std::tuple<int,int>>> adj(N);
    std::ostringstream in;
    in << N << ' ' << M << '\n';
    for (int i = 0; i < M; ++i) {
        int u = i, v = (i + 1) % N, w = (i % 2 ? 100000 : 99999);
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
        in << u << ' ' << v << ' ' << w << '\n';
    }
    in << D << '\n';

    long long expected = dijkstra_ref(adj, D);
    std::string out = run_with_input(in.str(), question4, 1500);
    EXPECT_EQ(out, std::to_string(expected));
}

TEST(Question4Extreme, UnreachableGraph) {
    std::string in = "4 1\n0 1 7\n0\n";
    std::string out = run_with_input(in, question4);
    EXPECT_EQ(out, "-1");
}

/************************* Question 5 *****************************/
TEST(Question5Extreme, AllZeroFlows) {
    std::string in =
        "2\n"
        "AA 0 1 BB\n"
        "BB 0 1 AA\n";
    std::string out = run_with_input(in, question5, 1500);
    EXPECT_EQ(out, "0");
}

TEST(Question5Extreme, SinglePositiveFlowAtStart) {
    std::string in = "1\nAA 10 0\n";
    std::string out = run_with_input(in, question5, 1500);
    EXPECT_EQ(out, "290"); // 29분 × 10
}
