//
// Created by ubuntu on 2/17/22.
//

#ifndef QT_CANVAS_MDTRANSFORM_H
#define QT_CANVAS_MDTRANSFORM_H


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


#endif //QT_CANVAS_MDTRANSFORM_H
