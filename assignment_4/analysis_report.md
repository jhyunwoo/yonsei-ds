# Assignment 4 - Program Analysis Report

## Question 3: Word Transformation Sequence

### Algorithm Overview
The solution uses a Breadth-First Search (BFS) approach to find the shortest transformation sequence. The problem is modeled as an unweighted graph where:
- Each word is a vertex
- An edge exists between two words if they differ by exactly one letter

### Time Complexity Analysis
- **Building the graph**: O(N² × L), where N is the number of words and L is the length of each word
  - We compare each pair of words: O(N²)
  - For each pair, we check if they differ by one letter: O(L)
- **BFS traversal**: O(N + E), where E is the number of edges
  - In the worst case, E can be O(N²) if many words differ by one letter
- **Overall**: O(N² × L)

### Space Complexity Analysis
- **Word storage**: O(N × L) to store all words
- **Adjacency list**: O(E) where E is the number of edges, worst case O(N²)
- **BFS queue and distance array**: O(N)
- **Overall**: O(N² + N × L)

### Key Optimizations
1. Early termination when endWord is not in the word list
2. Using integer indices instead of string comparisons during BFS
3. Efficient one-letter difference check with early termination

---

## Question 4: Minimum Cost to Reach City With Discounts

### Algorithm Overview
The solution uses a modified Dijkstra's algorithm with an extended state space. The state is represented as (city, discounts_used), allowing us to track the minimum cost to reach each city with a specific number of discounts used.

### Time Complexity Analysis
- **Dijkstra's algorithm with extended state**: O((N × D) × log(N × D) × M)
  - State space size: N cities × (D+1) discount states = O(N × D)
  - Each state can be pushed to the priority queue at most M times (number of edges)
  - Priority queue operations: O(log(N × D))
- **Overall**: O(N × D × M × log(N × D))

### Space Complexity Analysis
- **Distance array**: O(N × D) to store minimum cost for each (city, discounts_used) pair
- **Priority queue**: O(N × D) in the worst case
- **Adjacency list**: O(M) to store all edges
- **Overall**: O(N × D + M)

### Key Optimizations
1. Using a 2D distance array to track costs for different discount levels
2. Structured bindings (C++17) for cleaner code
3. Early termination when a better path is already found

---

## Question 5: Valve Control: Efficient Pressure Release Scheduling

### Algorithm Overview
The solution combines several techniques:
1. **Floyd-Warshall** to precompute shortest paths between all valve pairs
2. **DFS with memoization** to explore all possible valve opening sequences
3. **Bitmask** to represent opened valves efficiently

### Time Complexity Analysis
- **Floyd-Warshall**: O(N³) where N is the total number of valves
- **DFS with memoization**:
  - State space: O(N × T × 2^K) where T is time (30) and K is number of important valves (≤15)
  - Each state computation: O(K) to check all important valves
  - Overall DFS: O(N × T × 2^K × K)
- **Overall**: O(N³ + N × T × 2^K × K)

### Space Complexity Analysis
- **Distance matrix**: O(N²) for all-pairs shortest paths
- **Memoization table**: O(N × T × 2^K)
- **Graph representation**: O(N × average_degree)
- **Overall**: O(N² + N × T × 2^K)

### Key Optimizations
1. Only considering valves with positive flow rates as destinations
2. Using bitmask to efficiently represent opened valve states
3. Memoization to avoid recomputing identical states
4. Precomputing shortest paths to avoid repeated BFS calls

### Implementation Notes
- The algorithm handles the constraint that opening a valve takes 1 minute
- It correctly calculates pressure release as (remaining_time × flow_rate)
- The state representation ensures we don't revisit the same configuration

---

## General Code Quality
- All implementations include proper comments explaining the algorithms
- Input/output handling follows the specified format exactly
- Edge cases are handled (e.g., no path exists, already at destination)
- Memory usage is optimized where possible
- Modern C++ features (auto, lambda, structured bindings) are used for clarity
