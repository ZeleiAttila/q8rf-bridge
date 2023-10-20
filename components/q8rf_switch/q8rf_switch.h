#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "q8rf_controller.h"

namespace esphome
{
  namespace q8rf
  {

    class Q8RFSwitch : public switch_::Switch, public PollingComponent
    {
    protected:
      char *on_message = "";
      char *off_message = "";

    public:
      Q8RFSwitch()  {}
      void set_on_message( char *on_message );
      void set_off_message( char *off_message);
     
      void setup() override;
      void write_state(bool state) override;
      void dump_config() override;
      void update() override;
    };

  } // namespace q8rf
} // namespace esphome
