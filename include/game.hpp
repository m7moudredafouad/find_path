#pragma once

#include <SFML/Graphics.hpp>
#include <cell.hpp>
#include <vector>

class Game {
  public:
    uint32_t rows, cols;

  public:
    Game() : m_window(sf::RenderWindow(sf::VideoMode(800, 600), "")), rows(20), cols(25) { ; }

    Game(uint32_t rows, uint32_t cols) : rows(rows), cols(cols) { ; }

    void Loop();

    sf::RenderWindow& GetWindow() { return m_window; }

    std::vector<std::vector<Cell>>& GetMatrix() { return m_matrix; }

  private:
    sf::RenderWindow m_window;
    std::vector<std::vector<Cell>> m_matrix;
    float padding = 27.f, width = 25.f, height = 25.f;
};
