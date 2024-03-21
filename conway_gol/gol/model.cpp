#include <conway_gol/gol/model.hpp>

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

  size_type Gol::width() const {
    return data_.width();
  }

  size_type Gol::height() const {
    return data_.height();
  }

  const_reference Gol::at(const coordinate& c) const {
    return data_.at(c);
  }

  reference Gol::at(const coordinate& c) {
    return data_.at(c);
  }

  bool Gol::has_data_at(const coordinate& c) const noexcept {
    return data_.has_data_at(c);
  }

  void Gol::update() {
    // TODO: Look into memory optimization.
    Grid<bool> new_data(data_);
    coordinate c;

    for (c.row = 0; c.row < data_.height(); c.row++) {
      for (c.column = 0; c.column < data_.width(); c.column++) {
        unsigned int nb_count = live_neighbour_count_(c);

        if (nb_count == 3) {
          new_data.at(c) = true;
        } else if (nb_count < 2 || nb_count > 3) {
          new_data.at(c) = false;
        }
      }
    }

    data_ = std::move(new_data);
  }

  // --- Private methods ---

  unsigned int Gol::live_neighbour_count_(const coordinate& c) const {
    unsigned int retval = 0;
    coordinate nb; // nb is short for neighbour

    for (nb.row = c.row - 1; nb.row != c.row + 2; nb.row++) {
      for (nb.column = c.column - 1; nb.column != c.column + 2; nb.column++) {
        if (!data_.has_data_at(nb) || (nb == c)) {
          continue;
        }

        retval += data_.at(nb) ? 1 : 0;
      }
    }

    return retval;
  }

  // --- Non-member functions ---

  std::ostream& operator<<(std::ostream& os, const Gol& gol) {
    Gol::coordinate c;
    for (c.row = 0; c.row < gol.data_.height(); c.row++) {
      for (c.column = 0; c.column < gol.data_.width(); c.column++) {
        os << (gol.data_.at(c) ? '*' : ' ');
      }

      if (c.row != gol.data_.height() - 1) {
        os << std::endl;
      }
    }

    return os;
  }

} // namespace conway_gol
