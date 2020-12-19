#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "subredditwidget.h"

namespace mainui {
  class MainUI : public QWidget {
    QWidget *mainWidget;
    QHBoxLayout *mainLayout;
    QWidget *sideBarWidget;
    QVBoxLayout *sideBarLayout;
    subredditwidget::SubredditWidget *subredditWidget;
    QWidget *submissionsWidget;
    QHBoxLayout *submissionsLayout;
    QWidget *toolBarWidget;
    QWidget *subListWidget;
    QWidget *displayWidget;
    QVBoxLayout *displayLayout;
    QWidget *topBarWidget;
    QWidget *viewWidget;
    QVBoxLayout *viewLayout;
    QWidget *bodyWidget;
    QWidget *bottomBarWidget;
  public:
    MainUI();
    void setup();
  public slots:
    void toggleSideBar();
  };
}

#endif
