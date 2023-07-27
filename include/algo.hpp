#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

template <typename T> class Algorithm {
  public:
    virtual bool Apply(T&) = 0;
};

template <typename T>
class SimplePathFind : public Algorithm<std::vector<std::vector<T>>> {
  public:
    virtual bool Apply(std::vector<std::vector<T>>& matrix) override {
        auto rows = matrix.size(), cols = matrix[0].size();

        for (decltype(rows) i = 0; i < rows; i++) {
            for (decltype(cols) j = 0; j < cols; j++) {
                auto& el = matrix[i][j];
                if (el.IsStart())
                    continue;
                if (el.IsBlock())
                    break;
                if (el.IsEnd())
                    return true;

                el.Visit();
            }
        }

        return false;
    }
};

template <typename T>
class TmpPathFind : public Algorithm<std::vector<std::vector<T>>> {
  public:
    virtual bool Apply(std::vector<std::vector<T>>& matrix) override {
        return Find(matrix, 0, 0);
    }

  private:
    bool Find(std::vector<std::vector<T>>& matrix, int i, int j) {
        static int dirs[][2] = {
            {1, 0},
            {0, -1},
            {0, 1},
            {-1, 0},
        };

        if (i < 0 || j < 0 || i >= matrix.size() || j >= matrix[0].size())
            return false;

        if (matrix[i][j].IsBlock())
            return false;

        if (matrix[i][j].IsEnd())
            return true;

        if (matrix[i][j].IsVisited())
            return false;

        matrix[i][j].Visit();

        for (auto& dir : dirs) {
            if (Find(matrix, i + dir[0], j + dir[1]))
                return true;
            sf::sleep(sf::milliseconds(10));
        }

        return false;
    }
};