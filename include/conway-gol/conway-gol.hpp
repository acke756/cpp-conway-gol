#pragma once

#include <ostream>
#include <vector>

class ConwayGol {
  public:
  typedef std::vector<bool>::size_type size_type;

  ConwayGol(size_type width, size_type height);

  void update();

  friend std::ostream& operator<<(std::ostream& os, const ConwayGol& gol);

  private:
  size_type width_;
  size_type height_;
  std::vector<bool> data_;
};

std::ostream& operator<<(std::ostream& os, const ConwayGol& gol);
