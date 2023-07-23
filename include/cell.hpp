#pragma once

#include <SFML/Graphics.hpp>

struct Cell {
    float x, y, width, height;
    sf::Color color;

    bool contains(double ox, double oy) {
        if(ox < x || oy < y) return false;
        if(ox > x + width || oy > y + height) return false;

        return true;
    }
};
