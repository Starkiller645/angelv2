#include <iostream>
#include <QApplication>
#include <QThread>
#include <QFile>
#include <QCoreApplication>
#include <QIcon>
#include "mainwindow.h"

using namespace Qt;
using namespace mainwindow;
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/images/angel.ico"));
  /*app.setStyle("breeze");*/
  MainWindow window;
  QFile scrollbar_qss(":/resources/scrollbar.qss");
  scrollbar_qss.open(QFile::ReadOnly);
  QString scrollbar_style = QString(scrollbar_qss.readAll());
  window.setStyleSheet(scrollbar_style);
  return app.exec();
}
