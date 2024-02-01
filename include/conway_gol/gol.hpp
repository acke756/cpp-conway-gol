#pragma once

#include <ostream>
#include <vector>

namespace conway_gol {
  class Gol {
    public:
    typedef std::vector<bool>::size_type size_type;

    Gol(size_type width, size_type height);

    bool at(size_type row, size_type column) const;
    std::vector<bool>::reference at(size_type row, size_type column);

    void update();

    friend std::ostream& operator<<(std::ostream& os, const Gol& gol);

    private:
    const size_type width_;
    const size_type height_;
    std::vector<bool> data_;

    constexpr size_type index_of_(size_type row, size_type column) const {
      return row * width_ + column;
    }

    constexpr bool is_valid_index_(size_type row, size_type column) const {
      return row >= 0 && row < height_ && column > 0 && column < width_;
    }

    unsigned int live_neighbour_count_(size_type row, size_type column) const;
  };

  std::ostream& operator<<(std::ostream& os, const Gol& gol);

} // namespace conway_gol
