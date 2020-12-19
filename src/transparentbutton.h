#ifndef TRANSPARENT_BUTTON_H
#define TRANSPARENT_BUTTON_H
#include <QPushButton>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsOpacityEffect>

namespace transparentbutton {
  class TransparentButton : public QPushButton {
    public:
      TransparentButton(QIcon, QSize, int);
      void setOpacity();
      int getOpacity();
      void doAnimation();
    private:
      QPropertyAnimation *fade_anim;
      QTimer *timer;
      int time;
      QGraphicsOpacityEffect *opacity_effect;
  };
}
#endif
