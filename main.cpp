#include "src/md5.h"
#include "src/wasm/wasm.h"
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "计算MD5：" << calcSha512("啊哈哈abc222") << endl;
}

