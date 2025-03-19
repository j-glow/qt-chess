#ifndef PLAYER_H
#define PLAYER_H

#include "figure.h" // Include Figure.h because Player will have a color

enum CastlingOptions {
    NONE = 0b00,
    KINGSIDE = 0b01,
    QUEENSIDE = 0b10,
    BOTH = KINGSIDE | QUEENSIDE
};

class Player {
public:
    Player(Color color);
    ~Player() = default;

    Color getColor() const;
    CastlingOptions getCastlingOptions() { return m_castlingOptions; }
    void setCastlingOptions(CastlingOptions castlingOptions) { m_castlingOptions = castlingOptions; }

private:
    Color m_color;
    CastlingOptions m_castlingOptions = CastlingOptions::BOTH;
};

#endif // PLAYER_H
