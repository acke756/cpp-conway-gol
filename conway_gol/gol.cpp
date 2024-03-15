#include <conway_gol/gol.hpp>

#include <iostream>
#include <random>

namespace conway_gol {

  using size_type = typename Gol::size_type;
  using const_reference = typename Gol::const_reference;
  using reference = typename Gol::reference;

  // --- Public methods ---

  Gol::Gol(size_type width, size_type height):
      data_(width, height) {
    auto rng = std::mt19937();
    auto dist = std::bernoulli_distribution();
    for (auto it = data_.begin(); it != data_.end(); it++) {
      *it = dist(rng);
    }
  }

  const_reference Gol::at(size_type column, size_type row) const {
    return data_.at(column, row);
  }

  reference Gol::at(size_type column, size_type row) {
    return data_.at(column, row);
  }

  void Gol::update() {
    // TODO: Look into memory optimization.
    Grid<bool> new_data(data_);

    for (size_type row = 0; row < data_.height(); row++) {
      for (size_type column = 0; column < data_.width(); column++) {
        unsigned int nb_count = live_neighbour_count_(column, row);

        if (nb_count == 3) {
          new_data.at(column, row) = true;
        } else if (nb_count < 2 || nb_count > 3) {
          new_data.at(column, row) = false;
        }
      }
    }

    data_ = std::move(new_data);
  }

  // --- Private methods ---

  unsigned int Gol::live_neighbour_count_(size_type column, size_type row) const {
    unsigned int retval = 0;

    // Abbreviating neighbour as nb.
    for (size_type nb_row = row - 1; nb_row != row + 2; nb_row++) {
      for (size_type nb_column = column - 1; nb_column != column + 2; nb_column++) {
        if (!data_.is_valid_index_(nb_column, nb_row) || (nb_row == row && nb_column == column)) {
          continue;
        }

        retval += data_.at(nb_column, nb_row) ? 1 : 0;
      }
    }

    return retval;
  }

  // --- Non-member functions ---

  std::ostream& operator<<(std::ostream& os, const Gol& gol) {
    for (size_type row = 0; row < gol.data_.height(); row++) {
      for (size_type column = 0; column < gol.data_.width(); column++) {
        os << (gol.data_.at(column, row) ? '*' : ' ');
      }

      if (row != gol.data_.height() - 1) {
        os << std::endl;
      }
    }

    return os;
  }

} // namespace conway_gol
