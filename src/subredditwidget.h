#ifndef SUBREDDIT_WIDGET_H
#define SUBREDDIT_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <waitingspinnerwidget.h>

namespace subredditwidget {
  class SubredditWidget : public QWidget {
    WaitingSpinnerWidget *spinner;
    QVBoxLayout *sub_main_layout;
    QHBoxLayout *sub_title_layout;
    QHBoxLayout *sub_subtext_layout;
    QWidget *sub_title_widget;
    QWidget *sub_subtext_widget;
    QWidget *spinnerContainer;
    QVBoxLayout *spinnerContainerLayout;
    QLabel *sub_title;
    QLabel *sub_subscribers;
    QLabel *sub_online;
  public:
    SubredditWidget();
    QLabel *sub_image;
    QVBoxLayout *external_layout;
    QWidget *main_widget;
    QHBoxLayout *main_layout;
    void setIcon(const QPixmap&);
    void setName(std::string);
    void setSubscribers(int);
    void startSpinner();
    void stopSpinner();
    void setOnlineSubscribers(int);
  };
  class SmallFormWidget : public QPushButton {
    QHBoxLayout *mainLayout;
    QLabel *sub_title;
    QLabel *sub_image;
  public:
    SmallFormWidget(std::string);
    void setIcon(const QPixmap&);
  };
}

#endif
