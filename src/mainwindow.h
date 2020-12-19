#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainui.h"

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
    QWidget *mainWidget;
    mainui::MainUI *ui;
  public slots:
    void doSetupMainUI();
    void onResponseReceived(QString);
  private:
    std::string bearer_token;
    std::string refresh_token;
    QVBoxLayout *mainLayout;
  };
}
#endif
