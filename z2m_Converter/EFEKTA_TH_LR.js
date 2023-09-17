const fz = require('zigbee-herdsman-converters/converters/fromZigbee');
const tz = require('zigbee-herdsman-converters/converters/toZigbee');
const exposes = require('zigbee-herdsman-converters/lib/exposes');
const reporting = require('zigbee-herdsman-converters/lib/reporting');
const extend = require('zigbee-herdsman-converters/lib/extend');
const e = exposes.presets;
const ea = exposes.access;

const tzLocal = {
    node_config: {
        key: ['report_delay', 'comparison_previous_data'],
        convertSet: async (entity, key, rawValue, meta) => {
            const lookup = {'OFF': 0x00, 'ON': 0x01};
            const value = lookup.hasOwnProperty(rawValue) ? lookup[rawValue] : parseInt(rawValue, 10);
            const payloads = {
                report_delay: ['genPowerCfg', {0x0201: {value, type: 0x21}}],
				comparison_previous_data: ['genPowerCfg', {0x0205: {value, type: 0x10}}],
            };
            await entity.write(payloads[key][0], payloads[key][1]);
            return {
                state: {[key]: rawValue},
            };
        },
    },
	termostat_config: {
        key: ['high_temp', 'low_temp', 'enable_temp', 'invert_logic_temp'],
        convertSet: async (entity, key, rawValue, meta) => {
            const lookup = {'OFF': 0x00, 'ON': 0x01};
            const value = lookup.hasOwnProperty(rawValue) ? lookup[rawValue] : parseInt(rawValue, 10);
            const payloads = {
                high_temp: ['msTemperatureMeasurement', {0x0221: {value, type: 0x29}}],
                low_temp: ['msTemperatureMeasurement', {0x0222: {value, type: 0x29}}],
				enable_temp: ['msTemperatureMeasurement', {0x0220: {value, type: 0x10}}],
				invert_logic_temp: ['msTemperatureMeasurement', {0x0225: {value, type: 0x10}}],
            };
            await entity.write(payloads[key][0], payloads[key][1]);
            return {
                state: {[key]: rawValue},
            };
        },
    },
	hydrostat_config: {
        key: ['high_hum', 'low_hum', 'enable_hum', 'invert_logic_hum'],
        convertSet: async (entity, key, rawValue, meta) => {
            const lookup = {'OFF': 0x00, 'ON': 0x01};
            const value = lookup.hasOwnProperty(rawValue) ? lookup[rawValue] : parseInt(rawValue, 10);
            const payloads = {
                high_hum: ['msRelativeHumidity', {0x0221: {value, type: 0x21}}],
                low_hum: ['msRelativeHumidity', {0x0222: {value, type: 0x21}}],
				enable_hum: ['msRelativeHumidity', {0x0220: {value, type: 0x10}}],
				invert_logic_hum: ['msRelativeHumidity', {0x0225: {value, type: 0x10}}],
            };
            await entity.write(payloads[key][0], payloads[key][1]);
            return {
                state: {[key]: rawValue},
            };
        },
    },
};

