#ifndef PLAYER_H
#define PLAYER_H

#include "figure.h" // Include Figure.h because Player will have a color

class Player {
public:
    Player(Color color);
    ~Player() = default;

    Color getColor() const;

private:
    Color m_color;
};

#endif // PLAYER_H
