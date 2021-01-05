#include "mainui.h"
#include "subredditwidget.h"
#include "submissionwidget.h"
#include "filejson.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QTimer>
#include <QThreadPool>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

mainui::MainUI::MainUI() {
  this->sideBarWidget = new QWidget();
  this->subredditWidget = new subredditwidget::SubredditWidget;
  std::string subName = "<b>frontpage</b>";
  this->subredditWidget->setName(subName);
  this->subredditWidget->setOnlineSubscribers(2041);
  this->subredditWidget->setSubscribers(240141);
  this->submissionsWidget = new QWidget();
  this->toolBarWidget = new QWidget();
  this->subListWidget = new QWidget();
  this->displayWidget = new QWidget();
  this->topBarWidget = new QWidget();
  this->subScroll = new QScrollArea();
  this->viewWidget = new QWidget();
  this->bodyWidget = new QWidget();
  this->bottomBarWidget = new QWidget();

  this->mainLayout = new QHBoxLayout();
  this->submissionsLayout = new QHBoxLayout();


  QThreadPool *threadpool = new QThreadPool();
  std::string filename = "/home/jacob/.config/angel.json";
  filejson::JsonRead *json = new filejson::JsonRead(filename);
  this->jsondata = json->runSynced();
  cpr::Url url{"https://oauth.reddit.com/r/linux/hot"};
  std::cout << this->jsondata.dump() << std::endl;
  std::string authparam = jsondata["access_token"];
  authparam = "bearer " + authparam;
  std::cout << authparam << std::endl;
  cpr::Header headers{{"Authorization", authparam.c_str()}, {"User-Agent", "angel/v1.0 (by /u/Starkiller645)"}, {"limit", "100"}};
  cpr::Response response = cpr::Get(url, headers);
  std::cout << response.header["g"];
  std::cout << response.status_code;
  auto json_response = nlohmann::json::parse(response.text);
  std::cout << "First post: " << json_response["data"]["children"][0]["data"]["title"] << std::endl;

  this->sideBarLayout = new QVBoxLayout();
  this->displayLayout = new QVBoxLayout();
  this->viewLayout = new QVBoxLayout();

  this->subredditWidget->setStyleSheet("background-color: #232629;");
  this->submissionsWidget->setStyleSheet("background-color: #232629;");
  this->bodyWidget->setStyleSheet("background-color: #ffffff;");
  this->topBarWidget->setStyleSheet("background-color: #eff0f1;");
  this->bottomBarWidget->setStyleSheet("background-color: #eff0f1;");
  this->subredditWidget->setMaximumHeight(120);
  this->subListWidget->setStyleSheet("background-color: #232629;");
  QVBoxLayout *subListLayout = new QVBoxLayout;
  std::string test_title = json_response["data"]["children"][0]["data"]["title"];
  std::string test_body = "This is a test of the submission widget. Lorem ipsum dolor sit amet...";
  std::string test_author = json_response["data"]["children"][0]["data"]["author_fullname"];
  std::string test_id = "t3_skofenwio";
  int test_score = 30144;

  std::vector<nlohmann::json> submission_json_list;
  std::vector<submissionwidget::SubmissionWidget *> submission_widget_list;

  for(int i = 0; i < 25; i++) {
    nlohmann::json temp_json = json_response["data"]["children"][i];
    submission_json_list.push_back(temp_json);
    std::cout << "Got submission: " << i << ": " << submission_json_list[i]["data"]["title"] << std::endl;
  }

  for(int i = 0; i < submission_json_list.size(); i++) {
    std::cout << "[DBG] Getting temp score..." << std::endl;
    std::cout << submission_json_list.at(i).dump() << std::endl;
    /*int temp_score = std::stoi(
      std::string(submission_json_list.at(i)["data"]["ups"])) - std::stoi(
      std::string(submission_json_list.at(i)["data"]["downs"]));*/
    std::cout << "[DBG] Done\n[DBG] Setting up temp widget" << std::endl;
    submissionwidget::SubmissionWidget *temp_widget = new submissionwidget::SubmissionWidget(
      submission_json_list.at(i)["data"]["title"],
      submission_json_list.at(i)["data"]["selftext"],
      submission_json_list.at(i)["data"]["author"],
      0,
      std::to_string(i),
      submissionwidget::Text
    );
    std::cout << "[DBG] Done\n[DBG] Adding widget to list and layout" << std::endl;

    temp_widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    temp_widget->setFixedHeight(150);
    temp_widget->setMinimumWidth(100);

    submission_widget_list.push_back(temp_widget);
    subListLayout->addWidget(temp_widget);
  }

  this->subListWidget->setLayout(subListLayout);
  this->subListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  this->toolBarWidget->setStyleSheet("background-color: #31363b;");
  this->toolBarWidget->setMaximumWidth(60);
  this->subScroll->setMinimumWidth(440);
  this->subScroll->setWidgetResizable(true);
  this->subScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->bottomBarWidget->setMaximumHeight(60);
  this->topBarWidget->setFixedHeight(120);

  this->mainLayout->setMargin(0);
  this->mainLayout->setSpacing(0);
  this->sideBarLayout->setMargin(0);
  this->displayLayout->setSpacing(0);
  this->viewLayout->setSpacing(0);
  this->viewLayout->setMargin(0);
  this->sideBarLayout->setSpacing(0);
  this->submissionsLayout->setSpacing(0);
  this->submissionsLayout->setMargin(0);
  this->displayLayout->setMargin(0);
  this->displayLayout->setSpacing(0);
  this->viewLayout->setMargin(0);
  this->submissionsLayout->addWidget(this->toolBarWidget);
  this->subScroll->setWidget(this->subListWidget);
  this->submissionsLayout->addWidget(this->subScroll);
  this->submissionsWidget->setLayout(this->submissionsLayout);
  this->subScroll->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
  this->sideBarLayout->addWidget(this->subredditWidget);
  this->sideBarLayout->addWidget(this->submissionsWidget);
  this->sideBarWidget->setFixedWidth(500);
  this->sideBarWidget->setLayout(this->sideBarLayout);

  this->viewLayout->addWidget(this->bodyWidget);
  this->viewLayout->addWidget(this->bottomBarWidget);
  this->viewWidget->setLayout(this->viewLayout);
  this->displayLayout->addWidget(this->topBarWidget);
  this->displayLayout->addWidget(this->viewWidget);
  this->displayWidget->setLayout(this->displayLayout);

  this->mainLayout->addWidget(this->sideBarWidget);
  this->mainLayout->addWidget(this->displayWidget);
  this->setLayout(this->mainLayout);

  QTimer *timer_animation = new QTimer(this);
  timer_animation->setSingleShot(true);
  QObject::connect(timer_animation, &QTimer::timeout, this, &mainui::MainUI::toggleSideBar);
  timer_animation->start(3000);

  QTimer *timer_animation2 = new QTimer(this);
  timer_animation2->setSingleShot(true);
  QObject::connect(timer_animation2, &QTimer::timeout, this, &mainui::MainUI::toggleSideBar);
  timer_animation2->start(4500);
}

