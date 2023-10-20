from esphome.cpp_generator import RawExpression
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID



ON_MESSAGE = "on_message"
OFF_MESSAGE = "off_message"

q8rf_ns = cg.esphome_ns.namespace("q8rf")
q8rf_SWITCH = q8rf_ns.class_("Q8RFSwitch", switch.Switch, cg.Component)

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
            }
        )
    )
)

