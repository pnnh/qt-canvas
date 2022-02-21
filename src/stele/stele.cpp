//
// Created by ubuntu on 2/21/22.
//

#include "stele.h"
#include <QJsonDocument>
#include <QJsonObject>

std::string buildHtml(const std::string &content) {
    QJsonParseError jsonError;
    auto jsonDoc = QJsonDocument::fromJson(content.c_str(), &jsonError);
    
}