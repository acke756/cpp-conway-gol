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
    using coordinate = grid_type::coordinate;

    Gol(size_type width, size_type height);

    size_type width() const;
    size_type height() const;

    const_reference at(const coordinate& c) const;
    reference at(const coordinate& c);

    bool has_data_at(const coordinate& c) const noexcept;

    void update();

    friend std::ostream& operator<<(std::ostream& os, const Gol& gol);

    private:
    Grid<bool> data_;

    unsigned int live_neighbour_count_(const coordinate& c) const;
  };

  std::ostream& operator<<(std::ostream& os, const Gol& gol);

} // namespace conway_gol
