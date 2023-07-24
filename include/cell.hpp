#pragma once

#include <SFML/Graphics.hpp>

class Object {
  public:
    float pos_x, pos_y, width, height;

    virtual sf::Color GetColor() const { return color; }

    virtual bool Contains(double x, double y) = 0;
    virtual bool Within(double x1, double y1, double x2, double y2) = 0;
    virtual void Click() = 0;
    virtual std::unique_ptr<sf::Drawable> GetShape() = 0;

  protected:
    sf::Color color;

    Object() = default;

    Object(float pos_x, float pos_y, float width, float height, sf::Color color)
        : pos_x(pos_x), pos_y(pos_y), width(width), height(height),
          color(color) {
        ;
    }
};

class Cell : public Object {
  public:
    Cell() = default;

    Cell(float pos_x, float pos_y, float width, float height, sf::Color color)
        : Object{pos_x, pos_y, width, height, color} {
        ;
    }

    virtual bool Contains(double x, double y) override {
        if (x < pos_x || y < pos_y)
            return false;
        if (x > pos_x + width || y > pos_y + height)
            return false;

        return true;
    }

    virtual bool Within(double x1, double y1, double x2, double y2) override {

        auto min_x = std::min(y1, y2);
        auto min_y = std::min(x1, x2);
        auto max_x = std::max(y1, y2);
        auto max_y = std::max(x1, x2);

        if (pos_x >= min_x && pos_x <= max_y && pos_y >= min_y &&
            pos_y <= max_y)
            return true;

        return false;
    }

    virtual void Click() override {
        color = color == sf::Color::White ? sf::Color::Red : sf::Color::White;
    }

    virtual std::unique_ptr<sf::Drawable> GetShape() override {
        std::unique_ptr<sf::RectangleShape> shape(
            new sf::RectangleShape(sf::Vector2f(width, height)));

        shape->setFillColor(color);
        shape->setPosition(sf::Vector2f(pos_x, pos_y));

        return std::move(shape);
    }
};
