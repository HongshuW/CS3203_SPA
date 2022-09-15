#!/bin/bash
# before first execution: chmod 777 test-source-simple.sh to set execution permission
# remember to build the latest autotester before execution
# paths to edit
autotester=../../../Code41/cmake-build-debug/src/autotester/autotester
# the path for source and query files
filesPath=./
# where do you want the output to be located, remember to put analysis.xsl under this path
outputPath=../m1-out/

source=source-simple.txt

query_prefix=queries-simple-
query_suffix=-clause.txt

output_prefix=simle-
output_suffix=-clause-out.xml

types=("no" "one-follows" "one-parent" "one-uses")

for type in "${types[@]}"
do
	 $autotester $filesPath$source $filesPath$query_prefix$type$query_suffix $outputPath$output_prefix$type$output_suffix
done

#open browser to view result, can comment out
cd $outputPath
open http://localhost:8000
python -m http.server 8000

