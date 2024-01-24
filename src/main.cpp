#include <iostream>
#include <string>

void hide_cursor(std::ostream& out) {
  out << "\033[?25l";
}

void show_cursor(std::ostream& out) {
  out << "\033[?25h";
}

void clear_screen(std::ostream& out) {
  out << "\033[;H\033[2J";
}

void enable_alt_buffer(std::ostream& out) {
  out << "\033[?1049h";
}

void disable_alt_buffer(std::ostream& out) {
  out << "\033[?1049l";
}

int main(int argc, char** argv) {
  int state = 0;
  auto& in = std::cin;
  auto& out = std::cout;

  enable_alt_buffer(out);
  hide_cursor(out);
  out.flush();

  std::string line;
  while (in.good()) {
    clear_screen(out);
    out << state++;
    out.flush();

    std::getline(in, line);
  }

  show_cursor(out);
  disable_alt_buffer(out);
  out.flush();

  return EXIT_SUCCESS;
}
