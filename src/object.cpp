#include <assert.h>
#include <object.hpp>

bool Object::Contains(float x, float y) const {
    if (x < pos_x || y < pos_y)
        return false;
    if (x > pos_x + width || y > pos_y + height)
        return false;

    return true;
}

bool Object::Within(float x1, float y1, float x2, float y2) const {

    auto left_a = std::min(x1, x2);
    auto bottom_a = std::min(y1, y2);
    auto right_a = std::max(x1, x2);
    auto top_a = std::max(y1, y2);

    // if (pos_x >= min_x && pos_x <= max_y && pos_y >= min_y && pos_y <= max_y)
    //     return true;
    // return false;

    return std::max(pos_x, left_a) < std::min(pos_x + width, right_a)
    && std::max(pos_y, top_a) < std::min(pos_x + height, bottom_a);

}

void Cell::Click() {
    switch (cell_type) {
    case Type::kOpen: {
        cell_type = Type::kBlock;
        break;
    }
    case Type::kBlock: {
        cell_type = Type::kOpen;
        break;
    }

    default:
        break;
    }
}

void Cell::LeftPress() {
    switch (cell_type) {
    case Type::kOpen:
    case Type::kVisited: {
        cell_type = Type::kBlock;
        break;
    }
    case Type::kBlock: {
        cell_type = Type::kOpen;
        break;
    }

    default:
        break;
    }
};

std::unique_ptr<sf::Drawable> Cell::GetShape() {
    std::unique_ptr<sf::RectangleShape> shape(
        new sf::RectangleShape(sf::Vector2f(width, height)));

    switch (cell_type) {
    case Type::kOpen: {
        color = sf::Color::White;
        break;
    }
    case Type::kBlock: {
        color = sf::Color::Red;
        break;
    }
    case Type::kStart: {
        color = sf::Color::Blue;
        break;
    }
    case Type::kVisited: {
        color = sf::Color::Magenta;
        break;
    }
    case Type::kEnd: {
        color = sf::Color::Green;
        break;
    }

    default:
        break;
    }
    shape->setFillColor(color);
    shape->setPosition(sf::Vector2f(pos_x, pos_y));

    return std::move(shape);
}

void Button::Click() { m_click_func(); }

std::unique_ptr<sf::Drawable> Button::GetShape() {

    static sf::RenderTexture m_render_texture;

    if (!m_render_texture.create(width, height)) {
        assert(0);
    }

    sf::RectangleShape shape(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color::White);

    m_render_texture.clear();
    m_render_texture.draw(shape);
    m_render_texture.draw(m_draw_text);
    m_render_texture.display();

    std::unique_ptr<sf::Sprite> ptr(new sf::Sprite());
    ptr->setTexture(m_render_texture.getTexture());
    ptr->setPosition(sf::Vector2f(pos_x, pos_y));

    return std::move(ptr);
}

void Text::Click() { m_click_func(); }

std::unique_ptr<sf::Drawable> Text::GetShape() {

    std::unique_ptr<sf::Text> ptr(new sf::Text(m_draw_text));

    return std::move(ptr);
}