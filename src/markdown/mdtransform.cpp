//
// Created by ubuntu on 2/17/22.
//

#include "mdtransform.hpp"
#include <sstream>
#include <map>
#include <QJsonDocument>
#include <QJsonArray>
#include <iostream>

// HTML 前置标签
const string frontTag[] = {
        "", "<p>", "", "<ul>", "<ol>", "<li>", "<em>", "<strong>",
        "<hr color=#CCCCCC size=1 />", "<br />",
        "", "<blockquote>",
        "<h1 ", "<h2 ", "<h3 ", "<h4 ", "<h5 ", "<h6 ", // 右边的尖括号预留给添加其他的标签属性
        "<pre><code>", "<code>"
};

// HTML 后置标签
const string backTag[] = {
        "", "</p>", "", "</ul>", "</ol>", "</li>", "</em>", "</strong>",
        "", "", "", "</blockquote>",
        "</h1>", "</h2>", "</h3>", "</h4>", "</h5>", "</h6>",
        "</code></pre>", "</code>"
};

std::map<elementType, std::string> elementsVector{
        {elementType::nul,       "text"},
        {elementType::paragraph, "paragraph"},
        {elementType::quote,     "quote-block"},
        {elementType::h1,        "h1"},
        {elementType::h2,        "h2"},
        {elementType::h3,        "h3"},
        {elementType::h4,        "h4"},
        {elementType::h5,        "h5"},
        {elementType::h6,        "h6"},
        {elementType::blockcode, "code-block"}
};

MarkdownTransform::MarkdownTransform(const std::string &content) {
    root = new node(nul);
    now = root;

    std::stringstream fin(content);

    bool newpara = false;
    bool inblock = false;
    while (!fin.eof()) {
        // 从文件中获取一行
        fin.getline(s, maxLength);

        // 处理不在代码块且需要换行的情况
        if (!inblock && isCutline(s)) {
            now = root;
            now->ch.push_back(new node(hr));
            newpara = false;
            continue;
        }

        // std::pair 实质上是一个结构体, 可以将两个数据组合成一个数据
        // 计算一行中开始的空格和 Tab 数
        std::pair<int, char *> ps = start(s);

        // 如果没有位于代码块中, 且没有统计到空格和 Tab, 则直接读取下一行
        if (!inblock && ps.second == nullptr) {
            now = root;
            newpara = true;
            continue;
        }

        // 分析该行文本的类型
        std::pair<elementType, char *> tj = JudgeType(ps.second);

        // 如果是代码块类型
        if (tj.first == blockcode) {
            // 如果位于代码块中, 则 push 一个空类型的节点
            inblock ? now->ch.push_back(new node(nul)) : now->ch.push_back(new node(blockcode));
            inblock = !inblock;
            continue;
        }

        // 如果在代码块中, 直接将内容拼接到当前节点中
        if (inblock) {
            now->ch.back()->elem[0] += string(s) + '\n';
            continue;
        }

        // 如果是普通段落
        if (tj.first == paragraph) {
            if (now == root) {
                now = findnode(ps.first);
                now->ch.push_back(new node(paragraph));
                now = now->ch.back();
            }
            bool flag = false;
            if (newpara && !now->ch.empty()) {
                node *ptr = nullptr;
                for (auto i: now->ch) {
                    if (i->type == nul)
                        ptr = i;
                }
                if (ptr != nullptr)
                    mkpara(ptr);
                flag = true;
            }
            if (flag) {
                now->ch.push_back(new node(paragraph));
                now = now->ch.back();
            }
            now->ch.push_back(new node(nul));
            insert(now->ch.back(), string(tj.second));
            newpara = false;
            continue;
        }

        now = findnode(ps.first);

        // 如果是标题行, 则向其标签中插入属性 tag
        if (tj.first >= h1 && tj.first <= h6) {
            now->ch.push_back(new node(tj.first));
            now->ch.back()->elem[0] = "tag" + to_string(++cntTag);
            insert(now->ch.back(), string(tj.second));
        }

        newpara = false;

    }

    // 深入优先遍历整个语法树
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
        return make_pair(blockcode, ptr + 3);

    // 如果出现 > 且下一个字符为空格，则说明是引用
    if (*ptr == '>' && (ptr[1] == ' '))
        return make_pair(quote, ptr + 1);

    // 否则，就是普通段落
    return make_pair(paragraph, ptr);
}

void MarkdownTransform::dfs(QJsonObject &jsonParentNode, node *v) {
    if (v->type == paragraph && v->elem[0].empty() && v->ch.empty())
        return;

    auto elementName{elementsVector[v->type].c_str()};

    jsonParentNode.insert("name", elementName);
    QJsonArray children;

    //content += frontTag[v->type];
    bool flag = true;

    // 处理标题, 支持用目录进行跳转
    if (isHeading(v)) {
        //content += "id=\"" + v->elem[0] + "\">";
        flag = false;
    }

    // 如果上面三者都不是, 则直接添加内容
    if (flag) {
        //content += v->elem[0];
        flag = false;
        QString elementValue = v->elem[0].c_str();
        std::cout << "elementValue " << elementValue.toStdString() << std::endl;
        jsonParentNode.insert("text", elementValue);
    }

    // 递归遍历所有
    for (int i = 0; i < (int) v->ch.size(); i++) {
        QJsonObject subNode;
        dfs(subNode, v->ch[i]);
        children.push_back(subNode);
    }

    if (children.size() > 0) {
        jsonParentNode.insert("children", children);
    }
    // 拼接为结束标签
    //content += backTag[v->type];
}

string MarkdownTransform::getJsonString() {
    auto strFromObj = QJsonDocument(jsonObject).toJson(QJsonDocument::Compact).toStdString();
    return strFromObj;
}