void mainui::MainUI::toggleSideBar() {
  if(this->toolBarWidget->maximumWidth() > 60 && this->subScroll->minimumWidth() < 440) {
    QPropertyAnimation *toolbar_anim = new QPropertyAnimation(this->toolBarWidget, "maximumWidth");
    toolbar_anim->setStartValue(this->toolBarWidget->maximumWidth());
    toolbar_anim->setEndValue(60);
    toolbar_anim->setDuration(600);

    QPropertyAnimation *sublist_anim = new QPropertyAnimation(this->subScroll, "minimumWidth");
    sublist_anim->setStartValue(this->subScroll->minimumWidth());
    sublist_anim->setEndValue(440);
    sublist_anim->setDuration(600);

    sublist_anim->start();
    toolbar_anim->start();

  } else {

    QPropertyAnimation *toolbar_anim = new QPropertyAnimation(this->toolBarWidget, "maximumWidth");
    toolbar_anim->setStartValue(this->toolBarWidget->maximumWidth());
    toolbar_anim->setEndValue(500);
    toolbar_anim->setDuration(600);

    QPropertyAnimation *sublist_anim = new QPropertyAnimation(this->subScroll, "minimumWidth");
    sublist_anim->setStartValue(this->subScroll->minimumWidth());
    sublist_anim->setEndValue(0);
    sublist_anim->setDuration(600);

    sublist_anim->start();
    toolbar_anim->start();

  }
}
