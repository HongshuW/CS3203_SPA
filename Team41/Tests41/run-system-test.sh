cd ../build/src/autotester
./autotester $GITHUB_WORKSPACE/Team41/Tests41/milestone-2/affects/simple_source.txt affects/simple_one_affects_clause_queries.txt milestone-2-out/affects-analysis.xml
./autotester $GITHUB_WORKSPACE/Team41/Tests41/milestone-2/affects/simple_source_2.txt affects/simple_2_one_affects_clause_queries.txt milestone-2-out/affects-2-analysis.xml
./autotester $GITHUB_WORKSPACE/Team41/Tests41/milestone-2/basic-spa/deep_nesting_source.txt basic-spa/deep_nesting_multiple_clauses_queries.txt milestone-2-out/basic-spa-analysis.xml
./autotester $GITHUB_WORKSPACE/Team41/Tests41/milestone-2/calls/multiple_procedures_source.txt calls/multiple_procedures_one_calls_clause_queries.txt milestone-2-out/calls-analysis.xml
./autotester $GITHUB_WORKSPACE/Team41/Tests41/milestone-2/multi-clause/deep_nesting_source.txt multi-clause/deep_nesting_multiple_clauses_queries.txt milestone-2-out/multiple-clauses-analysis.xml
./autotester $GITHUB_WORKSPACE/Team41/Tests41/milestone-2/next/deep_nesting_source.txt next/deep_nesting_one_next_clause_queries.txt milestone-2-out/next-analysis.xml
./autotester $GITHUB_WORKSPACE/Team41/Tests41/milestone-2/next/deep_nesting_source_2.txt next/deep_nesting_2_one_next_clause_queries.txt milestone-2-out/next-2-analysis.xml
./autotester $GITHUB_WORKSPACE/Team41/Tests41/milestone-2/pattern/container_patterns_source.txt pattern/container_patterns_one_pattern_clause_queries.txt milestone-2-out/pattern-analysis.xml
./autotester $GITHUB_WORKSPACE/Team41/Tests41/milestone-2/pattern/stress_patterns_source.txt pattern/stress_patterns_one_pattern_clause_queries.txt milestone-2-out/pattern-stress-analysis.xml
./autotester $GITHUB_WORKSPACE/Team41/Tests41/milestone-2/stress/deep_nesting_source.txt stress/stress_queries.txt milestone-2-out/stress-analysis.xml
./autotester $GITHUB_WORKSPACE/Team41/Tests41/milestone-2/with/deep_nesting_source.txt with/deep_nesting_one_with_clause_queries.txt milestone-2-out/with-analysis.xml
./autotester $GITHUB_WORKSPACE/Team41/Tests41/milestone-2/cache/test_cache_source.txt cache/test_cache_queries.txt milestone-2-out/cache-analysis.xml