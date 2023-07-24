#pragma once
#include <vector>

template <typename T> class Algorithm {
  public:
    virtual bool Find(T&) = 0;
};

template <typename T>
class SimplePathFind : public Algorithm<std::vector<std::vector<T>>> {
  public:
    virtual bool Find(std::vector<std::vector<T>>& matrix) override {
        auto rows = matrix.size(), cols = matrix[0].size();

        for (decltype(rows) i = 0; i < rows; i++) {
            for (decltype(cols) j = 0; j < cols; j++) {
                auto &el = matrix[i][j];
                if(el.IsStart()) continue;
                if(el.IsBlock()) break;
                if(el.IsEnd()) return true;

                el.Visit();
            }
        }

        return false;
    }
};