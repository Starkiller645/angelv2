#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QLineEdit>
#include <QWebEngineView>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <curl/curl.h>

#include "subredditwidget.h"
#include "submissionwidget.h"
#include "authworker.h"

namespace mainui {
  enum SidebarButton {Search, Self, Frontpage, Settings};
  class MainUI : public QWidget {
    Q_OBJECT
    QWebEngineView *authsite;
    QWidget *mainWidget;
    QHBoxLayout *mainLayout;
    int inc;
    QWidget *sideBarWidget;
    QVBoxLayout *sideBarLayout;
    subredditwidget::SubredditWidget *subredditWidget;
    QScrollArea *subScroll;
    QWidget *submissionsWidget;
    QHBoxLayout *submissionsLayout;
    QWidget *toolBarWidget;
    QVBoxLayout *toolBarLayout;
    QWidget *subListWidget;
    QWidget *displayWidget;
    QVBoxLayout *displayLayout;
    QWidget *topBarWidget;
    QWidget *viewWidget;
    QVBoxLayout *viewLayout;
    QWidget *bodyWidget;
    QVBoxLayout *bodyLayout;
    QLabel *selftextWidget;
    QWidget *bottomBarWidget;
    QVBoxLayout *subListLayout;
    QVBoxLayout *topBarLayout;
    QWidget *topBarInfoWidget;
    QHBoxLayout *topBarInfoLayout;
    QLabel *upvoteInfoWidget;
    QLabel *titleWidget;
    QLabel *authorInfoWidget;
    QLabel *subredditInfoWidget;
    QLabel *subredditIconWidget;
    QLabel *loadingLabel;
    QVBoxLayout *searchResultsLayout;
    QTimer *searchTimeoutTimer;
    nlohmann::json jsondata;
    nlohmann::json json_response;
    nlohmann::json json_about;
    nlohmann::json json_frontpage;
    bool subUpdateFlag;
    std::vector<submissionwidget::SubmissionWidget *> submission_widget_list;
    std::string bearer_token;
    std::string refresh_token;
    std::string currentSub;
    std::string sub;
    std::string before;
    nlohmann::json conf_json;
    cpr::Header headers;
    std::vector<nlohmann::json> submission_json_list;

    QPushButton *searchButton;
    QPushButton *goButton;
    QLineEdit *searchTextEdit;
    QThread *imageThread;
    bool isSidebarOut;
    authworker::AuthorisationWorker *imageWorker;
  signals:
    void doDownloadImage(std::string);
  public:
    MainUI();
    void setup();
    std::string downloadUrlData(std::string, std::string);
    void toggleSideBar(SidebarButton);
    void switchSub(std::string);
    void view(int);
    void onResponseReceived(QString);
    void doGetSearchSubs();
    void updateSubList(nlohmann::json);
    static size_t writeBinaryData(void*, size_t, size_t, FILE*);
  public slots:
    void onSearchTextUpdate();
    void setSubmissionImage(std::string);
    void onSlideChange(int);
    void onSwitchSubComplete(std::string, nlohmann::json, nlohmann::json, nlohmann::json, std::string);
  };
}

#endif
