#include "mainui.h"
#include "subredditwidget.h"
#include "submissionwidget.h"
#include "filejson.h"
#include "redditasyncrequest.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QTimer>
#include <QIcon>
#include <QFile>
#include <QThreadPool>
#include <iostream>
#include <string>
#include <QLineEdit>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <curl/curl.h>

mainui::MainUI::MainUI() {
  //Top level primary widgets
  this->sideBarWidget = new QWidget();
  this->subredditWidget = new subredditwidget::SubredditWidget;
  std::string subName = "<b>frontpage</b>";
  this->subredditWidget->setName(subName);
  this->subredditWidget->setOnlineSubscribers(2041);
  this->subredditWidget->setSubscribers(240141);
  this->submissionsWidget = new QWidget();
  this->toolBarWidget = new QWidget();
  this->toolBarLayout = new QVBoxLayout();
  this->subListWidget = new QWidget();
  this->displayWidget = new QWidget();
  this->topBarWidget = new QWidget();
  this->sideBarLayout = new QVBoxLayout();
  this->displayLayout = new QVBoxLayout();
  this->viewLayout = new QVBoxLayout();

  // Nested inside topBarWidget
  this->topBarLayout = new QVBoxLayout();
  this->topBarInfoWidget = new QWidget();
  this->topBarInfoLayout = new QHBoxLayout();
  this->upvoteInfoWidget = new QLabel();
  this->titleWidget = new QLabel();
  this->authorInfoWidget = new QLabel();
  this->subredditInfoWidget = new QLabel();
  this->subredditIconWidget = new QLabel();

  this->subScroll = new QScrollArea();
  this->viewWidget = new QWidget();
  this->subListLayout = new QVBoxLayout();
  this->bodyWidget = new QWidget();
  this->bottomBarWidget = new QWidget();
  this->goButton = new QPushButton("Go");
  this->searchTextEdit = new QLineEdit();

  this->mainLayout = new QHBoxLayout();
  this->submissionsLayout = new QHBoxLayout();

  this->switchSub("frontpage");

  this->subredditWidget->setStyleSheet("background-color: #232629;");
  this->submissionsWidget->setStyleSheet("background-color: #232629; border: 0px;");
  this->bodyWidget->setStyleSheet("background-color: #ffffff;");
  this->topBarWidget->setStyleSheet("background-color: #eff0f1;");
  this->bottomBarWidget->setStyleSheet("background-color: #eff0f1;");
  this->subredditWidget->setMaximumHeight(120);
  this->subListWidget->setStyleSheet("background-color: #232629; border: 0px;");

  this->subListWidget->setLayout(this->subListLayout);
  this->subListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  this->toolBarWidget->setStyleSheet("background-color: #31363b;");
  this->toolBarWidget->setMaximumWidth(60);
  this->toolBarWidget->setLayout(this->toolBarLayout);
  this->subScroll->setMinimumWidth(440);
  this->subScroll->setWidgetResizable(true);
  this->subScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->bottomBarWidget->setMaximumHeight(60);
  this->topBarWidget->setFixedHeight(120);
  this->topBarWidget->setLayout(this->topBarLayout);
  this->topBarLayout->addWidget(this->titleWidget);
  this->topBarLayout->addWidget(this->topBarInfoWidget);
  this->topBarInfoWidget->setLayout(this->topBarInfoLayout);
  this->topBarInfoLayout->addWidget(this->upvoteInfoWidget);
  this->topBarInfoLayout->addWidget(this->authorInfoWidget);
  this->topBarInfoLayout->addWidget(this->subredditIconWidget);
  this->topBarInfoLayout->addWidget(this->subredditInfoWidget);
  this->subredditInfoWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
  this->titleWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  this->topBarInfoLayout->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  this->titleWidget->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

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

  QIcon searchIcon(":/images/icon-search.svg");
  this->searchButton = new QPushButton();
  this->searchButton->setIcon(searchIcon);
  this->searchButton->setIconSize(QSize(36, 36));
  this->searchButton->setStyleSheet("background-color: rgba(255, 255, 255, 0); border: 0px; padding: 7px;");
  this->toolBarLayout->setAlignment(Qt::AlignCenter | Qt::AlignTop);
  QObject::connect(this->searchButton, &QPushButton::clicked, this, [=](){this->toggleSideBar(mainui::SidebarButton::Search);});

  /*QTimer *timer_animation = new QTimer(this);
  timer_animation->setSingleShot(true);
  QObject::connect(timer_animation, &QTimer::timeout, this, [=](){mainui::MainUI::toggleSideBar(mainui::SidebarButton::Search);});
  timer_animation->start(3000);

  QTimer *timer_animation2 = new QTimer(this);
  timer_animation2->setSingleShot(true);
  QObject::connect(timer_animation2, &QTimer::timeout, [=](){mainui::MainUI::toggleSideBar(mainui::SidebarButton::Search);});
  timer_animation2->start(8000);*/
}

