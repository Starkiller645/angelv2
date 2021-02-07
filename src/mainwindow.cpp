#include "mainwindow.h"
#include "authworker.h"
#include "transparentbutton.h"
#include "filejson.h"
#include "mainui.h"
#include <waitingspinnerwidget.h>

#include <QLabel>
#include <QObject>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QPushButton>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QUrl>
#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QEasingCurve>

#include <iostream>
#include <unistd.h>
#include <fstream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using namespace Qt;
using json = nlohmann::json;
using namespace mainwindow;

MainWindow::MainWindow() {
  this->authsite = new QWebEngineView();
  this->signinReddit = new QPushButton();
  this->mainLayout = new QVBoxLayout();
  this->browse = new QPushButton();
  QLabel *label = new QLabel("<b>angel</b>v1.0");
  QLabel *angelLogo = new QLabel();
  this->loginLayout = new QVBoxLayout();
  this->mainWidget = new QWidget();
  QPixmap angelPixmap(":/images/angel.png");

  const QPixmap angelPixmapScaled = angelPixmap.scaled(300, 300, Qt::KeepAspectRatio);
  angelLogo->setPixmap(angelPixmapScaled);
  this->mainWidget->setLayout(this->loginLayout);
  this->loginLayout->addWidget(angelLogo);
  this->loginLayout->addWidget(label);
  label->setStyleSheet("font-size: 60px;");
  this->loginLayout->setAlignment(Qt::AlignCenter);
  setCentralWidget(mainWidget);
  resize(1080, 640);
  show();
  this->mainWidget->setStyleSheet("background-color: #eff0f1; color: #232629;");
  setWindowTitle("Angel v1 beta 1");
  QWidget *statusWidget = new QWidget();
  QHBoxLayout *statusLayout = new QHBoxLayout();
  this->spinner = new WaitingSpinnerWidget();
  statusWidget->setLayout(statusLayout);
  statusLayout->addWidget(spinner);
  this->checkCredentials();
  this->loginLayout->addWidget(statusWidget);
  statusLayout->setAlignment(Qt::AlignCenter);
  this->spinner->setStyleSheet("padding: 4px;");
  this->spinner->start();
}
void MainWindow::runConnect() {
  QThreadPool *threadpool = new QThreadPool();
  authworker::AuthorisationWorker *receive = new authworker::AuthorisationWorker();
  this->authsite->setWindowTitle("Authenticate with Reddit");
  this->authsite->load(QUrl("https://www.reddit.com/api/v1/authorize.compact?client_id=Jq0BiuUeIrsr3A&response_type=code&state=JDOfne0oPnf&redirect_uri=http://localhost:8800&duration=permanent&scope=read+identity"));
  threadpool->start(receive);
  this->authsite->show();
  QObject::connect(receive, &authworker::AuthorisationWorker::onResponseReceived, this, &MainWindow::onResponseReceived);
}

void MainWindow::doSetupLoginUI() {
  filejson::JsonRead jsonreader(std::string(std::getenv("HOME")) + std::string("/.config/angel.json"));
  nlohmann::json conf_json = jsonreader.runSynced();
  this->spinner->deleteLater();
  this->signinReddit->deleteLater();
  this->browse->deleteLater();
  this->signinReddit = new QPushButton();
  this->browse = new QPushButton("Browse without login");
  this->browse->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  this->loginLayout->addWidget(signinReddit);
  this->loginLayout->addWidget(browse);
  this->signinReddit->setIconSize(QSize(300,85));
  const QIcon redditIcon(":/images/reddit.png");
  this->signinReddit->setFixedSize(redditIcon.actualSize(redditIcon.availableSizes().first()));
  this->signinReddit->setIcon(redditIcon);
  this->signinReddit->setStyleSheet("border-radius: 3px;");
  QObject::connect(this->browse, &QPushButton::clicked, this, &MainWindow::doSetupMainUI);
  QObject::connect(this->signinReddit, &QPushButton::clicked,
	  this, &MainWindow::runConnect);
  this->loginLayout->setAlignment(this->browse, Qt::AlignHCenter);
}

void MainWindow::checkCredentials() {
  QThread *credentialThread = new QThread();
  authworker::AuthorisationWorker *credworker = new authworker::AuthorisationWorker();
  credentialThread->setObjectName("Credential Check Thread");
  QObject::connect(credentialThread, &QThread::started, credworker, &authworker::AuthorisationWorker::checkCredentials, Qt::UniqueConnection);
  QObject::connect(credworker, &authworker::AuthorisationWorker::credCheckFailed, this, [=](){this->doSetupLoginUI();});
  QObject::connect(credworker, &authworker::AuthorisationWorker::credCheckSucceeded, this, [=](){this->doSetupMainUI();});
  credworker->moveToThread(credentialThread);
  credentialThread->start();
}

