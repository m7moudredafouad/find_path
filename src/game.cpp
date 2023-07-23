#include <game.hpp>
#include <iostream>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

void Render(Game* game) {
    auto& window = game->GetWindow();
    auto& matrix = game->GetMatrix();

    window.setActive(true);

    sf::RectangleShape temp_shape;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized) {
                sf::View view(sf::FloatRect(0.f, 0.f, event.size.width,
                                            event.size.height));
                window.setView(view);
            }
        }

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

        window.display();
    }
}

void Random(Game* game) {
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

    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            m_matrix[i][j] =
                Cell{j * padding, i * padding, width, height, sf::Color::White};
        }
    }

    m_window.setActive(false);
    sf::Thread render_thread(&Render, this);
    render_thread.launch();

    sf::Thread random_thread(&Random, this);
    random_thread.launch();

    while (m_window.isOpen()) {
        sf::Clock clock;
        sf::Time elapsed1 = clock.getElapsedTime();
        std::cout << 1 / elapsed1.asSeconds() << std::endl;
    }
}
