
#include <iostream>
#include <thread>
#include <chrono>
#include <deque>
#include <cstdlib>


#include "rotary-encoder/rotary_encoder.h"

int main(int argc, char **argv) {

  std::cout << "Created rotary encoder" << std::endl;
  RotaryEncoder rotaryEncoderRight(50,48*4);

  // std::cout << "Start rotary encoder" << std::endl;
  // rotaryEncoderRight.run();

  std::cout << "Start reading encoder values" << std::endl;
  while (true) {
    std::cout << "RPM MA: " << rotaryEncoderRight.rpm(atoi(argv[1])) << std::endl;
    std::cout << "RPM raw: " << rotaryEncoderRight.rpm(0) << std::endl;
    std::this_thread::sleep_for (std::chrono::milliseconds(1000));
  }

  return 0;
}
