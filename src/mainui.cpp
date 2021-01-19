#include "mainui.h"
#include "subredditwidget.h"
#include "submissionwidget.h"
#include "filejson.h"
#include "redditasyncrequest.h"
#include "authworker.h"
#include <maddy/parser.h>

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMetaType>
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

  this->imageThread = new QThread();
  this->imageWorker = new authworker::AuthorisationWorker();

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
  this->bodyLayout = new QVBoxLayout();

  this->switchSub("frontpage");

  this->subredditWidget->setStyleSheet("background-color: #232629;");
  this->submissionsWidget->setStyleSheet("background-color: #232629; border: 0px;");
  //this->bodyWidget->setStyleSheet("background-color: #ffffff;");
  this->topBarWidget->setStyleSheet("background-color: #eff0f1;");
  this->bottomBarWidget->setStyleSheet("background-color: #eff0f1;");
  this->subredditWidget->setMaximumHeight(120);
  this->subListWidget->setStyleSheet("background-color: #232629; border: 0px;");

  // Widgets under this->topBarWidget
  this->subListWidget->setLayout(this->subListLayout);
  this->subListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  this->toolBarWidget->setStyleSheet("background-color: #31363b;");
  this->toolBarWidget->setMaximumWidth(60);
  this->toolBarWidget->setLayout(this->toolBarLayout);
  this->subScroll->setMinimumWidth(440);
  this->subScroll->setWidgetResizable(true);
  this->subScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->bottomBarWidget->setFixedHeight(60);
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

  // Widgets under this->bodyWidget
  this->bodyWidget->setLayout(this->bodyLayout);

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
  std::cout << "Switching to " << sub << std::endl;
  this->subredditWidget->setName(sub);
  QThread *switchSubThread = new QThread();
  std::string filename = std::string(std::getenv("HOME")) + std::string("/.config/angel.json");
  filejson::JsonRead *json = new filejson::JsonRead(filename);
  this->jsondata = json->runSynced();
  authworker::AuthorisationWorker *switchSubWorker = new authworker::AuthorisationWorker(sub);
  switchSubWorker->setObjectName("Switch-Sub Thread Worker");
  QObject::connect(switchSubThread, &QThread::started, switchSubWorker, &authworker::AuthorisationWorker::switchSub);
  QObject::connect(switchSubWorker, &authworker::AuthorisationWorker::onSwitchSubComplete, this, &mainui::MainUI::onSwitchSubComplete);
  switchSubWorker->moveToThread(switchSubThread);
  switchSubThread->start();
}

