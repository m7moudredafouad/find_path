#pragma once

#include <SFML/Graphics.hpp>
#include <assert.h>
#include <object.hpp>
#include <vector>

class Game {
  public:
    uint32_t rows, cols;

  public:
    Game()
        : m_window(sf::RenderWindow(sf::VideoMode(800, 600), "")), rows(20),
          cols(25) {
        if (!m_font.loadFromFile("./fonts/arial.ttf"))
            assert(0);
    }

    Game(uint32_t rows, uint32_t cols) : rows(rows), cols(cols) { ; }

    void Loop();

    sf::RenderWindow& GetWindow() { return m_window; }

    std::vector<std::vector<Cell>>& GetMatrix() { return m_matrix; }

    const sf::Font& GetFont() const { return m_font; }

    template <typename T> void ForEachObject(T apply);
    template <typename T> void ForEachCell(T apply);

  private:
    sf::RenderWindow m_window;
    sf::Font m_font;
    std::vector<std::vector<Cell>> m_matrix;
    std::vector<Button> m_buttons;
    float padding = 27.f, width = 25.f, height = 25.f;

    void Restart();
    void ResetMatrix();
};
