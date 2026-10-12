#if 0
path=${0}
cmd=g++
flags=()
exe=${path%.*}.out
args=(${@})
trap '[[ -f ${exe} ]] && rm ${exe}' EXIT
${cmd} ${flags[@]} -o ${exe} ${path} && ${exe} ${args[@]}
exit
#endif

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]){
    for (int i = 1; i < argc; ++i) {
		std::cout << argv[i] << "\t" << std::getenv(argv[i]) << std::endl;
	}
	return 0;
}