void mainui::MainUI::onSwitchSubComplete(std::string sub, nlohmann::json response_json, nlohmann::json json_about, nlohmann::json frontpage_json) {
  std::string url_prev = std::string(json_about["data"]["icon_img"]);
  this->downloadUrlData(url_prev, std::string("/opt/angel-reddit/temp/.subimg.png"));
  if(sub != "frontpage") {
    this->subredditWidget->setSubscribers(json_about["data"]["subscribers"]);
    this->subredditWidget->setOnlineSubscribers(json_about["data"]["active_user_count"]);
  } else {
    this->subredditWidget->setSubscribers(1);
    this->subredditWidget->setOnlineSubscribers(1);
  };
    this->subredditWidget->setIcon(QPixmap("/opt/angel-reddit/.subimg.png"));
    this->json_response = response_json;
    this->json_frontpage = frontpage_json;
    this->json_about = json_about;
    std::cout << url_prev << std::endl;
    std::cout << "Parsing response" << std::endl;
    std::cout << "First post: " << this->json_response["data"]["children"][0]["data"]["title"] << std::endl;

  std::vector<submissionwidget::SubmissionWidget *> submission_widget_list;

  QLayoutItem *child;
  while ((child = this->subListLayout->takeAt(0)) != nullptr) {
      delete child->widget();
      delete child;
  };

  this->submission_json_list.clear();

  for(int i = 0; i < 25; i++) {
    if(sub == "frontpage") {
      nlohmann::json temp_json = this->json_frontpage["data"]["children"][i];
      this->submission_json_list.push_back(temp_json);
    } else {
      nlohmann::json temp_json = this->json_response["data"]["children"][i];
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
    std::cout << i << std::endl;
    temp_widget->setMinimumWidth(100);

    submission_widget_list.push_back(temp_widget);
    subListLayout->addWidget(temp_widget);
  }
  std::cout << this->json_about["data"]["icon_img"] << std::endl;
  std::cout << std::string(this->json_about["data"]["icon_img"]).c_str() << std::endl;
  this->view(1);

  for(int i = 0; i < submission_widget_list.size(); i++) {
    std::cout << "[DBG] Submission ID: " << submission_widget_list[i]->index << std::endl;;
    QObject::connect(submission_widget_list[i], &QPushButton::clicked, [=](){this->view(submission_widget_list[i]->index);});
  };
}

void mainui::MainUI::view(int id) {
  QLayoutItem *child;
  while ((child = this->bodyLayout->takeAt(0)) != nullptr) {
      delete child->widget();
      delete child;
  };

  nlohmann::json temp_json = this->submission_json_list[id];
  this->titleWidget->setStyleSheet("font-weight: bold; font-size: 40px; padding: 2px 8px;");
  signed int score = int(temp_json["data"]["ups"]) - int(temp_json["data"]["downs"]);
  this->topBarInfoWidget->setStyleSheet("padding: 8px 3px;");
  this->upvoteInfoWidget->setText(QString(std::to_string(score).c_str()));
  if(std::getenv("OS") == "OSX") {
    this->upvoteInfoWidget->setStyleSheet("color: #ff4500; font-weight: bold; padding: 0px;");
  } else {
    this->upvoteInfoWidget->setStyleSheet("color: #ff4500; font-weight: bold;");
  }
  std::string author = "u/";
  std::string title = temp_json["data"]["title"];
  author += std::string(temp_json["data"]["author"]);
  std::string selftext = std::string(temp_json["data"]["selftext"]);
  if(selftext == "") {
    selftext = "Image or link post";
  }

  std::stringstream markdownInput(selftext);
  std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>();
  std::string htmlOutput = parser->Parse(markdownInput);
  submissionwidget::submission_type subType;

  selftext = htmlOutput;
  std::string url = temp_json["data"]["url"];

  std::cout << url << std::endl;

  if(url.find("i.redd.it") != std::string::npos || url.find("i.imgur.com") != std::string::npos || url.find("i.reddit.com") != std::string::npos) {
    subType = submissionwidget::Image;
  } else if(url.find("v.reddit.com") != std::string::npos) {
    subType = submissionwidget::Video;
  } else if(url.find("reddit.com/r/") != std::string::npos) {
    subType = submissionwidget::Text;
  } else {
    subType = submissionwidget::Link;
  }

  if(subType == submissionwidget::Image || subType == submissionwidget::Video) {
    this->bodyLayout->setAlignment(Qt::AlignCenter);
    std::cout << "[DBG] Starting image-dl thread" << std::endl;
    this->imageThread->deleteLater();
    this->imageThread = new QThread();
    this->imageWorker->deleteLater();
    this->imageWorker = new authworker::AuthorisationWorker();
    this->imageThread->setObjectName("Image Download Worker");
    QObject::connect(this, &mainui::MainUI::doDownloadImage, this->imageWorker, &authworker::AuthorisationWorker::downloadImageFile, Qt::UniqueConnection);
    QObject::connect(this->imageWorker, &authworker::AuthorisationWorker::onDownloadImageFile, this, &mainui::MainUI::setSubmissionImage);
    this->imageWorker->moveToThread(this->imageThread);
    this->imageThread->start();
    emit mainui::MainUI::doDownloadImage(url);
  } else {
    this->bodyLayout->setAlignment(Qt::AlignLeft);
  }

  if(title.length() > 35) {
    title = title.substr(0, 35) + "...";
  }
  this->titleWidget->setText(QString(title.c_str()));
  const QString selftextQstring(selftext.c_str());
  this->authorInfoWidget->setText(QString(author.c_str()));
  this->authorInfoWidget->setStyleSheet("color: #0dd3bb;");
  this->subredditInfoWidget->setStyleSheet("font-weight: bold; padding: 0px 6px;");
  cpr::Url icon_url(std::string(this->json_about["data"]["icon_img"]));
  cpr::Header headers = this->headers;
  std::vector<std::string> selftextVector;
  std::string replacement_img_tag = "<a href=\"https://preview.redd.it";
  size_t pos = selftext.find(replacement_img_tag);
  int i = 0;

  std::cout << "Starting vector sort" << std::endl;
  std::cout << "Position: " << pos << " Size: " << selftextVector.size() << std::endl;
  while(pos < selftext.length()) {
    pos = selftext.find(replacement_img_tag);
    if(i == 0) {
      selftextVector.push_back(selftext.substr(0, pos));
    } else {
      selftextVector[i - 1] = selftextVector[i - 1].substr(0, pos);
    }
    std::cout << i << std::endl;
    selftextVector[i] = selftextVector[i - 1].substr(pos);
    i++;
  }

  std::cout << "Iterating through vector" << std::endl;

  for(int i = 0; i < selftextVector.size(); i++) {
    std::cout << "Vector position [" << i << "]: " << selftextVector[i] << std::endl;
  }

  this->subredditIconWidget->setPixmap(QPixmap("/opt/angel-reddit/temp/.subimg.png").scaled(30, 30));
  this->subredditInfoWidget->setText(QString(std::string(this->json_about["data"]["display_name"]).c_str()));
  QLabel *selfTextLabel = new QLabel(selftextQstring);
  selfTextLabel->setWordWrap(true);
  selfTextLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  selfTextLabel->setTextFormat(Qt::RichText);
  this->bodyLayout->addWidget(selfTextLabel);
}

size_t mainui::MainUI::writeBinaryData(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void mainui::MainUI::onResponseReceived(QString request_qstr) {
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
                                 cpr::Header{{"User-Agent", "linux:com.example.angel:v1.0 (by /u/Starkiller645)"}, {"Authorization", "Basic SnEwQml1VWVJcnNyM0E6"}},
                                 params);
  auto response_json = nlohmann::json::parse(resp.text);
  this->bearer_token = response_json["access_token"];
  this->refresh_token = response_json["refresh_token"];
  cpr::Response get_credentials = cpr::Get(cpr::Url{"https://oauth.reddit.com/api/v1/me"},
                                  cpr::Header{{"User-Agent", "angel/v1.0 (by /u/Starkiller645)"}, {"Authorization", "bearer " + this->bearer_token}}
                                  );

  auto get_credentials_json = nlohmann::json::parse(get_credentials.text);
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
  std::cout << "OAuth 2 access token: " << this->conf_json["access_token"] << std::endl << "OAuth 2 refresh token: " << this->conf_json["refresh_token"] << std::endl;
  this->switchSub("frontpage");
}

  std::string mainui::MainUI::downloadUrlData(std::string url, std::string filename) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char buffer[CURL_ERROR_SIZE];
    char outfilename[FILENAME_MAX];
    strcpy(outfilename, filename.c_str());
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, buffer);
        res = curl_easy_perform(curl);
        std::cout << res << std::endl;
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
      }
    return filename;
  }
  void mainui::MainUI::setSubmissionImage(std::string filename) {
    const QPixmap pixmap(filename.c_str());
    QLabel *imageLabel = new QLabel();
    QSize original_size = this->bodyWidget->size();
    QSize new_size(original_size.width() - 16, original_size.height() - 16);
    imageLabel->setPixmap(pixmap.scaled(new_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QLayoutItem *child;
    while ((child = this->bodyLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    };
    imageLabel->setStyleSheet("padding: 0px; margin: 0px");
    this->bodyWidget->setStyleSheet("margin: 0px; padding: 0px;");
    this->bodyLayout->addWidget(imageLabel);
    this->imageThread->quit();
  }

Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(nlohmann::json);
int stdStringMetaTypeID = qRegisterMetaType<std::string>();
int nlohmannJsonMetaTypeID = qRegisterMetaType<nlohmann::json>();
