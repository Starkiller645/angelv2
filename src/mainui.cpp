#include "mainui.h"
#include "subredditwidget.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QTimer>
#include <iostream>
#include <string>

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
  this->viewWidget = new QWidget();
  this->bodyWidget = new QWidget();
  this->bottomBarWidget = new QWidget();

  this->mainLayout = new QHBoxLayout();
  this->submissionsLayout = new QHBoxLayout();

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
  this->toolBarWidget->setStyleSheet("background-color: #31363b;");
  this->toolBarWidget->setMaximumWidth(60);
  this->subListWidget->setMinimumWidth(440);
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
  this->submissionsLayout->addWidget(this->subListWidget);
  this->submissionsWidget->setLayout(this->submissionsLayout);
  this->subListWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
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
  timer_animation2->start(3300);
}

void mainui::MainUI::toggleSideBar() {
  if(this->toolBarWidget->maximumWidth() > 60 && this->subListWidget->minimumWidth() < 440) {
    QPropertyAnimation *toolbar_anim = new QPropertyAnimation(this->toolBarWidget, "maximumWidth");
    toolbar_anim->setStartValue(this->toolBarWidget->maximumWidth());
    toolbar_anim->setEndValue(60);
    toolbar_anim->setDuration(600);

    QPropertyAnimation *sublist_anim = new QPropertyAnimation(this->subListWidget, "minimumWidth");
    sublist_anim->setStartValue(this->subListWidget->minimumWidth());
    sublist_anim->setEndValue(440);
    sublist_anim->setDuration(600);

    sublist_anim->start();
    toolbar_anim->start();

  } else {

    QPropertyAnimation *toolbar_anim = new QPropertyAnimation(this->toolBarWidget, "maximumWidth");
    toolbar_anim->setStartValue(this->toolBarWidget->maximumWidth());
    toolbar_anim->setEndValue(500);
    toolbar_anim->setDuration(600);

    QPropertyAnimation *sublist_anim = new QPropertyAnimation(this->subListWidget, "minimumWidth");
    sublist_anim->setStartValue(this->subListWidget->minimumWidth());
    sublist_anim->setEndValue(0);
    sublist_anim->setDuration(600);

    sublist_anim->start();
    toolbar_anim->start();

  }
}
