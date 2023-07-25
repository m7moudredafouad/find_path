#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Object {
  public:
    float pos_x, pos_y, width, height;

    virtual bool Contains(double x, double y) const;
    virtual bool Within(double x1, double y1, double x2, double y2) const;
    virtual void Click() = 0;
    virtual std::unique_ptr<sf::Drawable> GetShape() = 0;

  protected:
    sf::Color color;

    Object() = default;

    Object(float pos_x, float pos_y, float width, float height,
           sf::Color color = sf::Color::White)
        : pos_x(pos_x), pos_y(pos_y), width(width), height(height),
          color(color) {
        ;
    }
};

class Cell : public Object {
  public:
    enum class Type : uint32_t {
        kOpen,
        kStart,
        kBlock,
        kVisited,
        kEnd,
    };

  public:
    Cell() = default;

    Cell(float pos_x, float pos_y, float width, float height)
        : Object{pos_x, pos_y, width, height}, cell_type(Type::kOpen) {
        ;
    }

    virtual void Click() override;

    virtual std::unique_ptr<sf::Drawable> GetShape() override;

    void UpdateType(Cell::Type new_type) { cell_type = new_type; }

    bool IsStart() const { return cell_type == Type::kStart; }

    bool IsVisited() const { return cell_type == Type::kVisited; }

    bool IsBlock() const { return cell_type == Type::kBlock; }

    bool IsEnd() const { return cell_type == Type::kEnd; }

    void Visit() { cell_type = Type::kVisited; }

  private:
    Type cell_type;
};

class Button : public Object {
  public:
    Button() = default;

    Button(float pos_x, float pos_y, float width, float height, sf::Font& font,
           std::string text)
        : Object{pos_x, pos_y, width, height}, m_draw_text(text, font, 16) {
        m_draw_text.setFillColor(sf::Color::Black);

        auto offset_x = (width - m_draw_text.getGlobalBounds().width) / 2;
        m_draw_text.setPosition(sf::Vector2f(offset_x, 0));

        m_func = [] () {};
    }

    virtual void Click() override;

    virtual std::unique_ptr<sf::Drawable> GetShape() override;

    void SetClickFuntion(std::function<void()> func) { m_func = func; }

  private:
    sf::Text m_draw_text;
    std::function<void()> m_func;
};
