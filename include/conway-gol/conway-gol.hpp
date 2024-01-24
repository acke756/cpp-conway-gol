#pragma once

#include <ostream>

class ConwayGol {
  int state_ = 0;

  public:
  ConwayGol();

  void update();

  friend std::ostream& operator<<(std::ostream& os, const ConwayGol& gol);
};

std::ostream& operator<<(std::ostream& os, const ConwayGol& gol);
