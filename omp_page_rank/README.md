# PageRank OMP implementation

-- Generate random graphs and test $A^m$ counter of number of paths. Evaluate it within $log_2(m)$ matmul-s.
-- Generate small random (or select some graph and implement pagerank procedure. Compare with naive ranking.
 Tip: Moscow subway graph is a nice example (but rather tiny). 
-- Try all tricks for evaluation of performance: optimization, blas, openmp, loops placing.
-- Science oriented guys can study SimRank.

## Sources

* **path_count_log2.c** - Number of paths in graph with O(log2(n)) ; n - path length
* **omp_gs_page_rank.c** - Paralell PageRank implementation with Gauss-Seidel solving algorithm
* **naive_page_rank.c** - PageRank with eigen value implementation
