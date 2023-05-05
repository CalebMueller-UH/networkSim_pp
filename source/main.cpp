#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  std::cout << "Yep" << std::endl;

#ifdef DEBUG
  std::cout << "YEPPI" << std::endl;
#endif

  return 0;
}