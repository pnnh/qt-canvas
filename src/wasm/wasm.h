//
// Created by Larry on 2021/12/11.
//

#ifndef QT_CANVAS_WASM_H
#define QT_CANVAS_WASM_H

#include <string>
#include <iostream>

using namespace std;

float compareBig(int a, int b) {
  return a > b ? a : b;
}

#ifdef __EMSCRIPTEN__

#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>


EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("compareBig", &compareBig);
}

emscripten::val tryCharPtr() {
  const char* str = "abc哈哈😁☺";
  return emscripten::val(str);
}

emscripten::val tryStdString() {
  std::string str = "abc哈哈O(∩_∩)O哈哈~🐶efg☺";
  return emscripten::val(str);
}

emscripten::val tryCalcMd5(const std::string& content) {
  auto md5Val = calcMd5(content);
  return emscripten::val(md5Val);
}

emscripten::val tryCalcBase64(const std::string& content) {
  auto value = calcBase64(content);
  return emscripten::val(value);
}

emscripten::val tryCalcHex(const std::string& content) {
  auto value = calcHex(content);
  return emscripten::val(value);
}

emscripten::val tryCalcSha1(const std::string& content) {
  auto value = calcSha1(content);
  return emscripten::val(value);
}

emscripten::val tryCalcSha256(const std::string& content) {
  auto value = calcSha256(content);
  return emscripten::val(value);
}

emscripten::val tryCalcSha512(const std::string& content) {
  auto value = calcSha512(content);
  return emscripten::val(value);
}

EMSCRIPTEN_BINDINGS(demo) {
  emscripten::function("tryCharPtr", &tryCharPtr);
  emscripten::function("tryStdString", &tryStdString);
  emscripten::function("tryCalcMd5", &tryCalcMd5);
  emscripten::function("tryCalcBase64", &tryCalcBase64);
  emscripten::function("tryCalcHex", &tryCalcHex);
  emscripten::function("tryCalcSha1", &tryCalcSha1);
  emscripten::function("tryCalcSha256", &tryCalcSha256);
  emscripten::function("tryCalcSha512", &tryCalcSha512);
}

extern "C" {

EMSCRIPTEN_KEEPALIVE int myFunction() {
  printf("我的函数已被调用\n");
  return 0;
}

EMSCRIPTEN_KEEPALIVE int set_data_demo(char* data, int len)
{
  printf("我的函数已被调用2\n");
  //auto str = QString::fromUtf8(data, len);
  string str = data;
  //qInfo() << "received:" << str;
  //QByteArray ba = str.toLocal8Bit();
  const char *c_str2 = str.c_str();
  printf("str2: %s", c_str2);
  return strlen(data);
}

}
#endif




#endif //QT_CANVAS_WASM_H
