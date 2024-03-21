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

    typedef struct coordinate {
      size_type column, row;

      friend inline bool operator==(const coordinate& lhs, const coordinate& rhs) {
        return lhs.column == rhs.column && lhs.row == rhs.row;
      }

      friend inline bool operator!=(const coordinate& lhs, const coordinate& rhs) {
        return !(lhs == rhs);
      }
    } coordinate;

    Grid():
        Grid(0, 0) {
    }

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

    ~Grid() = default;

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

    inline bool has_data_at(const coordinate& c) const noexcept {
      return c.column < width_ && c.row < height_;
    }

    const_reference at(const coordinate& c) const {
      throw_if_invalid_(c);
      return data_.at(index_of_(c));
    }

    reference at(const coordinate& c) {
      throw_if_invalid_(c);
      return data_.at(index_of_(c));
    }

    private:
    size_type width_;
    size_type height_;
    container_type_ data_;

    inline size_type index_of_(const coordinate& c) const noexcept {
      return width_ * c.row + c.column;
    }

    void throw_if_invalid_(const coordinate& c) const {
      if (!has_data_at(c)) {
        std::ostringstream oss;
        oss << "Index (" << c.column << ", " << c.row
          << ") out of range for Grid with dimensions ("
          << width_ << ", " << height_ << ")";
        throw std::out_of_range(oss.str());
      }
    }
  };
} // namespace conway_gol
