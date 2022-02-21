//
// Created by ubuntu on 2/17/22.
//

#include "mdtransform.hpp"
#include <sstream>
#include <map>
#include <QJsonDocument>
#include <QJsonArray>
#include <iostream>


#include <cstdlib>
#include <fstream>
#include <vector>
#include <cstring>
#include <utility>
#include <string>
#include <cctype>
#include <cstdio>
#include <QJsonObject>

using namespace std;

#define maxLength 10000

enum elementType {
    container = 0,
    text = 1,
    paragraph = 2,
    h1 = 3,
    h2 = 4,
    h3 = 5,
    h4 = 6,
    h5 = 7,
    h6 = 8,
    codeBlock = 9
};

struct elementNode {
    elementType type;
    std::string name;
    std::string text;
    vector<elementNode> children;
    bool multiline;
};

class MarkdownTransform {
private:
    elementNode root;
    char s[maxLength];
    QJsonObject jsonObject;

    // 判断是否为标题
    inline bool isHeading(elementNode *v) {
        return (v->type >= h1 && v->type <= h6);
    }

    void dfs(QJsonObject &jsonNode, elementNode *v);

    // 判断当前行的类型
    // src: 源串
    // 返回值: 当前行的类型和除去行标志性关键字的正是内容的 char* 指针组成的 std::pair
    inline pair<elementType, char *> JudgeType(char *src);

    void dfs(QJsonObject &jsonParentNode, elementNode &v);

public:
    // 构造函数
    MarkdownTransform(const std::string &content);

    string getJsonString();

    // 析构函数
    ~MarkdownTransform() {
    }

};

elementNode containerNode{.type=elementType::container, .name="container"};
elementNode textNode{.type=elementType::text, .name="text"};
elementNode h1Node{.type=elementType::h1, .name="h1", .multiline=false};
elementNode paragraphNode{.type=elementType::paragraph, .name="paragraph", .multiline=true};
elementNode codeBlockNode{.type=elementType::codeBlock, .name="code-block", .multiline=true};


MarkdownTransform::MarkdownTransform(const std::string &content) {
    root = elementNode{.type=elementType::container};

    std::stringstream fin(content);

    while (!fin.eof()) {
        // 从文件中获取一行
        fin.getline(s, maxLength);

        // 分析该行文本的类型
        std::pair<elementType, char *> tj = JudgeType(s);

        // 如果是普通段落
        if (tj.first == paragraph) {
            if (tj.second == nullptr || strlen(tj.second) == 0) {
                continue;
            }
            if (root.children.empty() || !root.children.back().multiline) {
                elementNode paraNode = paragraphNode;
                root.children.push_back(paraNode);
            }
            elementNode tn{.type=elementType::text};
            tn.name = "text";
            tn.text = tj.second;
            root.children.back().children.push_back(tn);
        }
        // 如果是标题行, 则向其标签中插入属性 tag
        if (tj.first >= h1 && tj.first <= h6) {
            elementNode headNode = elementNode{.type = static_cast<elementType>(tj.first),
                    .name="h" + to_string(tj.first)};
            elementNode tn{.type=elementType::text};
            tn.name = "text";
            tn.text = string(tj.second);
            headNode.children.push_back(tn);
            root.children.push_back(headNode);
            continue;
        }
        // 如果是代码块类型
        if (tj.first == elementType::codeBlock && root.children.back().type != elementType::codeBlock) {
            elementNode paraNode = codeBlockNode;
            root.children.push_back(paraNode);
        }
    }

    dfs(jsonObject, root);
}

inline pair<elementType, char *> MarkdownTransform::JudgeType(char *src) {
    char *ptr = src;

    // 跳过 `#`
    while (*ptr == '#') ptr++;

    // 如果出现空格, 则说明是 `<h>` 标签
    if (ptr - src > 0 && *ptr == ' ')
        return make_pair(static_cast<elementType>(ptr - src + h1 - 1), ptr + 1);

    // 重置分析位置
    ptr = src;

    // 如果出现 ``` 则说明是代码块
    if (strncmp(ptr, "```", 3) == 0)
        return make_pair(elementType::codeBlock, ptr + 3);

    // 否则，就是普通段落
    return make_pair(paragraph, ptr);
}

void MarkdownTransform::dfs(QJsonObject &jsonParentNode, elementNode &v) {
    jsonParentNode.insert("name", v.name.c_str());
    QJsonArray children;

    if (!v.text.empty()) {
        jsonParentNode.insert("text", v.text.c_str());
    }

    // 递归遍历所有
    for (auto &i: v.children) {
        QJsonObject subNode;
        dfs(subNode, i);
        children.push_back(subNode);
    }

    if (!children.empty()) {
        jsonParentNode.insert("children", children);
    }
}

string MarkdownTransform::getJsonString() {
    auto strFromObj = QJsonDocument(jsonObject).toJson(QJsonDocument::Indented).toStdString();
    return strFromObj;
}

std::string markdown2json(const std::string &content) {
    MarkdownTransform transformer(content);
    std::string table = transformer.getJsonString();
    return table;
}