#ifndef AUTHWORKER_H
#define AUTHWORKER_H

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
      AuthorisationWorker();
      void run() override;
    public slots:
      void capture_signal();
      void ready_Read();
      void onEchoReceived(QString);
    signals:
      void onResponseReceived(QString);
    private:
      /*QWebSocketServer sock_server;*/
  };
};

#endif
