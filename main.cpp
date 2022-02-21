#include "src/md5.h"
#include "src/wasm/wasm.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include "src/markdown/mdtransform.hpp"

#if EMSCRIPTEN

// 在WebAssembly环境下不执行测试代码
int main(int argc, char *argv[]) {
    std::cout << "======" << std::endl;

}

#else

int main(int argc, char *argv[]) {
    std::cout << "计算MD5：" << calcSha512("啊哈哈abc222") << endl;

//    std::ifstream fin("static/test.md");
//    std::string content((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
//
//    fin.close();

    std::string table = markdown2json("ssss");

    std::cout << "======\n" << table;

    return 0;
}

#endif //EMSCRIPTEN



