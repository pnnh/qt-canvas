//
// Created by Larry on 2021/12/11.
//

#ifndef QT_CANVAS_MD5_H
#define QT_CANVAS_MD5_H

#include <string>

using namespace std;

string calcMd5(const string& content);
string calcBase64(const string& content);
string calcHex(const string& content);
string calcSha1(const string& content);
string calcSha256(const string& content);
string calcSha512(const string& content);

#endif //QT_CANVAS_MD5_H
