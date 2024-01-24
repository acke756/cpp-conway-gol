#include <iostream>
#include <string>

int main(int argc, char** argv) {
  int state = 0;
  auto& in = std::cin;

  std::string line;
  while (in.good()) {
    std::cout << state++;
    std::getline(in, line);
  }

  return EXIT_SUCCESS;
}
