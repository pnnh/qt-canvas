#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "src/md5.h"
#include "src/wasm/wasm.h"
#include <iostream>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  const QUrl url(QStringLiteral("qrc:/src/qrc/textfield.qml"));

  engine.load(url);

  if (engine.rootObjects().isEmpty())
    return -1;

  std::cout << "compare: " << compareBig(100, 30) << endl;
  md5 m;
  std::cout << "计算MD5：" << m.calcMd5("啊哈哈abc") << endl;

  return app.exec();
}
