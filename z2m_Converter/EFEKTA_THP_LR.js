const fz = require('zigbee-herdsman-converters/converters/fromZigbee');
const tz = require('zigbee-herdsman-converters/converters/toZigbee');
const exposes = require('zigbee-herdsman-converters/lib/exposes');
const reporting = require('zigbee-herdsman-converters/lib/reporting');
const e = exposes.presets;
const ea = exposes.access;


const definition = {
        zigbeeModel: ['EFEKTA_THP_LR'],
        model: 'EFEKTA_THP_LR',
        vendor: 'Custom devices (DiY)',
        description: 'DIY outdoor long-range sensor for temperature, humidity and atmospheric pressure',
        fromZigbee: [fz.temperature, fz.humidity, fz.pressure, fz.battery],
        toZigbee: [tz.factory_reset],
        configure: async (device, coordinatorEndpoint, logger) => {
            const endpoint = device.getEndpoint(1);
            await reporting.bind(endpoint, coordinatorEndpoint, [
                'genPowerCfg', 'msTemperatureMeasurement', 'msRelativeHumidity', 'msPressureMeasurement']);
            const overides = {min: 0, max: 64800, change: 0};
            await reporting.batteryVoltage(endpoint, overides);
            await reporting.batteryPercentageRemaining(endpoint, overides);
            await reporting.temperature(endpoint, overides);
            await reporting.humidity(endpoint, overides);
            await reporting.pressureExtended(endpoint, overides);
            await endpoint.read('msPressureMeasurement', ['scale']);
        },
        exposes: [e.temperature(), e.humidity(), e.pressure(), e.battery(), e.battery_voltage()],
    },
};

module.exports = definition;
