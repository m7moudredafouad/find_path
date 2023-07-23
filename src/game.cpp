#include <game.hpp>
#include <iostream>

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

static void Events(Game* game) {
    auto& window = game->GetWindow();

    sf::RectangleShape temp_shape;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed: {
                window.close();
                break;
            }
            case sf::Event::Resized: {
                sf::View view(sf::FloatRect(0.f, 0.f, event.size.width,
                                            event.size.height));
                window.setView(view);
                break;
            }
            case sf::Event::MouseButtonPressed: {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    game->ForEachObject(
                        [event](Cell& cell) {
                            if(cell.contains(event.mouseButton.x, event.mouseButton.y)) {
                                std::cout << cell.x << " " << cell.y << std::endl;
                            }
                        });
                }
                break;
            }

            default:
                sf::sleep(sf::milliseconds(20));
                break;
            }
        }
    }
}

static void Render(Game* game) {
    auto& window = game->GetWindow();
    auto& matrix = game->GetMatrix();

    window.setActive(true);

    sf::Font font;
    sf::Text text;
    if (!font.loadFromFile("./fonts/arial.ttf"))
        assert(0);

    text.setFont(font);
    text.setCharacterSize(16); // in pixels, not points!
    text.setFillColor(sf::Color::White);

    int fps = 0;
    sf::RectangleShape temp_shape;
    sf::Clock clock;
    while (window.isOpen()) {
        text.setString(std::to_string(fps));

        sf::Time elapsed1 = clock.restart();
        window.clear();

        for (uint32_t i = 0; i < matrix.size(); i++) {
            for (uint32_t j = 0; j < matrix[i].size(); j++) {
                auto& el = matrix[i][j];
                temp_shape.setSize(sf::Vector2f(el.width, el.height));
                temp_shape.setFillColor(el.color);
                temp_shape.setPosition(sf::Vector2f(el.x, el.y));

                window.draw(temp_shape);
            }
        }

        window.draw(text);
        window.display();
        fps = 1 / elapsed1.asSeconds();
    }
}

static void Random(Game* game) {
    auto& window = game->GetWindow();
    auto& matrix = game->GetMatrix();

    while (window.isOpen()) {
        auto x = rand() % game->cols;
        auto y = rand() % game->rows;

        matrix[y][x].color = matrix[y][x].color == sf::Color::White
                                 ? sf::Color::Red
                                 : sf::Color::White;

        sf::sleep(sf::milliseconds(100));
    }
}

void Game::Loop() {
    m_window.setTitle("2dgame");

    m_matrix = std::vector<std::vector<Cell>>(rows, std::vector<Cell>(cols));

    int offset_y = 30;

    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            m_matrix[i][j] =
                Cell{j * padding, i * padding + offset_y, width, height, sf::Color::White};
        }
    }

    m_window.setActive(false);

    sf::Thread render_thread(&Render, this);
    render_thread.launch();

    sf::Thread event_thread(&Events, this);
    event_thread.launch();

    sf::Thread random_thread(&Random, this);
    random_thread.launch();

    while (m_window.isOpen()) {
    }
}

template <typename T> void Game::ForEachObject(T apply) {
    for (auto& mrow : m_matrix) {
        for (auto& mcol : mrow) {
            apply(mcol);
        }
    }
}