void mainui::MainUI::toggleSideBar(mainui::SidebarButton type = mainui::SidebarButton::Search) {

  QLayoutItem *child;
  while ((child = this->toolBarLayout->takeAt(0)) != nullptr) {
      delete child->widget();
      delete child;
  };

  if(this->toolBarWidget->maximumWidth() > 60 && this->subScroll->minimumWidth() < 440) {
    QPropertyAnimation *toolbar_anim = new QPropertyAnimation(this->toolBarWidget, "maximumWidth");
    toolbar_anim->setStartValue(this->toolBarWidget->maximumWidth());
    toolbar_anim->setEndValue(60);
    toolbar_anim->setDuration(600);

    QPropertyAnimation *sublist_anim = new QPropertyAnimation(this->subScroll, "minimumWidth");
    sublist_anim->setStartValue(this->subScroll->minimumWidth());
    sublist_anim->setEndValue(440);
    sublist_anim->setDuration(600);

    this->searchButton = new QPushButton();
    QIcon searchIcon(":/images/icon-search.svg");
    this->searchButton->setIcon(searchIcon);
    this->searchButton->setIconSize(QSize(36, 36));
    this->searchButton->setStyleSheet("background-color: rgba(255, 255, 255, 0); border: 0px; padding: 7px;");
    this->toolBarLayout->addWidget(this->searchButton);
    QObject::connect(this->searchButton, &QPushButton::clicked, this, [=](){this->toggleSideBar(mainui::SidebarButton::Search);});

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

    switch(type) {
      case mainui::SidebarButton::Search:
        QWidget *searchWidget = new QWidget();
        QHBoxLayout *searchLayout = new QHBoxLayout();
        QLabel *searchLabel = new QLabel("Search");
        this->searchTextEdit = new QLineEdit();
        this->goButton = new QPushButton("Go");
        this->searchTextEdit->setStyleSheet("color: white; background-color: #232629; border: 1px solid #31363b;");
        this->searchTextEdit->setFixedHeight(25);
        this->goButton->setStyleSheet("color: white; background-color: #232629; border: 1px solid #31363b;");
        QObject::connect(this->goButton, &QPushButton::clicked, this, [=](){this->switchSub(this->searchTextEdit->text().toStdString());});
        searchLabel->setStyleSheet("color: white; font-size: 40px; font-weight: bold;");
        searchLayout->addWidget(this->searchTextEdit);
        searchLayout->addWidget(this->goButton);
        searchWidget->setLayout(searchLayout);
        this->toolBarLayout->addWidget(searchLabel);
        this->toolBarLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);
        this->toolBarLayout->addWidget(searchWidget);
    };

    sublist_anim->start();
    toolbar_anim->start();

  }
}

