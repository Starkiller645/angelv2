#include "submissionwidget.h"

#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <string>

submissionwidget::SubmissionWidget::SubmissionWidget(std::string title,
                                                     std::string body_plaintext,
                                                     std::string author,
                                                     int score,
                                                     int id,
                                                     submissionwidget::submission_type type) {

  this->submission_widget = new QWidget();
  this->info_widget = new QWidget();

  this->main_layout = new QHBoxLayout();
  this->info_layout = new QHBoxLayout();
  this->submission_layout = new QVBoxLayout();

  this->submission_title = new QLabel(title.c_str());
  this->submission_author = new QLabel(author.c_str());
  this->submission_score = new QLabel(std::to_string(score).c_str());
  this->longname = id;
  switch(type) {
    case submissionwidget::submission_type::Image:
      this->submission_icon_pixmap.load(":/images/icon-image.svg");
    case submissionwidget::submission_type::Video:
      this->submission_icon_pixmap.load(":/images/icon-video.svg");
    case submissionwidget::submission_type::Text:
      this->submission_icon_pixmap.load(":/images/icon-text.svg");
    case submissionwidget::submission_type::Link:
      this->submission_icon_pixmap.load(":/images/icon-link.svg");
  };
  this->submission_icon_pixmap = this->submission_icon_pixmap.scaledToWidth(10, Qt::SmoothTransformation);

  this->submission_icon = new QLabel();
  this->submission_icon->setPixmap(this->submission_icon_pixmap);

  this->info_layout->addWidget(this->submission_score);
  this->info_layout->addWidget(this->submission_author);
  this->info_layout->addWidget(this->submission_icon);
  this->info_widget->setLayout(this->info_layout);

  this->submission_layout->addWidget(this->submission_title);
  this->submission_layout->addWidget(this->info_widget);
  this->submission_widget->setLayout(this->submission_layout);

  this->main_layout->setSpacing(0);
  this->main_layout->addWidget(this->submission_widget);
  this->setLayout(this->main_layout);

  this->submission_score->setStyleSheet("color: #ff4500; font-weight: bold;");
  this->submission_author->setStyleSheet("color: #0dd3bb;");
  this->submission_title->setStyleSheet("font-size: 16px; font-weight: bold; color: #fcfcfc;");
  this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
  this->setMinimumWidth(0);
  this->setMinimumHeight(90);
  this->info_layout->setAlignment(Qt::AlignLeft);
  this->submission_score->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  this->submission_author->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  this->submission_icon->setMaximumWidth(20);
  this->submission_icon->setStyleSheet("padding: 5px;");
}
