#pragma once

#include <ostream>
#include <vector>

class ConwayGol {
  public:
  typedef std::vector<bool>::size_type size_type;

  ConwayGol(size_type width, size_type height);

  bool at(size_type row, size_type column) const;
  std::vector<bool>::reference at(size_type row, size_type column);

  void update();

  friend std::ostream& operator<<(std::ostream& os, const ConwayGol& gol);

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

std::ostream& operator<<(std::ostream& os, const ConwayGol& gol);
