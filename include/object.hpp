#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Object {
  public:
    float pos_x, pos_y, width, height;

    virtual bool Contains(float x, float y) const;
    virtual bool Within(float x1, float y1, float x2, float y2) const;
    virtual std::unique_ptr<sf::Drawable> GetShape() = 0;
    virtual void Click(){};
    virtual void LeftPress(){};

    void SetClickFuntion(std::function<void()> func) { m_click_func = func; }

    void SetLeftPressFuntion(std::function<void()> func) {
        m_left_press_func = func;
    }

  protected:
    sf::Color color;
    std::function<void()> m_click_func, m_left_press_func;

    Object() = default;

    Object(float pos_x, float pos_y, float width, float height,
           sf::Color color = sf::Color::White)
        : pos_x(pos_x), pos_y(pos_y), width(width), height(height),
          color(color) {
        m_click_func = []() {};
        m_left_press_func = []() {};
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
    virtual void LeftPress() override;

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

    Button(float pos_x, float pos_y, float width, float height,
           const sf::Font& font, std::string text)
        : Object{pos_x, pos_y, width, height}, m_draw_text(text, font, 16) {
        m_draw_text.setFillColor(sf::Color::Black);

        auto offset_x = (width - m_draw_text.getGlobalBounds().width) / 2;
        m_draw_text.setPosition(sf::Vector2f(offset_x, 0));
    }

    virtual void Click() override;

    virtual std::unique_ptr<sf::Drawable> GetShape() override;

  private:
    sf::Text m_draw_text;
};

class Text : public Object {
  public:
    Text() = default;

    Text(float pos_x, float pos_y, float width, float height,
         const sf::Font& font, std::string text)
        : Object{pos_x, pos_y, width, height}, m_draw_text(text, font, 16) {
        m_draw_text.setFillColor(sf::Color::White);
        m_draw_text.setPosition(sf::Vector2f(pos_x, pos_y));
    }

    virtual void Click() override;

    virtual std::unique_ptr<sf::Drawable> GetShape() override;

    void SetText(const std::string& text) { m_draw_text.setString(text); }

  private:
    sf::Text m_draw_text;
};
