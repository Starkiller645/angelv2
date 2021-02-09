#ifndef AUTHWORKER_H
#define AUTHWORKER_H

#include "filejson.h"
#include <cpr/cpr.h>
#include <QRunnable>
#include <QtCore>
#include <QObject>
#include <QString>
#include <iostream>
#include <QtWebSockets/QtWebSockets>
#include <QStringLiteral>
#include <QtWebSockets/QWebSocketServer>

namespace authworker {
  enum switch_type {New, Update};
  using namespace Qt;
  class AuthorisationWorker: public QObject, public QRunnable {
      Q_OBJECT
    public:
      AuthorisationWorker(std::string sub = "frontpage", authworker::switch_type type = authworker::New, std::string before = "");
      void run() override;
    public slots:
      void ready_Read();
      void checkCredentials();
      void switchSub();
      void downloadImageFile(std::string);
    signals:
      void credCheckFailed();
      void credCheckSucceeded();
      void onResponseReceived(QString);
      void onDownloadImageFile(std::string);
      void onSwitchSubComplete(std::string, nlohmann::json, nlohmann::json, nlohmann::json, std::string);
    private:
      std::string sub;
      authworker::switch_type type;
      filejson::JsonRead *json;
      std::string before;
  };
};

#endif
