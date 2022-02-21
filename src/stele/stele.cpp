//
// Created by ubuntu on 2/21/22.
//

#include "stele.h"
#include <QJsonDocument>
#include <QJsonObject>

std::string buildHtml(const std::string &content) {
  QJsonParseError jsonError;
  auto jsonDoc = QJsonDocument::fromJson(content.c_str(), &jsonError);
  qDebug() << "json转换结果：" << jsonError.errorString();
  QJsonObject jsonObject = jsonDoc.object();

      foreach(const QString &key, jsonObject.keys()) {
      QJsonValue value = jsonObject.value(key);
      qDebug() << "Key = " << key << ", Value = " << value.toString();
    }

}