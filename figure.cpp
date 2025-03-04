#include "figure.h"

Figure::Figure(Color color) : m_color(color) {}

Color Figure::getColor() const {
    return m_color;
}
