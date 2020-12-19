#include <iostream>
#include <QApplication>
#include <QThread>
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
  return app.exec();
}
