
#include <chrono>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>

#include "rotary-encoder/rotary_encoder.h"

int main(int argc, char **argv) {

  std::cout << "Start rotary encoder" << std::endl;
  RotaryEncoder rotaryEncoderRight(14, 48 * 4, 5.2 * 10 * 1000);

  std::cout << "Start reading encoder values" << std::endl;
  while (true) {
    std::cout << "RPM MA: " << rotaryEncoderRight.rpm(atoi(argv[1])) << std::endl;
    std::cout << "RPM raw: " << rotaryEncoderRight.rpm(0) << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return 0;
}
