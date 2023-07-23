#include <Cell.hpp>
// #include <Threads.hpp>

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <vector>


float padding = 27.f, width = 25.f, height = 25.f;
uint32_t rows = 20, cols = 25;

std::vector<std::vector<Cell>> matrix;

void RenderThread(sf::RenderWindow* window) {
    window->setActive(true);

    sf::RectangleShape temp_shape;

    // the rendering loop
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.type == sf::Event::Resized) {
                sf::View view(sf::FloatRect(0.f, 0.f, event.size.width,
                                            event.size.height));
                window->setView(view);
            }
        }

        window->clear();

        for (uint32_t i = 0; i < matrix.size(); i++) {
            for (uint32_t j = 0; j < matrix[i].size(); j++) {
                auto& el = matrix[i][j];
                temp_shape.setSize(sf::Vector2f(el.width, el.height));
                temp_shape.setFillColor(el.color);
                temp_shape.setPosition(sf::Vector2f(j * padding, i * padding));

                window->draw(temp_shape);
            }
        }

        window->display();
    }
}

void RandomThread(sf::RenderWindow* window) {
    while (window->isOpen()) {
        auto x = rand() % cols;
        auto y = rand() % rows;

        matrix[y][x].color = matrix[y][x].color == sf::Color::White
                                 ? sf::Color::Red
                                 : sf::Color::White;

        sf::sleep(sf::milliseconds(100));
    }
}

int main() {
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(800, 600), "2dgame");

    matrix = std::vector<std::vector<Cell>>(
        rows, std::vector<Cell>(cols, Cell{width, height, sf::Color::White}));

    window.setActive(false);
    sf::Thread render_thread(&RenderThread, &window);
    render_thread.launch();

    sf::Thread random_thread(&RandomThread, &window);
    random_thread.launch();

    while (window.isOpen()) {
        sf::Clock clock;
        sf::Time elapsed1 = clock.getElapsedTime();
        std::cout << 1 / elapsed1.asSeconds() << std::endl;
    }

    return 0;
}