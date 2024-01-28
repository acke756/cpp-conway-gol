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
  // TODO: Look into memory optimization.
  std::vector<bool> new_data(data_.size());

  for (size_type row = 0; row < height_; row++) {
    for (size_type column = 0; column < width_; column++) {
      unsigned int nb_count = live_neighbour_count_(row, column);

      if (nb_count == 3) {
        new_data[index_of_(row, column)] = true;
      } else if (nb_count < 2 || nb_count > 3) {
        new_data[index_of_(row, column)] = false;
      } else {
        new_data[index_of_(row, column)] = at(row, column);
      }
    }
  }

  data_.swap(new_data);
}

// --- Private methods ---

unsigned int ConwayGol::live_neighbour_count_(size_type row, size_type column) const {
  unsigned int retval = 0;

  // Abbreviating neighbour as nb.
  for (size_type nb_row = row - 1; nb_row <= row + 1; nb_row++) {
    for (size_type nb_column = column - 1; nb_column <= column + 1; nb_column++) {
      if (!is_valid_index_(nb_row, nb_column) || (nb_row == row && nb_column == column)) {
        continue;
      }

      retval += at(nb_row, nb_column) ? 1 : 0;
    }
  }

  return retval;
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
