#!/bin/bash
# before first execution: chmod 777 
# remember to build the latest autotester before execution
# paths to edit
autotester=../../../Code41/cmake-build-release/src/autotester/autotester
# the path for source and query files
filesPath=./
# where do you want the output to be located, remember to put analysis.xsl under this path
outputPath=../m1-out/

source=simple_source.txt

query_prefix=simple_
query_suffix=_clause_queries.txt

output_prefix=simle_
output_suffix=_clause_out.xml

#check if the directory exist and make one if not
[ ! -d "../m1-out/" ] && mkdir ../m1-out

types=("no" "one_follows" "one_parent" "one_uses" "one_pattern" "one_modifies" "two_follows_pattern"
"two_modifies_pattern" "two_parent_pattern" "two_uses_pattern")

for type in "${types[@]}"
do
	 $autotester $filesPath$source $filesPath$query_prefix$type$query_suffix $outputPath$output_prefix$type$output_suffix
done

#open browser to view result, can comment out
cd $outputPath
open http://localhost:8000
python3 -m http.server 8000

