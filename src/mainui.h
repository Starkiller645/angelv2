#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QLineEdit>
#include <nlohmann/json.hpp>

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
    nlohmann::json jsondata;

    QPushButton *searchButton;
    QPushButton *goButton;
    QLineEdit *searchTextEdit;
  public:
    MainUI();
    void setup();
    void toggleSideBar(SidebarButton);
    void switchSub(std::string);
  };
}

#endif
