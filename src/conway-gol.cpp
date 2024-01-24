#include <conway-gol/conway-gol.hpp>

#include <random>

// --- ConwayGol methods ---

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

void ConwayGol::update() {
  for (auto it = data_.begin(); it != data_.end(); it++) {
    *it = !*it;
  }
}

// --- Non-member functions ---

std::ostream& operator<<(std::ostream& os, const ConwayGol& gol) {
  typedef ConwayGol::size_type size_type;

  for (size_type row = 0; row < gol.height_; row++) {
    for (size_type column = 0; column < gol.width_; column++) {
      size_type index = row * gol.width_ + column;

      os << (gol.data_[index] ? '*' : ' ');
    }

    if (row != gol.height_ - 1) {
      os << std::endl;
    }
  }

  return os;
}
