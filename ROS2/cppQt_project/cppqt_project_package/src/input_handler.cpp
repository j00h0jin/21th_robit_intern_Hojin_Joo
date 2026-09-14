#include "cppqt_project_package/input_handler.hpp"

InputHandler::InputHandler() : key_w_(false), key_a_(false), key_s_(false), key_d_(false)
{
}
void InputHandler::handleKeyPress(QKeyEvent *event, bool is_game_over, bool stamina_exhausted)
{
    if (is_game_over || stamina_exhausted)
        return;

    if (event->key() == Qt::Key_W)
        key_w_ = true;
    else if (event->key() == Qt::Key_A)
        key_a_ = true;
    else if (event->key() == Qt::Key_S)
        key_s_ = true;
    else if (event->key() == Qt::Key_D)
        key_d_ = true;
}

void InputHandler::handleKeyRelease(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
        key_w_ = false;
    else if (event->key() == Qt::Key_A)
        key_a_ = false;
    else if (event->key() == Qt::Key_S)
        key_s_ = false;
    else if (event->key() == Qt::Key_D)
        key_d_ = false;
}

void InputHandler::clearKeys()
{
    key_w_ = key_a_ = key_s_ = key_d_ = false;
}