#include "player.h"

Player::Player(Color color) : m_color(color) {}

Color Player::getColor() const {
    return m_color;
}
