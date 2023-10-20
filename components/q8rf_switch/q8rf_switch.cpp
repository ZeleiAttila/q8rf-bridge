#include "esphome/core/log.h"
#include "limits.h"
#include "q8rf_switch.h"

namespace esphome
{
    namespace q8rf
    {

        static const char *TAG = "q8rf.switch";

        void Q8RFSwitch::setup()
        {
            ESP_LOGCONFIG(TAG, "setup");
        }

        void Q8RFSwitch::write_state(bool state)
        {

            ESP_LOGCONFIG(TAG, "state: %s", state);
            if (this->state_ != state)
            {
                this->set_state(state);
            }
        }

        void Q8RFSwitch::set_state(bool state)
        {
            this->state_ = state;
            this->publish_state(state);
        }

        void Q8RFSwitch::dump_config()
        {
        }

        void Q8RFSwitch::update()
        {
            ESP_LOGCONFIG(TAG, "update");
        }

        void Q8RFSwitch::set_on_message(char *on_message) { this->on_message_ = on_message; }
        void Q8RFSwitch::set_off_message(char *off_message) { this->off_message_ = off_message; }

    } // namespace q8rf
} // namespace esphome
