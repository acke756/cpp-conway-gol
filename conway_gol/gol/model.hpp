#pragma once

#include <ostream>
#include <conway_gol/grid.hpp>

namespace conway_gol {
  class IGol {
    public:
      using grid_type = Grid<bool>;
      using size_type = grid_type::size_type;
      using reference = grid_type::reference;
      using const_reference = grid_type::const_reference;
      using coordinate = grid_type::coordinate;

      virtual size_type width() const = 0;
      virtual size_type height() const = 0;
      
      virtual const_reference at(const coordinate& c) const = 0;
      virtual reference at(const coordinate& c) = 0;

      virtual bool has_data_at(const coordinate& c) const noexcept = 0;

      virtual void update() = 0;

      friend std::ostream& operator<<(std::ostream& os, const IGol& gol);
  };

  class Gol: public IGol {
    public:
      Gol(size_type width, size_type height);

      virtual size_type width() const override;
      virtual size_type height() const override;

      virtual const_reference at(const coordinate& c) const override;
      virtual reference at(const coordinate& c) override;

      virtual bool has_data_at(const coordinate& c) const noexcept override;

      virtual void update() override;

    private:
      Grid<bool> data_;

      unsigned int live_neighbour_count_(const coordinate& c) const;
  };

} // namespace conway_gol
