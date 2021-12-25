//
// Created by Larry on 2021/12/11.
//

#include "md5.h"
#include <QtCore>

string calcMd5(const string& content) {
  QString md5;
  QString pwd = QString::fromUtf8(content);
  QByteArray bb;

  bb = QCryptographicHash::hash (pwd.toUtf8(), QCryptographicHash::Md5 );
  md5.append(bb.toHex());

  return md5.toStdString();
}

string calcBase64(const string& content) {
  QByteArray text(content.c_str());
  return QString(text.toBase64()).toStdString();
}

string calcHex(const string& content) {
  QByteArray text(content.c_str());
  return QString(text.toHex()).toStdString();
}

string calcSha1(const string& content) {
  QByteArray data;

  data = QCryptographicHash::hash (content.c_str(), QCryptographicHash::Sha1 );

  return data.toHex().toStdString();

}

string calcSha256(const string& content) {
  auto data = QCryptographicHash::hash (content.c_str(), QCryptographicHash::Sha256 );

  return data.toHex().toStdString();
}

string calcSha512(const string& content) {
  auto data = QCryptographicHash::hash (content.c_str(), QCryptographicHash::Sha512 );

  return data.toHex().toStdString();
}
