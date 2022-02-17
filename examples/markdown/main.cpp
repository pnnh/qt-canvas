//
//  main.cpp
//  MarkdownParser
//

#include <iostream>
#include "src/markdown/mdtransform.hpp"

int main() {

    std::ifstream fin("static/test.md");
    std::string content((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

    fin.close();

    // 装载构造 Markdown 文件
    MarkdownTransform transformer(content);

    std::string table = transformer.getJsonString();

    std::cout << "======\n" << table;


    return 0;
}
