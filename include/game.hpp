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
        : m_window(sf::RenderWindow(sf::VideoMode(1280 , 720), "")), rows(45),
          cols(60) {
        if (!m_font.loadFromFile("./fonts/arial.ttf"))
            assert(0);
    }

    Game(uint32_t rows, uint32_t cols) : rows(rows), cols(cols) { ; }

    void Loop();

    sf::RenderWindow& GetWindow() { return m_window; }

    std::vector<std::vector<Cell>>& GetMatrix() { return m_matrix; }

    const sf::Font& GetFont() const { return m_font; }

    template <typename T> void ForEachObject(T apply);

  private:
    sf::RenderWindow m_window;
    sf::Font m_font;
    std::vector<std::vector<Cell>> m_matrix;
    std::vector<std::unique_ptr<Object>> m_objects;

    void Init();
    void ResetMatrix();
};
