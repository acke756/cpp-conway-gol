#include <conway-gol/conway-gol.hpp>

// --- ConwayGol methods ---

ConwayGol::ConwayGol(): state_(0) {
}

void ConwayGol::update() {
  state_++;
}

// --- Non-member functions ---

std::ostream& operator<<(std::ostream& os, const ConwayGol& gol) {
  os << gol.state_;
  return os;
}