const fzLocal = {
    node_config: {
        cluster: 'genPowerCfg',
        type: ['attributeReport', 'readResponse'],
        convert: (model, msg, publish, options, meta) => {
            const result = {};
            if (msg.data.hasOwnProperty(0x0201)) {
                result.report_delay = msg.data[0x0201];
            }
			if (msg.data.hasOwnProperty(0x0205)) {
				result.comparison_previous_data = ['OFF', 'ON'][msg.data[0x0205]];
            }
            return result;
        },
    },
	termostat_config: {
        cluster: 'msTemperatureMeasurement',
        type: ['attributeReport', 'readResponse'],
        convert: (model, msg, publish, options, meta) => {
            const result = {};
            if (msg.data.hasOwnProperty(0x0221)) {
                result.high_temp = msg.data[0x0221];
            }
			if (msg.data.hasOwnProperty(0x0222)) {
                result.low_temp = msg.data[0x0222];
            }
            if (msg.data.hasOwnProperty(0x0220)) {
                result.enable_temp = ['OFF', 'ON'][msg.data[0x0220]];
            }
			if (msg.data.hasOwnProperty(0x0225)) {
                result.invert_logic_temp = ['OFF', 'ON'][msg.data[0x0225]];
            }
            return result;
        },
    },
	hydrostat_config: {
        cluster: 'msRelativeHumidity',
        type: ['attributeReport', 'readResponse'],
        convert: (model, msg, publish, options, meta) => {
            const result = {};
            if (msg.data.hasOwnProperty(0x0221)) {
                result.high_hum = msg.data[0x0221];
            }
			if (msg.data.hasOwnProperty(0x0222)) {
                result.low_hum = msg.data[0x0222];
            }
            if (msg.data.hasOwnProperty(0x0220)) {
                result.enable_hum = ['OFF', 'ON'][msg.data[0x0220]];
            }
			if (msg.data.hasOwnProperty(0x0225)) {
                result.invert_logic_hum = ['OFF', 'ON'][msg.data[0x0225]];
            }
            return result;
        },
    },
};

const definition = {
        zigbeeModel: ['EFEKTA_TH_LR'],
        model: 'EFEKTA_TH_LR',
        vendor: 'EfektaLab',
        description: 'EFEKTA_TH_LR - temperature and humidity sensors with increased battery life',
        fromZigbee: [fz.temperature, fz.humidity, fz.battery, fzLocal.termostat_config, fzLocal.hydrostat_config, fzLocal.node_config],
        toZigbee: [tz.factory_reset, tzLocal.termostat_config, tzLocal.hydrostat_config, tzLocal.node_config],
        configure: async (device, coordinatorEndpoint, logger) => {
            const endpointOne = device.getEndpoint(1);
            await reporting.bind(endpointOne, coordinatorEndpoint, ['genPowerCfg', 'msTemperatureMeasurement', 'msRelativeHumidity']);
			const overides = {min: 6, max: 66, change: 1};
			await reporting.temperature(endpointOne, overides);
        },
        exposes: [e.temperature(), e.humidity(), e.battery(), e.battery_low(), e.battery_voltage(),
		    exposes.numeric('report_delay', ea.STATE_SET).withUnit('Seconds').withDescription('Adjust Report Delay. Setting the time in seconds, by default 60 seconds')
                .withValueMin(10).withValueMax(3600),
		    exposes.binary('comparison_previous_data', ea.STATE_SET, 'ON', 'OFF').withDescription('Enable сontrol of comparison with previous data'),
		    exposes.binary('enable_temp', ea.STATE_SET, 'ON', 'OFF').withDescription('Enable Temperature Control'),
		    exposes.binary('invert_logic_temp', ea.STATE_SET, 'ON', 'OFF').withDescription('Invert Logic Temperature Control'),
            exposes.numeric('high_temp', ea.STATE_SET).withUnit('C').withDescription('Setting High Temperature Border')
                .withValueMin(-5).withValueMax(50),
            exposes.numeric('low_temp', ea.STATE_SET).withUnit('C').withDescription('Setting Low Temperature Border')
                .withValueMin(-5).withValueMax(50),				
		    exposes.binary('enable_hum', ea.STATE_SET, 'ON', 'OFF').withDescription('Enable Humidity Control'),
		    exposes.binary('invert_logic_hum', ea.STATE_SET, 'ON', 'OFF').withDescription('Invert Logoc Humidity Control'),
            exposes.numeric('high_hum', ea.STATE_SET).withUnit('C').withDescription('Setting High Humidity Border')
                .withValueMin(0).withValueMax(99),
            exposes.numeric('low_hum', ea.STATE_SET).withUnit('C').withDescription('Setting Low Humidity Border')
                .withValueMin(0).withValueMax(99)],
};

module.exports = definition;