#include "rotary-encoder/rotary_encoder.h"
#include "gpiolib.h"

RotaryEncoder::RotaryEncoder(int gpio, int rotationPulses, int maxPulseTime)
    : _gpio(gpio), _rotationPulses(rotationPulses), _maxPulseTime(maxPulseTime), _buffSize(rotationPulses / 4), _terminate_thread(false) {

  // setup GPIO
  gpio_export(_gpio);
  gpio_direction(_gpio, 0);  // 0: input
  gpio_setedge(_gpio, 1, 0); // rising, falling
  _buffer.clear();

  // start thread for rotary encoder measurement
  _polling_thread = std::thread(&RotaryEncoder::poll_gpio, this, _gpio);
}

// API to read RPM, order for moving average order (order 0 = unfiltered)
double RotaryEncoder::rpm(int order = 0) {

  if (order > (_buffSize - 2) || order < 0)
    return -1;

  std::lock_guard<std::mutex> lck(_bufferMutex); // protect the _buffer
  // check if at least two timestamps availalbe, needed to calcualte time
  // difference, if order greater than 0 more elements required std::cout <<
  // "buffer size: " << _buffer.size() << std::endl;

  if (_buffer.size() >= (2 + order)) {
    std::chrono::steady_clock::time_point t_latest = _buffer.at(0);        // latest timestamp
    std::chrono::steady_clock::time_point t_second_latest = _buffer.at(1); // second latest timestamp
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    if (std::chrono::duration_cast<std::chrono::microseconds>(now - t_latest).count() < _maxPulseTime) { // check if standing still (last pulse
                                                                                                         // later than maxPulseTime)
      double duration = double(std::chrono::duration_cast<std::chrono::microseconds>(t_latest - t_second_latest).count()) / (order + 1);

      // apply moving average
      for (int i = 1; i <= order; i++) {
        // std::cout << "duration: " << duration << std::endl;
        t_latest = _buffer.at(0 + i);
        t_second_latest = _buffer.at(1 + i);
        duration += double(std::chrono::duration_cast<std::chrono::microseconds>(t_latest - t_second_latest).count()) / (order + 1);
      }
      return 1000000 * 60 / (duration * _rotationPulses); // 1000000 because the timestamps are in
                                                          // microseconds; 60 to get to minutes
    }
    _buffer.clear();
  }
  return 0;
}

RotaryEncoder::~RotaryEncoder() {
  _terminate_thread = true;
  std::cout << "Terminate thread" << std::endl;
  _polling_thread.join();
}

void RotaryEncoder::poll_gpio(int gpio) {

  while (!_terminate_thread) {

    gpio_select(gpio); // block until gpio was toggled

    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    int input = gpio_read(gpio);

    if (input) { // on rising edge (read gpio to verify if rising or
                 // falling event (setting edge didn't work))
      std::lock_guard<std::mutex> lck(_bufferMutex);
      _buffer.push_front(now);

      // std::cout << "buffer size: " << _buffer.size() << std::endl;
      while (_buffer.size() > _buffSize) {

        _buffer.pop_back();
      }
    }
  }
}