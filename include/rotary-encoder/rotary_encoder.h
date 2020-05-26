
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

class RotaryEncoder {
public:
  /**
   * Constructor to create Rotary Encoder instance.
   *
   * @param gpio GPIO pin where the encoder is connected.
   * @param rotationPulsea Number of encoder pulses per rotation.
   */
  RotaryEncoder(int gpio, int rotationPulses);

  /**
   * API to read RPM.
   *
   * @param order order for moving average order (order 0 = unfiltered); Order
   * cannot be higher than 1/4 of rotationPulses.
   */
  double rpm(int order);

private:
  /**
   * this function blocks until gpio was toggled and inserts the current
   * timestamp into the buffer.
   *
   * @param gpio order for moving average order (order 0 = unfiltered).
   */
  void poll_gpio(int gpio);

  const int _gpio;
  const int _rotationPulses;
  const int _buffSize;

  std::thread polling_thread;
  std::deque<std::chrono::steady_clock::time_point> _buffer;
  std::mutex _bufferMutex;
};
