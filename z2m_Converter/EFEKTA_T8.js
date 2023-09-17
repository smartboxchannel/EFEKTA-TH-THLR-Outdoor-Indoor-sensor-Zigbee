// ############################################################################//
//                                                                             //
//    ... перезагрузить z2m, что бы конвертер применился                       //
//                                                                             //
//#############################################################################//


const fz = require('zigbee-herdsman-converters/converters/fromZigbee');
const tz = require('zigbee-herdsman-converters/converters/toZigbee');
const exposes = require('zigbee-herdsman-converters/lib/exposes');
const constants = require('zigbee-herdsman-converters/lib/constants');
const reporting = require('zigbee-herdsman-converters/lib/reporting');
const extend = require('zigbee-herdsman-converters/lib/extend');
const e = exposes.presets;
const ea = exposes.access;
const {calibrateAndPrecisionRoundOptions} = require('zigbee-herdsman-converters/lib/utils');
const {postfixWithEndpointName} = require('zigbee-herdsman-converters/lib/utils');


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
			if (msg.data.hasOwnProperty(0xA199)) {
                result.number_of_sensors = msg.data[0xA199];
            }
            return result;
        },
    },
	temperaturef_config: {
        cluster: 'msTemperatureMeasurement',
        type: ['attributeReport', 'readResponse'],
        convert: (model, msg, publish, options, meta) => {
            const serial_id_ds18b20 = msg.data[0xA19B];
            const prope2 = postfixWithEndpointName('serial_id_ds18b20', msg, model, meta);
			return {[prope2]: serial_id_ds18b20};
        },
    },
	temperaturef_config2: {
        cluster: 'msTemperatureMeasurement',
        type: ['attributeReport', 'readResponse'],
        convert: (model, msg, publish, options, meta) => {
		    const sensId2 = msg.data[0xA19B];
			const temperature = parseFloat(msg.data['measuredValue']) / 100.0;
			return {[sensId2] : temperature};
        },
    },
	uptime: {
        cluster: 'genTime',
        type: ['attributeReport', 'readResponse'],
        convert: (model, msg, publish, options, meta) => {
            //return {uptime: Math.round(msg.data.localTime/60)};
			if (msg.data.hasOwnProperty('localTime')) {
				return {uptime: Math.round(msg.data.localTime/60)};
			}
        },
    },
};

const definition = {
        zigbeeModel: ['EFEKTA_T8'],
        model: 'EFEKTA_T8',
        vendor: 'EFEKTALAB',
        description: 'EFEKTA_T8 - Temperature sensor, support up to 8 DS18B20 sensors on one line. Self-contained, powered by 2 AAA batteries',
        fromZigbee: [fz.temperature, fz.battery, fzLocal.temperaturef_config, fzLocal.temperaturef_config2, fzLocal.node_config , fzLocal.uptime],
        toZigbee: [tz.factory_reset, tzLocal.node_config],
		meta: {multiEndpoint: true},
        configure: async (device, coordinatorEndpoint, logger) => {
            const endpoint = device.getEndpoint(1);
            const clusters = ['genPowerCfg', 'genTime', 'msTemperatureMeasurement'];
			await reporting.bind(endpoint, coordinatorEndpoint, clusters);
			const endpoint2 = device.getEndpoint(2);
            const clusters2 = ['msTemperatureMeasurement'];
			await reporting.bind(endpoint2, coordinatorEndpoint, clusters2);
			const endpoint3 = device.getEndpoint(3);
            const clusters3 = ['msTemperatureMeasurement'];
			await reporting.bind(endpoint3, coordinatorEndpoint, clusters3);
			const endpoint4 = device.getEndpoint(4);
            const clusters4 = ['msTemperatureMeasurement'];
			await reporting.bind(endpoint4, coordinatorEndpoint, clusters4);
			const endpoint5 = device.getEndpoint(5);
            const clusters5 = ['msTemperatureMeasurement'];
			await reporting.bind(endpoint5, coordinatorEndpoint, clusters5);
			const endpoint6 = device.getEndpoint(6);
            const clusters6 = ['msTemperatureMeasurement'];
			await reporting.bind(endpoint6, coordinatorEndpoint, clusters6);
			const endpoint7 = device.getEndpoint(7);
            const clusters7 = ['msTemperatureMeasurement'];
			await reporting.bind(endpoint7, coordinatorEndpoint, clusters7);
			const endpoint8 = device.getEndpoint(8);
            const clusters8 = ['msTemperatureMeasurement'];
			await reporting.bind(endpoint8, coordinatorEndpoint, clusters8);
        },
        exposes: [e.temperature().withEndpoint('1').withDescription('Temperature DS18B20'), 
		    e.temperature().withEndpoint('2').withDescription('Temperature DS18B20'), 
			e.temperature().withEndpoint('3').withDescription('Temperature DS18B20'), 
			e.temperature().withEndpoint('4').withDescription('Temperature DS18B20'), 
			e.temperature().withEndpoint('5').withDescription('Temperature DS18B20'), 
			e.temperature().withEndpoint('6').withDescription('Temperature DS18B20'), 
			e.temperature().withEndpoint('7').withDescription('Temperature DS18B20'), 
			e.temperature().withEndpoint('8').withDescription('Temperature DS18B20'), 
		    exposes.numeric('serial_id_ds18b20', ea.STATE).withEndpoint('1').withDescription('Serial ID DS18B20'),
			exposes.numeric('serial_id_ds18b20', ea.STATE).withEndpoint('2').withDescription('Serial ID DS18B20'),
			exposes.numeric('serial_id_ds18b20', ea.STATE).withEndpoint('3').withDescription('Serial ID DS18B20'),
			exposes.numeric('serial_id_ds18b20', ea.STATE).withEndpoint('4').withDescription('Serial ID DS18B20'),
			exposes.numeric('serial_id_ds18b20', ea.STATE).withEndpoint('5').withDescription('Serial ID DS18B20'),
			exposes.numeric('serial_id_ds18b20', ea.STATE).withEndpoint('6').withDescription('Serial ID DS18B20'),
			exposes.numeric('serial_id_ds18b20', ea.STATE).withEndpoint('7').withDescription('Serial ID DS18B20'),
			exposes.numeric('serial_id_ds18b20', ea.STATE).withEndpoint('8').withDescription('Serial ID DS18B20'),
			exposes.numeric('number_of_sensors', ea.STATE).withDescription('Number of ds18b20 sensors per data bus'),
		    e.battery(), e.battery_low(), e.battery_voltage(),
		    exposes.numeric('uptime', ea.STATE).withUnit('Minutes').withDescription('Uptime'),
		    exposes.numeric('report_delay', ea.STATE_SET).withUnit('Seconds').withDescription('Adjust Report Delay. Setting the time in seconds')
                .withValueMin(20).withValueMax(3600)],
};

module.exports = definition;