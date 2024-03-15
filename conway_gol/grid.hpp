#pragma once

#include <stdexcept>
#include <sstream>
#include <vector>

namespace conway_gol {
  template<typename T>
  class Grid {
    using container_type_ = typename std::vector<T>;

    public:
    using size_type = typename container_type_::size_type;
    using reference = typename container_type_::reference;
    using const_reference = typename container_type_::const_reference;
    using iterator = typename container_type_::iterator;
    using const_iterator = typename container_type_::const_iterator;

    Grid() = delete;
    ~Grid() = default;

    Grid(size_type width, size_type height):
        width_(width),
        height_(height),
        data_(width * height) {
    }

    Grid(const Grid&) = default;

    Grid(Grid&& other):
        width_(other.width),
        height_(other.height),
        data_(std::move(other.data)) {
      other.width_ = 0;
      other.height_ = 0;
    }

    Grid& operator=(const Grid&) = default;

    Grid& operator=(Grid&& other) {
      data_ = std::move(other.data_);
      other.width_ = 0;
      other.height_ = 0;
      return *this;
    }

    inline size_type width() const noexcept {
      return width_;
    }

    inline size_type height() const noexcept {
      return height_;
    }

    inline const_iterator begin() const noexcept {
      return data_.begin();
    }

    inline iterator begin() noexcept {
      return data_.begin();
    }

    inline const_iterator end() const noexcept {
      return data_.end();
    }

    inline iterator end() noexcept {
      return data_.end();
    }

    inline bool has_data_at(size_type column, size_type row) const noexcept {
      return column < width_ && row < height_;
    }

    const_reference at(size_type column, size_type row) const {
      throw_if_invalid_index_(column, row);
      return data_.at(index_of_(column, row));
    }

    reference at(size_type column, size_type row) {
      throw_if_invalid_index_(column, row);
      return data_.at(index_of_(column, row));
    }

    private:
    size_type width_;
    size_type height_;
    container_type_ data_;

    inline size_type index_of_(size_type column, size_type row) const noexcept {
      return width_ * row + column;
    }

    void throw_if_invalid_index_(size_type column, size_type row) const {
      if (!has_data_at(column, row)) {
        std::ostringstream oss;
        oss << "Index (" << column << ", " << row
          << ") out of range for Grid with dimensions ("
          << width_ << ", " << height_ << ")";
        throw std::out_of_range(oss.str());
      }
    }
  };
} // namespace conway_gol
