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

// 词法关键字枚举
enum elementType {
    nul = 0,
    paragraph = 1,
    href = 2,
    ul = 3,
    ol = 4,
    li = 5,
    em = 6,
    strong = 7,
    hr = 8,
    br = 9,
    image = 10,
    quote = 11,
    h1 = 12,
    h2 = 13,
    h3 = 14,
    h4 = 15,
    h5 = 16,
    h6 = 17,
    blockcode = 18,
    code = 19
};

typedef struct Cnode {
    vector<Cnode *> ch;
    string heading;
    string tag;

    Cnode(const string &hd) : heading(hd) {}
} Cnode;

typedef struct node {
    elementType type;                           // 节点代表的类型
    vector<node *> ch;
    string elem[3];                     // 用来存放三个重要的属性, elem[0] 保存了要显示的内容
    // elem[1] 保存了链接, elem[2] 则保存了 title
    node(elementType _type) : type(_type) {}
} node;

class MarkdownTransform {
private:
    node *root, *now;
    int cntTag = 0;
    char s[maxLength];
    QJsonObject jsonObject;

    // 判断是否为标题
    inline bool isHeading(node *v) {
        return (v->type >= h1 && v->type <= h6);
    }


    // 递归销毁数节点
    template<typename T>
    void destroy(T *v) {
        for (int i = 0; i < (int) v->ch.size(); i++) {
            destroy(v->ch[i]);
        }
        delete v;
    }

    void Cins(Cnode *v, int x, const string &hd, int tag) {
        int n = (int) v->ch.size();
        if (x == 1) {
            v->ch.push_back(new Cnode(hd));
            v->ch.back()->tag = "tag" + to_string(tag);
            return;
        }

        if (!n || v->ch.back()->heading.empty())
            v->ch.push_back(new Cnode(""));
        Cins(v->ch.back(), x - 1, hd, tag);
    }

    void dfs(QJsonObject &jsonNode, node *v);

    // 判断是否换行
    inline bool isCutline(char *src) {
        int cnt = 0;
        char *ptr = src;
        while (*ptr) {
            // 如果不是 空格、tab、- 或 *，那么则不需要换行
            if (*ptr != ' ' && *ptr != '\t' && *ptr != '-')
                return false;
            if (*ptr == '-')
                cnt++;
            ptr++;
        }
        // 如果出现 --- 则需要增加一个分割线, 这时需要换行
        return (cnt >= 3);
    }

    // 生成段落
    inline void mkpara(node *v) {
        if (v->ch.size() == 1u && v->ch.back()->type == paragraph)
            return;
        if (v->type == paragraph)
            return;
        if (v->type == nul) {
            v->type = paragraph;
            return;
        }
        node *x = new node(paragraph);
        x->ch = v->ch;
        v->ch.clear();
        v->ch.push_back(x);
    }

    // 开始解析一行中开始的空格和 Tab
    // src: 源串
    // 返回值: 由空格数和有内容处的 char* 指针组成的 std::pair
    inline pair<int, char *> start(char *src) {
        // 如果该行内容为空，则直接返回
        if ((int) strlen(src) == 0)
            return make_pair(0, nullptr);
        // 统计空格键和 Tab 键的个数
        int cntspace = 0, cnttab = 0;
        // 从该行的第一个字符读其, 统计空格键和 Tab 键,
        // 当遇到不是空格和 Tab 时，立即停止
        for (int i = 0; src[i] != '\0'; i++) {
            if (src[i] == ' ') cntspace++;
            else if (src[i] == '\t') cnttab++;
            // 如果内容前有空格和 Tab，那么将其统一按 Tab 的个数处理,
            // 其中, 一个 Tab = 四个空格
            return make_pair(cnttab + cntspace / 4, src + i);
        }
        return make_pair(0, nullptr);
    }

    // 判断当前行的类型
    // src: 源串
    // 返回值: 当前行的类型和除去行标志性关键字的正是内容的 char* 指针组成的 std::pair
    inline pair<elementType, char *> JudgeType(char *src);

    // 给定树的深度寻找节点
    // depth: 树的深度
    // 返回值: 找到的节点指针
    inline node *findnode(int depth) {
        node *ptr = root;
        while (!ptr->ch.empty() && depth != 0) {
            ptr = ptr->ch.back();
            if (ptr->type == li)
                depth--;
        }
        return ptr;
    }

    // 向指定的节点中插入要处理的串
    // v: 节点
    // src: 要处理的串
    void insert(node *v, const string &src) {
        int n = (int) src.size();
        bool incode = false,
                inem = false,
                instrong = false,
                inautolink = false;
        v->ch.push_back(new node(nul));

        for (int i = 0; i < n; i++) {
            char ch = src[i];
            if (ch == '\\') {
                ch = src[++i];
                v->ch.back()->elem[0] += string(1, ch);
                continue;
            }

            // 处理行内代码
            if (ch == '`' && !inautolink) {
                incode ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(code));
                incode = !incode;
                continue;
            }

            // 处理加粗
            if (ch == '*' && (i < n - 1 && (src[i + 1] == '*')) && !incode && !inautolink) {
                ++i;
                instrong ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(strong));
                instrong = !instrong;
                continue;
            }
            if (ch == '_' && !incode && !instrong && !inautolink) {
                inem ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(em));
                inem = !inem;
                continue;
            }

            // 处理图片
            if (ch == '!' && (i < n - 1 && src[i + 1] == '[')
                && !incode && !instrong && !inem && !inautolink) {
                v->ch.push_back(new node(image));
                for (i += 2; i < n - 1 && src[i] != ']'; i++)
                    v->ch.back()->elem[0] += string(1, src[i]);
                i++;
                for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++)
                    v->ch.back()->elem[1] += string(1, src[i]);
                if (src[i] != ')')
                    for (i++; i < n - 1 && src[i] != ')'; i++)
                        if (src[i] != '"')
                            v->ch.back()->elem[2] += string(1, src[i]);
                v->ch.push_back(new node(nul));
                continue;
            }

            // 处理超链接
            if (ch == '[' && !incode && !instrong && !inem && !inautolink) {
                v->ch.push_back(new node(href));
                for (i++; i < n - 1 && src[i] != ']'; i++)
                    v->ch.back()->elem[0] += string(1, src[i]);
                i++;
                for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++)
                    v->ch.back()->elem[1] += string(1, src[i]);
                if (src[i] != ')')
                    for (i++; i < n - 1 && src[i] != ')'; i++)
                        if (src[i] != '"')
                            v->ch.back()->elem[2] += string(1, src[i]);
                v->ch.push_back(new node(nul));
                continue;
            }

            v->ch.back()->elem[0] += string(1, ch);
            if (inautolink) v->ch.back()->elem[1] += string(1, ch);
        }
        if (src.size() >= 2)
            if (src.at(src.size() - 1) == ' ' && src.at(src.size() - 2) == ' ')
                v->ch.push_back(new node(br));
    }

public:
    // 构造函数
    MarkdownTransform(const std::string &content);

    string getJsonString();

    // 析构函数
    ~MarkdownTransform() {
        destroy<node>(root);
    }

};


#endif //QT_CANVAS_MDTRANSFORM_H