void MainWindow::onResponseReceived(QString request_qstr) {
  std::string request = request_qstr.toStdString();
  std::string delim = "&code=";
  std::string token = request.substr(request.find(delim));
  token = token.substr(0, token.find(" "));
  token = token.substr(token.find("="));
  token = token.substr(1);
  const char *token_c = token.c_str();
  //this->authsite->hide();
  cpr::Parameter code{"code", token_c};
  std::string post_body = "grant_type=authorization_code&code=" + token + "&redirect_uri=http://localhost:8800";
  cpr::Payload params{{"grant_type", "authorization_code"}, {"code", token_c}, {"redirect_uri", "http://localhost:8800"}};
  this->authsite->deleteLater();
  cpr::Response resp = cpr::Post(cpr::Url{"https://www.reddit.com/api/v1/access_token"},
                                 cpr::Header{{"User-Agent", "angel/v1.0 (by /u/Starkiller645)"}, {"Authorization", "Basic SnEwQml1VWVJcnNyM0E6"}},
                                 params);
  auto response_json = json::parse(resp.text);
  this->bearer_token = response_json["access_token"];
  this->refresh_token = response_json["refresh_token"];
  this->mainWidget->deleteLater();
  this->mainWidget = new QWidget();
  cpr::Response get_credentials = cpr::Get(cpr::Url{"https://oauth.reddit.com/api/v1/me"},
                                  cpr::Header{{"User-Agent", "angel/v1.0 (by /u/Starkiller645)"}, {"Authorization", "bearer " + this->bearer_token}}
                                  );

  auto get_credentials_json = json::parse(get_credentials.text);
  std::string get_uname_string = get_credentials_json["subreddit"]["display_name"];
  get_uname_string = get_uname_string.substr(2);

  nlohmann::json cred_json;
  cred_json["client_id"] = "Jq0BiuUeIrsr3A";
  cred_json["user_agent"] = "angel/v1.0 (by /u/Starkiller645)";
  cred_json["refresh_token"] = this->refresh_token;
  cred_json["access_token"] = this->bearer_token;

  QThreadPool *write_thread = new QThreadPool();
  std::string filename = std::getenv("HOME");
  filename += "/.config/angel.json";
  filejson::JsonWrite *write_json = new filejson::JsonWrite(cred_json, filename);
  write_json->run();

  filejson::JsonRead *read_json = new filejson::JsonRead(filename);
  QObject::connect(read_json, &filejson::JsonRead::onJsonRead, this, [=](nlohmann::json json_read){this->conf_json = json_read;});
  read_json->run();

  QWidget *labels_container = new QWidget();
  const QIcon next_icon(":/images/next.svg");
  transparentbutton::TransparentButton *next_button = new transparentbutton::TransparentButton(next_icon, QSize(128, 128), 1800);
  QWidget *spacer = new QWidget();
  QVBoxLayout *labels_layout = new QVBoxLayout();
  QLabel *subtitle = new QLabel("Welcome,");
  QLabel *label = new QLabel();
  const QString cred_qstring(get_uname_string.c_str());

  label->setText(cred_qstring);
  label->setStyleSheet("font-size: 60px; font-weight: bold;");
  label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

  spacer->setMinimumHeight(250);
  next_button->setStyleSheet("background-color: rgba(255, 255, 255, 0); border: 0px;");
  labels_layout->addWidget(spacer);
  subtitle->setStyleSheet("font-size: 20px");
  subtitle->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  spacer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
  labels_layout->addWidget(subtitle);
  labels_layout->addWidget(label);
  labels_layout->addWidget(next_button);
  labels_layout->addStretch();
  labels_layout->setAlignment(Qt::AlignHCenter);
  labels_container->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  labels_container->setLayout(labels_layout);

  QPropertyAnimation *move_up_anim = new QPropertyAnimation(spacer, "minimumHeight");

  move_up_anim->setDuration(1000);
  move_up_anim->setStartValue(250);
  move_up_anim->setEndValue(160);
  move_up_anim->setEasingCurve(QEasingCurve::InOutQuad);

  QTimer *timer_animation = new QTimer(this);
  timer_animation->setSingleShot(true);
  connect(timer_animation, &QTimer::timeout, this, [=]() {move_up_anim->start();});
  timer_animation->start(800);
  next_button->doAnimation();

  this->mainLayout->deleteLater();
  this->mainLayout = new QVBoxLayout();
  this->mainLayout->addWidget(labels_container);
  this->mainLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  this->mainWidget->setLayout(this->mainLayout);
  setCentralWidget(this->mainWidget);
  QObject::connect(next_button, &QPushButton::clicked, this, &MainWindow::doSetupMainUI);
}

void MainWindow::doSetupMainUI() {
  this->mainWidget->deleteLater();

  this->ui = new mainui::MainUI();
  this->setCentralWidget(this->ui);
}
