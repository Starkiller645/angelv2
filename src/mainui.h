#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QLineEdit>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <curl/curl.h>

#include "subredditwidget.h"

namespace mainui {
  enum SidebarButton {Search, Self, Frontpage, Settings};
  class MainUI : public QWidget {
    QWidget *mainWidget;
    QHBoxLayout *mainLayout;
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
    nlohmann::json jsondata;
    nlohmann::json json_response;
    nlohmann::json json_about;
    cpr::Header headers;
    std::vector<nlohmann::json> submission_json_list;

    QPushButton *searchButton;
    QPushButton *goButton;
    QLineEdit *searchTextEdit;
  public:
    MainUI();
    void setup();
    void toggleSideBar(SidebarButton);
    void switchSub(std::string);
    void view(int);
    static size_t writeBinaryData(void*, size_t, size_t, FILE*);
  };
}

#endif
