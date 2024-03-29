#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainui.h"

#include <iostream>
#include <QMainWindow>
#include <QtWebEngineWidgets>
#include <nlohmann/json.hpp>
#include <waitingspinnerwidget.h>

using namespace Qt;

namespace mainwindow {
  class MainWindow: public QMainWindow {
  public:
    MainWindow();
    void runConnect();
    QWebEngineView *authsite;
    QWidget *mainWidget;
    QVBoxLayout *loginLayout;
    WaitingSpinnerWidget *spinner;
    mainui::MainUI *ui;
  public slots:
    void doSetupMainUI();
    void doSetupLoginUI();
    void onResponseReceived(QString);
  private:
    void checkCredentials();
    std::string bearer_token;
    std::string refresh_token;
    QPushButton *signinReddit;
    QPushButton *browse;
    QVBoxLayout *mainLayout;
    nlohmann::json conf_json;
  };
}
#endif
