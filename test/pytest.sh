#!/bin/bash

files=(${@:-*_test.py})
options=("-s" "-rA" "--durations=0" "--name test")
# options=("-s" "-rA" "--durations=0" "-k dump")

cmd=()
cmd+=("pytest")
cmd+=(${files[@]})
cmd+=(${options[@]})


# ${cmd[@]}
# ${cmd[@]} -m="not clean" -m="run"
# ${cmd[@]} -m="not clean"
# ${cmd[@]} -k="unique_tol"
# ${cmd[@]} -k="adjacency"
# ${cmd[@]} -k="texify"
# ${cmd[@]} -k="chunkify or clean" 	
${cmd[@]} -k="clean" 	
# ${cmd[@]} -k="sparsify" 	
# ${cmd[@]} -k="outer"
# ${cmd[@]} -m="clean"


