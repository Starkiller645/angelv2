#include "subredditwidget.h"

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <string>

subredditwidget::SubredditWidget::SubredditWidget() {
  this->main_layout = new QHBoxLayout();
  this->main_widget = new QWidget();
  this->external_layout = new QVBoxLayout();
  this->spinner = new WaitingSpinnerWidget();
  this->spinner->setColor(QColor(255, 255, 255));
  this->spinnerContainer = new QWidget();
  this->spinnerContainerLayout = new QVBoxLayout();
  this->spinnerContainer->setLayout(this->spinnerContainerLayout);
  this->spinnerContainerLayout->addWidget(this->spinner);
  this->sub_title = new QLabel("<b>subreddit</b>");
  this->sub_title->setStyleSheet("color: #fcfcfc; font-size: 50px; padding: 8px;");
  this->sub_image = new QLabel();
  this->sub_image->setMinimumWidth(60);
  this->sub_image->setLayout(this->external_layout);
  this->sub_online = new QLabel("Online: ");
  this->sub_online->setStyleSheet("color: #ff4500; padding-left: 16px; padding-top: 4px;");
  this->sub_image->setStyleSheet("background-color: #31363b;");
  this->sub_image->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
  this->sub_subscribers = new QLabel(" Subscribed: ");
  this->sub_subscribers->setStyleSheet("color: #ff4500; padding-left: 16px; padding-top: 4px;");
  this->sub_subtext_widget = new QWidget();
  this->sub_title_widget = new QWidget();
  this->sub_main_layout = new QVBoxLayout();
  this->sub_subtext_layout = new QHBoxLayout();
  this->sub_subtext_layout->setAlignment(Qt::AlignLeft);
  this->sub_title_layout = new QHBoxLayout();
  this->sub_main_layout->setMargin(0);
  this->sub_main_layout->setSpacing(0);
  QWidget *spacer_bottom = new QWidget();
  spacer_bottom->setFixedWidth(60);
  spacer_bottom->setStyleSheet("background-color: #31363b;");
  this->sub_title_layout->setMargin(0);
  this->sub_subtext_layout->setMargin(0);

  this->sub_title_layout->addWidget(this->sub_title);
  this->sub_title_layout->addWidget(this->spinnerContainer);
  this->sub_title_layout->setAlignment(this->spinnerContainer, Qt::AlignRight | Qt::AlignVCenter);
  this->spinnerContainer->setStyleSheet("margin: 8px; padding: 10px;");
  this->sub_subtext_layout->addWidget(this->sub_subscribers);
  this->sub_subtext_layout->addWidget(this->sub_online);

  this->sub_subtext_widget->setLayout(this->sub_subtext_layout);
  this->sub_title_widget->setLayout(this->sub_title_layout);

  this->sub_main_layout->addWidget(this->sub_title_widget);
  this->sub_main_layout->addWidget(this->sub_subtext_widget);
  this->main_widget->setLayout(this->sub_main_layout);
  this->main_widget->setMaximumWidth(440);
  this->main_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
  this->main_layout->addWidget(this->sub_image);
  this->main_layout->addWidget(this->main_widget);
  this->main_layout->setMargin(0);
  this->main_layout->setSpacing(0);
  this->main_layout->setAlignment(Qt::AlignLeft);
  this->sub_main_layout->setSpacing(0);
  this->sub_main_layout->setMargin(0);
  this->setLayout(this->main_layout);

  return;
}

void subredditwidget::SubredditWidget::setIcon(const QPixmap& icon) {
  this->sub_image->setPixmap(icon);
  return;
}

void subredditwidget::SubredditWidget::startSpinner() {
  this->spinner->show();
  this->spinner->start();
}

void subredditwidget::SubredditWidget::stopSpinner() {
  this->spinner->stop();
  this->spinner->hide();
}

void subredditwidget::SubredditWidget::setName(std::string name) {
  this->sub_title->setText(QString(name.c_str()));
  return;
}

void subredditwidget::SubredditWidget::setSubscribers(int subs) {
  char result[100];
  strcpy(result, "Subscribed: ");
  strcat(result, std::to_string(subs).c_str());
  this->sub_subscribers->setText(QString(result));
  return;
}

void subredditwidget::SubredditWidget::setOnlineSubscribers(int online_subs) {
  char result[100];
  strcpy(result, "Online: ");
  strcat(result, std::to_string(online_subs).c_str());
  this->sub_online->setText(QString(result));
  return;
}

subredditwidget::SmallFormWidget::SmallFormWidget(std::string subname) {
  this->sub_title = new QLabel(subname.c_str());
  this->sub_image = new QLabel();
  this->mainLayout = new QHBoxLayout();
  this->setStyleSheet("padding: 0px;");
  this->setMinimumHeight(40);
  this->sub_image->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  this->sub_title->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

  //this->mainLayout->addWidget(this->sub_image);
  this->mainLayout->addWidget(this->sub_title);
  this->setLayout(this->mainLayout);
}

void subredditwidget::SmallFormWidget::setIcon(const QPixmap &pixmap) {
  this->sub_image->setPixmap(pixmap);
}
