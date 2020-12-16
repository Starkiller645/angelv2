#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QtWebEngineWidgets>

using namespace Qt;

namespace mainwindow {
  class MainWindow: public QMainWindow {
  public:
    MainWindow();
    void runConnect();
    QWebEngineView *authsite;

  public slots:
    void onResponseReceived(QString);
  };
}
#endif
