#pragma once

#include <QKeyEvent>

class InputHandler
{
  public:
    InputHandler();

    void handleKeyPress(QKeyEvent *event, bool is_game_over, bool stamina_exhausted);
    void handleKeyRelease(QKeyEvent *event);
    void clearKeys();

    bool isWPressed() const
    {
        return key_w_;
    }
    bool isAPressed() const
    {
        return key_a_;
    }
    bool isSPressed() const
    {
        return key_s_;
    }
    bool isDPressed() const
    {
        return key_d_;
    }

  private:
    bool key_w_ = false;
    bool key_a_ = false;
    bool key_s_ = false;
    bool key_d_ = false;
};