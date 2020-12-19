#include "transparentbutton.h"
#include <QPushButton>
#include <QPainter>
#include <QGraphicsOpacityEffect>

transparentbutton::TransparentButton::TransparentButton(QIcon icon, QSize iconSize, int time) {
  this->setIcon(icon);
  this->setIconSize(iconSize);
  this->time = time;
  this->opacity_effect = new QGraphicsOpacityEffect(this);
  this->setGraphicsEffect(this->opacity_effect);
  this->opacity_effect->setOpacity(0);


  this->fade_anim = new QPropertyAnimation(this->opacity_effect, "opacity");
  this->fade_anim->setStartValue(0);
  this->fade_anim->setEndValue(1);
  this->fade_anim->setDuration(800);
  this->timer = new QTimer(this);
  this->timer->setSingleShot(true);
  connect(this->timer, &QTimer::timeout, this, [=]() {this->fade_anim->start();});
};

void transparentbutton::TransparentButton::doAnimation() {
  this->timer->start(this->time);
}
