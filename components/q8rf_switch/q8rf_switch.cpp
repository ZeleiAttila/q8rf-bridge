#include "esphome/core/log.h"
#include "limits.h"
#include "q8rf_switch.h"
#include "ELECHOUSE_CC1101_SRC_DRV.h"
#include "esphome.h"
#include <string>

#define BUF_LENGTH 128
namespace esphome
{
    namespace q8rf
    {

        static const char *TAG = "q8rf.switch";

        byte textbuffer[BUF_LENGTH];
        byte sck = 14;  // GPIO 14
        byte miso = 12; // GPIO 12
        byte mosi = 13; // GPIO 13
        byte ss = 15;   // GPIO 15
        int gdo0 = 5;   // GPIO 5
        int gdo2 = 4;   // GPIO 4

        int len;

        unsigned long elapsed(unsigned long since, unsigned long now)
        {
            if (since > now)
            {
                // millis() overflows every ~50 days
                return (ULONG_MAX - since) + now;
            }
            else
            {
                return now - since;
            }
        }

        void Q8RFSwitch::setup()
        {
            ELECHOUSE_cc1101.setSpiPin(sck, miso, mosi, ss);
            ELECHOUSE_cc1101.setGDO(gdo0, gdo2);

            if (ELECHOUSE_cc1101.getCC1101())
            {
                ESP_LOGCONFIG(TAG, "Connection OK");
            }
            else
            {
                ESP_LOGCONFIG(TAG, "Connection Error");
            }

            // Main part to tune CC1101 with proper frequency, modulation and encoding
            ELECHOUSE_cc1101.Init();                // must be set to initialize the cc1101!
            ELECHOUSE_cc1101.setGDO0(gdo0);         // set lib internal gdo pin (gdo0). Gdo2 not use for this example.
            ELECHOUSE_cc1101.setCCMode(1);          // set config for internal transmission mode. value 0 is for RAW recording/replaying
            ELECHOUSE_cc1101.setModulation(2);      // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
            ELECHOUSE_cc1101.setMHZ(868.5);         // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
            ELECHOUSE_cc1101.setDeviation(47.60);   // Set the Frequency deviation in kHz. Value from 1.58 to 380.85. Default is 47.60 kHz.
            ELECHOUSE_cc1101.setChannel(0);         // Set the Channelnumber from 0 to 255. Default is cahnnel 0.
            ELECHOUSE_cc1101.setChsp(199.95);       // The channel spacing is multiplied by the channel number CHAN and added to the base frequency in kHz. Value from 25.39 to 405.45. Default is 199.95 kHz.
            ELECHOUSE_cc1101.setRxBW(812.50);       // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
            ELECHOUSE_cc1101.setDRate(9.6);         // Set the Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud!
            ELECHOUSE_cc1101.setPA(10);             // Set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
            ELECHOUSE_cc1101.setSyncMode(2);        // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
            ELECHOUSE_cc1101.setSyncWord(211, 145); // Set sync word. Must be the same for the transmitter and receiver. Default is 211,145 (Syncword high, Syncword low)
            ELECHOUSE_cc1101.setAdrChk(0);          // Controls address check configuration of received packages. 0 = No address check. 1 = Address check, no broadcast. 2 = Address check and 0 (0x00) broadcast. 3 = Address check and 0 (0x00) and 255 (0xFF) broadcast.
            ELECHOUSE_cc1101.setAddr(0);            // Address used for packet filtration. Optional broadcast addresses are 0 (0x00) and 255 (0xFF).
            ELECHOUSE_cc1101.setWhiteData(0);       // Turn data whitening on / off. 0 = Whitening off. 1 = Whitening on.
            ELECHOUSE_cc1101.setPktFormat(0);       // Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX. 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins. 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX. 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins.
            ELECHOUSE_cc1101.setLengthConfig(1);    // 0 = Fixed packet length mode. 1 = Variable packet length mode. 2 = Infinite packet length mode. 3 = Reserved
            ELECHOUSE_cc1101.setPacketLength(0);    // Indicates the packet length when fixed packet length mode is enabled. If variable packet length mode is used, this value indicates the maximum packet length allowed.
            ELECHOUSE_cc1101.setCrc(0);             // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
            ELECHOUSE_cc1101.setCRC_AF(0);          // Enable automatic flush of RX FIFO when CRC is not OK. This requires that only one packet is in the RXIFIFO and that packet length is limited to the RX FIFO size.
            ELECHOUSE_cc1101.setDcFilterOff(0);     // Disable digital DC blocking filter before demodulator. Only for data rates ≤ 250 kBaud The recommended IF frequency changes when the DC blocking is disabled. 1 = Disable (current optimized). 0 = Enable (better sensitivity).
            ELECHOUSE_cc1101.setManchester(0);      // Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable.
            ELECHOUSE_cc1101.setFEC(0);             // Enable Forward Error Correction (FEC) with interleaving for packet payload (Only supported for fixed packet length mode. 0 = Disable. 1 = Enable.
            ELECHOUSE_cc1101.setPRE(0);             // Sets the minimum number of preamble bytes to be transmitted. Values: 0 : 2, 1 : 3, 2 : 4, 3 : 6, 4 : 8, 5 : 12, 6 : 16, 7 : 24
            ELECHOUSE_cc1101.setPQT(0);             // Preamble quality estimator threshold. The preamble quality estimator increases an internal counter by one each time a bit is received that is different from the previous bit, and decreases the counter by 8 each time a bit is received that is the same as the last bit. A threshold of 4∙PQT for this counter is used to gate sync word detection. When PQT=0 a sync word is always accepted.
            ELECHOUSE_cc1101.setAppendStatus(0);    // When enabled, two status bytes will be appended to the payload of the packet. The status bytes contain RSSI and LQI values, as well as CRC OK.

            set_update_interval(300000);
            ESP_LOGCONFIG(TAG, "setup");
        }

