#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome
{
  namespace q8rf
  {

    class Q8RFSwitch : public switch_::Switch, public Component
    {
    protected:
      char *on_message_;
      char *off_message_;
      bool state_ = false;

    public:
      Q8RFSwitch() {}
      void set_on_message(char *on_message);
      void set_off_message(char *off_message);
      void setup();
      void update();

      void write_state(bool state) override;
      void dump_config() override;

      void set_state(bool state);
    };

  } // namespace q8rf
} // namespace esphome
