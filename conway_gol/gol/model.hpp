#pragma once

#include <ostream>
#include <conway_gol/grid.hpp>

namespace conway_gol {
  class Gol {
    public:
    using grid_type = Grid<bool>;
    using size_type = grid_type::size_type;
    using reference = grid_type::reference;
    using const_reference = grid_type::const_reference;

    Gol(size_type width, size_type height);

    size_type width() const;
    size_type height() const;

    const_reference at(size_type row, size_type column) const;
    reference at(size_type row, size_type column);

    void update();

    friend std::ostream& operator<<(std::ostream& os, const Gol& gol);

    private:
    Grid<bool> data_;

    unsigned int live_neighbour_count_(size_type row, size_type column) const;
  };

  std::ostream& operator<<(std::ostream& os, const Gol& gol);

} // namespace conway_gol
