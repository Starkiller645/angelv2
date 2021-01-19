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
  using namespace Qt;
  class AuthorisationWorker: public QObject, public QRunnable {
      Q_OBJECT
    public:
      AuthorisationWorker(std::string sub = "frontpage");
      void run() override;
    public slots:
      void capture_signal();
      void ready_Read();
      void onEchoReceived(QString);
      void checkCredentials();
      void switchSub();
      void downloadImageFile(std::string);
    signals:
      void credCheckFailed();
      void credCheckSucceeded();
      void onResponseReceived(QString);
      void onDownloadImageFile(std::string);
      void onSwitchSubComplete(std::string, nlohmann::json, nlohmann::json, nlohmann::json);
    private:
      std::string sub;
      filejson::JsonRead *json;
  };
};

#endif
