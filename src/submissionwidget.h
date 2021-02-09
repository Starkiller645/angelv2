#ifndef SUBMISSION_WIDGET_H
#define SUBMISSION_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace submissionwidget {
  enum submission_type {Image, Video, Text, Link};
  class SubmissionWidget : public QPushButton {
    Q_OBJECT
  public:
    SubmissionWidget(std::string, std::string, std::string, int, int, submissionwidget::submission_type);
    int index;
    std::string title;
  private:
    QLabel *submission_title;
    QLabel *submission_icon;
    QLabel *submission_score;
    QLabel *submission_author;

    QWidget *submission_widget;
    QWidget *info_widget;

    QPixmap submission_icon_pixmap;

    QHBoxLayout *main_layout;
    QHBoxLayout *info_layout;
    QVBoxLayout *submission_layout;

    std::string longname;
  };
}

#endif
