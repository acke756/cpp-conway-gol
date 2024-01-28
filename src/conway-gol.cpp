#include <conway-gol/conway-gol.hpp>

#include <random>

typedef ConwayGol::size_type size_type;

// --- Public methods ---

ConwayGol::ConwayGol(size_type width, size_type height):
    width_(width),
    height_(height),
    data_(width * height) {
  auto rng = std::mt19937();
  auto dist = std::bernoulli_distribution();
  for (auto it = data_.begin(); it != data_.end(); it++) {
    *it = dist(rng);
  }
}

bool ConwayGol::at(size_type row, size_type column) const {
  // TODO: Assert that row and column are in range.
  return data_[index_of_(row, column)];
}

std::vector<bool>::reference ConwayGol::at(size_type row, size_type column) {
  // TODO: Assert that row and column are in range.
  return data_[index_of_(row, column)];
}

void ConwayGol::update() {
  for (auto it = data_.begin(); it != data_.end(); it++) {
    *it = !*it;
  }
}

// --- Non-member functions ---

std::ostream& operator<<(std::ostream& os, const ConwayGol& gol) {
  for (size_type row = 0; row < gol.height_; row++) {
    for (size_type column = 0; column < gol.width_; column++) {
      os << (gol.at(row, column) ? '*' : ' ');
    }

    if (row != gol.height_ - 1) {
      os << std::endl;
    }
  }

  return os;
}
