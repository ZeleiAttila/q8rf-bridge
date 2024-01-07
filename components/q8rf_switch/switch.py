import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.components import spi
from esphome.const import CONF_ID


ON_MESSAGE = "on_message"
OFF_MESSAGE = "off_message"
POLL_INTERVAL = "poll_interval"
SAMPLING_INTERVAL = "sampling_interval"

q8rf_ns = cg.esphome_ns.namespace("q8rf")
q8rf_SWITCH = q8rf_ns.class_("Q8RFSwitch",  switch.Switch, cg.PollingComponent)

CONFIG_SCHEMA = (
    switch.SWITCH_SCHEMA.extend({
        cv.GenerateID(): cv.declare_id(q8rf_SWITCH),
    }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(
        cv.Schema(
            {
                cv.Required(ON_MESSAGE): cv.string,
                cv.Required(OFF_MESSAGE): cv.string,
                cv.Optional(POLL_INTERVAL): cv.uint32_t,
                cv.Optional(SAMPLING_INTERVAL): cv.uint32_t
            }
        )
    )
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await switch.register_switch(var, config)

    cg.add(var.set_on_message(config[ON_MESSAGE]))
    cg.add(var.set_off_message(config[OFF_MESSAGE]))

    if POLL_INTERVAL in config:
        cg.add(var.set_poll_interval(config[POLL_INTERVAL]))

    if SAMPLING_INTERVAL in config:
        cg.add(var.set_sampling_interval(config[SAMPLING_INTERVAL]))

    cg.add(var.setup())