void mainui::MainUI::switchSub(std::string sub) {

  if(this->toolBarWidget->maximumWidth() > 60 && this->subScroll->minimumWidth() < 440) {
    this->toggleSideBar();
  };

  this->subredditWidget->setName(sub);
  QThreadPool *threadpool = new QThreadPool();
  std::string filename = "/home/jacob/.config/angel.json";
  filejson::JsonRead *json = new filejson::JsonRead(filename);
  this->jsondata = json->runSynced();
  cpr::Url frontpage_url{"https://oauth.reddit.com"};
  cpr::Url url{"https://oauth.reddit.com/r/" + sub + "/hot"};
  cpr::Url about_url{"https://oauth.reddit.com/r/" + sub + "/about"};
  std::string authparam = jsondata["access_token"];
  authparam = "bearer " + authparam;
  this->headers = cpr::Header{{"Authorization", authparam.c_str()}, {"User-Agent", "angel/v1.0 (by /u/Starkiller645)"}, {"limit", "100"}};
  cpr::Response response = cpr::Get(url, this->headers);
  cpr::Response about_sub = cpr::Get(about_url, this->headers);
  this->json_about = nlohmann::json::parse(about_sub.text);
  if(sub != "frontpage") {
    this->subredditWidget->setSubscribers(json_about["data"]["subscribers"]);
    this->subredditWidget->setOnlineSubscribers(json_about["data"]["active_user_count"]);
  } else {
    this->subredditWidget->setSubscribers(1);
    this->subredditWidget->setOnlineSubscribers(1);
  };
  cpr::Response frontpage_response = cpr::Get(frontpage_url, this->headers);
  this->json_response = nlohmann::json::parse(response.text);
  auto json_frontpage = nlohmann::json::parse(frontpage_response.text);
  std::cout << "First post: " << json_response["data"]["children"][0]["data"]["title"] << std::endl;

  std::vector<submissionwidget::SubmissionWidget *> submission_widget_list;

  QLayoutItem *child;
  while ((child = this->subListLayout->takeAt(0)) != nullptr) {
      delete child->widget();
      delete child;
  };

  this->submission_json_list.clear();

  for(int i = 0; i < 25; i++) {
    if(sub == "frontpage") {
      nlohmann::json temp_json = json_frontpage["data"]["children"][i];
      this->submission_json_list.push_back(temp_json);
    } else {
      nlohmann::json temp_json = json_response["data"]["children"][i];
      this->submission_json_list.push_back(temp_json);
    }
  }

  for(int i = 0; i < this->submission_json_list.size(); i++) {
    int ups = this->submission_json_list[i]["data"]["ups"];
    int downs = this->submission_json_list[i]["data"]["downs"];
    signed int score = ups - downs;
    submissionwidget::SubmissionWidget *temp_widget = new submissionwidget::SubmissionWidget(
      this->submission_json_list.at(i)["data"]["title"],
      this->submission_json_list.at(i)["data"]["selftext"],
      this->submission_json_list.at(i)["data"]["author"],
      score,
      i,
      submissionwidget::Text
    );

    temp_widget->setMinimumWidth(100);

    submission_widget_list.push_back(temp_widget);
    subListLayout->addWidget(temp_widget);
  }
  std::cout << this->json_about["data"]["icon_img"] << std::endl;
  std::cout << this->json_about.dump() << std::endl;
  std::cout << std::string(this->json_about["data"]["icon_img"]).c_str() << std::endl;
  this->view(1);
}

void mainui::MainUI::view(int id) {
  nlohmann::json temp_json = this->submission_json_list[id];
  this->titleWidget->setText(QString(std::string(temp_json["data"]["title"]).c_str()));
  this->titleWidget->setStyleSheet("font-weight: bold; font-size: 40px; padding: 8px 3px;");
  signed int score = int(temp_json["data"]["ups"]) - int(temp_json["data"]["downs"]);
  this->topBarInfoWidget->setStyleSheet("padding: 8px 3px;");
  this->upvoteInfoWidget->setText(QString(std::to_string(score).c_str()));
  this->upvoteInfoWidget->setStyleSheet("color: #ff4500; font-weight: bold;");
  std::string author = "u/";
  author += std::string(temp_json["data"]["author"]);
  this->authorInfoWidget->setText(QString(author.c_str()));
  this->authorInfoWidget->setStyleSheet("color: #0dd3bb;");
  this->subredditInfoWidget->setStyleSheet("font-weight: bold; font-size: 30px;");
  cpr::Url icon_url(std::string(this->json_about["data"]["icon_img"]));
  cpr::Header headers = this->headers;
  CURL *curl;
  FILE *fp;
  CURLcode res;
  char buffer[CURL_ERROR_SIZE];
  std::string url_prev = std::string(this->json_about["data"]["icon_img"]);
  char outfilename[FILENAME_MAX] = "/opt/angel-reddit/temp/.subimg.png";
  curl = curl_easy_init();
  if (curl) {
      fp = fopen(outfilename,"wb");
      curl_easy_setopt(curl, CURLOPT_URL, url_prev.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
      curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, buffer);
      res = curl_easy_perform(curl);
      std::cout << res << std::endl;
      /* always cleanup */
      curl_easy_cleanup(curl);
      fclose(fp);
      std::cout << buffer << std::endl;
  }
  this->subredditIconWidget->setPixmap(QPixmap("/opt/angel-reddit/temp/.subimg.png").scaled(30, 30));
  this->subredditInfoWidget->setText(QString(std::string(this->json_about["data"]["display_name"]).c_str()));
}

size_t mainui::MainUI::writeBinaryData(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
