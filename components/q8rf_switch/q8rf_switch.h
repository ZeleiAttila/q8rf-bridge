#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include <string>

#define byte uint8_t
namespace esphome
{
  namespace q8rf
  {

    class Q8RFSwitch : public switch_::Switch, public Component
    {
    protected:
      std::string on_message_;
      std::string off_message_;
      bool state_ = false;
      void hextoascii(byte *ascii_ptr, byte *hex_ptr, int len);

    public:
      Q8RFSwitch() {}
      void set_on_message(std::string on_message);
      void set_off_message(std::string off_message);
      void setup();
      void update();

      void write_state(bool state) override;
      void dump_config() override;

      void set_state(bool state);
    };

  } // namespace q8rf
} // namespace esphome
