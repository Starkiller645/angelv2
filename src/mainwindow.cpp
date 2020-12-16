#include "mainwindow.h"
#include <QLabel>
#include <QObject>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QPushButton>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QUrl>
#include <QWidget>
#include <iostream>
#include "authworker.h"

using namespace Qt;
using namespace mainwindow;

MainWindow::MainWindow() {
  this->authsite = new QWebEngineView;
  QLabel *label = new QLabel("<b>angel</b>v1.0");
  QLabel *angelLogo = new QLabel();
  QVBoxLayout *loginLayout = new QVBoxLayout();
  QPushButton *signinReddit = new QPushButton();
  QPushButton *browse = new QPushButton("Browse without login");
  QWidget *mainWidget = new QWidget();
  QPixmap angelPixmap("/opt/angel-reddit/angel.png");
  const QIcon redditIcon("/opt/angel-reddit/reddit.png");

  const QPixmap angelPixmapScaled = angelPixmap.scaled(300, 300, Qt::KeepAspectRatio);
  angelLogo->setPixmap(angelPixmapScaled);
  signinReddit->setIconSize(QSize(300,85));
  signinReddit->setFixedSize(redditIcon.actualSize(redditIcon.availableSizes().first()));
  signinReddit->setIcon(redditIcon);
  signinReddit->setStyleSheet("border-radius: 3px;");
  mainWidget->setLayout(loginLayout);
  loginLayout->addWidget(angelLogo);
  loginLayout->addWidget(label);
  browse->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  loginLayout->addWidget(signinReddit);
  loginLayout->addWidget(browse);
  label->setStyleSheet("font-size: 60px;");
  loginLayout->setAlignment(Qt::AlignCenter);
  loginLayout->setAlignment(label, Qt::AlignHCenter);
  loginLayout->setAlignment(browse, Qt::AlignHCenter);
  QObject::connect(signinReddit, &QPushButton::clicked,
	  this, &MainWindow::runConnect);

  setCentralWidget(mainWidget);
  show();
  setWindowTitle("Angel v1 beta 1");
  resize(1080, 640);
}
void MainWindow::runConnect(){
  QThreadPool *threadpool = new QThreadPool();
  authworker::AuthorisationWorker *receive = new authworker::AuthorisationWorker();
  this->authsite->setWindowTitle("Authenticate with Reddit");
  this->authsite->load(QUrl("https://www.reddit.com/api/v1/authorize?client_id=Jq0BiuUeIrsr3A&response_type=code&state=JDOfne0oPnf&redirect_uri=http://localhost:8080&duration=permanent&scope=account"));
  threadpool->start(receive);
  this->authsite->show();
  connect(receive, &authworker::AuthorisationWorker::onResponseReceived, this, &MainWindow::onResponseReceived);
  /*receiveThread->finished.connect(initUI);*/
  /*receiveThread->started.connect(receive.receiveRedditConnection)*/
}

void MainWindow::onResponseReceived(QString request_qstr) {
  std::cout << "Response received, control handed to main thread" << std::endl;
  std::string request = request_qstr.toStdString();
  std::string delim = "&code=";
  std::cout << request << std::endl;
  std::string token = request.substr(request.length(), request.find(delim));
  std::cout << token;
  this->authsite->hide();
}