        void Q8RFSwitch::write_state(bool state)
        {

            ESP_LOGCONFIG(TAG, "Send state %s: %s", name_.c_str(), state ? "on" : "off");

            if (this->state_ != state)
            {
                this->state_ = state;
                this->publish_state(state);
            }
            this->set_state(state);
        }

        void Q8RFSwitch::set_state(bool state)
        {
            const char *cmdline;
            cmdline = state ? on_message_.c_str() : off_message_.c_str();

            len = strlen(cmdline);
            hextoascii(textbuffer, (byte *)cmdline, len);
            len = len / 2;

            ELECHOUSE_cc1101.setCCMode(0);
            ELECHOUSE_cc1101.setPktFormat(3);
            ELECHOUSE_cc1101.SetTx();

            pinMode(gdo0, OUTPUT);
            for (int i = 1; i < len; i++)
            {
                byte receivedbyte = textbuffer[i];
                for (int j = 7; j > -1; j--)
                {
                    digitalWrite(gdo0, bitRead(receivedbyte, j));
                    delayMicroseconds(400);
                }
                ESP.wdtFeed();
            }

            ELECHOUSE_cc1101.setCCMode(1);
            ELECHOUSE_cc1101.setPktFormat(0);
            ELECHOUSE_cc1101.SetRx();

            ESP.wdtFeed();
            yield();
        }

        void Q8RFSwitch::dump_config()
        {
        }

        void Q8RFSwitch::update()
        {
            ESP_LOGD(TAG, "Resending last state.");
            this->write_state(state_);
        }

        void Q8RFSwitch::hextoascii(byte *ascii_ptr, byte *hex_ptr, int len)
        {
            byte i, j;
            for (i = 0; i < (len / 2); i++)
            {
                j = hex_ptr[i * 2];
                if ((j > 47) && (j < 58))
                    ascii_ptr[i] = (j - 48) * 16;
                if ((j > 64) && (j < 71))
                    ascii_ptr[i] = (j - 55) * 16;
                if ((j > 96) && (j < 103))
                    ascii_ptr[i] = (j - 87) * 16;
                j = hex_ptr[i * 2 + 1];
                if ((j > 47) && (j < 58))
                    ascii_ptr[i] = ascii_ptr[i] + (j - 48);
                if ((j > 64) && (j < 71))
                    ascii_ptr[i] = ascii_ptr[i] + (j - 55);
                if ((j > 96) && (j < 103))
                    ascii_ptr[i] = ascii_ptr[i] + (j - 87);
            };
            ascii_ptr[i++] = '\0';
        }

        void Q8RFSwitch::set_on_message(std::string on_message) { this->on_message_ = on_message; }
        void Q8RFSwitch::set_off_message(std::string off_message) { this->off_message_ = off_message; }

    } // namespace q8rf
} // namespace esphome
