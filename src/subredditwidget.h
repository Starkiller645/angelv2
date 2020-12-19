#ifndef SUBREDDIT_WIDGET_H
#define SUBREDDIT_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>

namespace subredditwidget {
  class SubredditWidget : public QWidget {
    QVBoxLayout *sub_main_layout;
    QHBoxLayout *sub_title_layout;
    QHBoxLayout *sub_subtext_layout;
    QWidget *sub_title_widget;
    QWidget *sub_subtext_widget;
    QLabel *sub_title;
    QLabel *sub_image;
    QLabel *sub_subscribers;
    QLabel *sub_online;
  public:
    SubredditWidget();
    void setIcon(const QPixmap&);
    void setName(std::string);
    void setSubscribers(int);
    void setOnlineSubscribers(int);
  };
}

#endif
