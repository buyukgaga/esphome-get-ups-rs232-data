#include "esphome.h"

class MakelsanUPS : public Component, public UARTDevice
{
public:
	BinarySensor* battery_low = new BinarySensor();
	BinarySensor* beeper_on = new BinarySensor();
	BinarySensor* eco_mode = new BinarySensor();
	BinarySensor* power_status = new BinarySensor();
	BinarySensor* shutdown_active = new BinarySensor();
	BinarySensor* test_in_progress = new BinarySensor();
	BinarySensor* ups_failed = new BinarySensor();
	BinarySensor* ups_type_is_standby = new BinarySensor();
	BinarySensor* utility_fail = new BinarySensor();
	Sensor* battery_capacity = new Sensor();
	Sensor* battery_voltage = new Sensor();
	Sensor* battery_voltage_current = new Sensor();
	Sensor* internal_tempeture = new Sensor();
	Sensor* output_current = new Sensor();
	Sensor* output_power = new Sensor();
	Sensor* raiting_current = new Sensor();
	Sensor* raiting_voltage = new Sensor();
	Sensor* voltage_frequency = new Sensor();
	Sensor* voltage_input = new Sensor();
	Sensor* voltage_input_fault = new Sensor();
	Sensor* voltage_input_frequency = new Sensor();
	Sensor* voltage_output = new Sensor();
	TextSensor* power_type = new TextSensor();
	TextSensor* ups_type = new TextSensor();

	MakelsanUPS(UARTComponent* parent) : Component(), UARTDevice(parent) {}

	int readline(int readch, char* buffer, int len)
	{
		static int pos = 0;
		int rpos;

		if (readch > 0)
		{
			switch (readch)
			{
			case '#': // "#220.0 005 024.0 50.0" Clear #.
			case '(': // "(216.5 000.0 219.6 027 50.0 2.26 40.00 00110000" Clear (.
				break;
			case '\r': // Return on CR
				rpos = pos;
				pos = 0; // Reset position index ready for next time
				return rpos;
			default:
				if (pos < len - 1)
				{
					buffer[pos++] = readch;
					buffer[pos] = 0;
				}
			}
		}
		// No end of line has been found, so return -1.
		return -1;
	}

	float get_battery_capacity(float value)
	{
		return (value * 43.66812227074236);
	}

	bool options_value(char value)
	{
		return (value == 48 ? false : (value == 49 ? true : false));
	}

	void binarySensor(BinarySensor* sensor, bool value)
	{
		if (sensor->state != value)
		{
			sensor->publish_state(value);
		}
	}

	void sensor(Sensor* sensor, float value)
	{
		if (sensor->state != value)
		{
			sensor->publish_state(value);
		}
	}

	void textSensor(TextSensor* sensor, char* value)
	{
		if (sensor->state != value)
		{
			sensor->publish_state(value);
		}
	}

	void setup() override
	{
		// nothing to do here
		binarySensor(battery_low, -1);
		binarySensor(beeper_on, -1);
		binarySensor(eco_mode, -1);
		binarySensor(power_status, -1);
		binarySensor(shutdown_active, -1);
		binarySensor(test_in_progress, -1);
		binarySensor(ups_failed, -1);
		binarySensor(ups_type_is_standby, -1);
		binarySensor(utility_fail, -1);
	}

	void loop() override
	{
		const int max_line_length = 50;
		static char buffer[max_line_length]{};
		while (available())
		{
			if (readline(read(), buffer, max_line_length) > 0)
			{
				//ESP_LOGD("UART Data Line : STRING", "%s", buffer);
				//ESP_LOGD("UART Data Line : INTEGER", "%i", strlen(buffer));
				const char* delimiter = " ";
				switch (strlen(buffer))
				{
				case 20: //"220.0 005 024.0 50.0"
					sensor(raiting_voltage, atof(strtok(buffer, delimiter)));
					sensor(raiting_current, atof(strtok(NULL, delimiter)));
					sensor(battery_voltage, atof(strtok(NULL, delimiter)));
					sensor(voltage_frequency, atof(strtok(NULL, delimiter)));
					break;
				case 45: //"216.5 000.0 219.6 027 50.0 2.26 40.00 00110000"
					sensor(voltage_input, atof(strtok(buffer, delimiter)));
					sensor(voltage_input_fault, atof(strtok(NULL, delimiter)));
					sensor(voltage_output, atof(strtok(NULL, delimiter)));
					sensor(output_current, atof(strtok(NULL, delimiter)));
					sensor(voltage_input_frequency, atof(strtok(NULL, delimiter)));
					const float battery_voltage_float = atof(strtok(NULL, delimiter));
					sensor(internal_tempeture, atof(strtok(NULL, delimiter)));
					static char other_values[9]{};
					strcpy(other_values, strtok(NULL, delimiter));
					binarySensor(battery_low, options_value(other_values[1]));
					binarySensor(beeper_on, options_value(other_values[7]));
					binarySensor(eco_mode, options_value(other_values[2]));
					binarySensor(utility_fail, options_value(other_values[0]));
					binarySensor(shutdown_active, options_value(other_values[6]));
					binarySensor(test_in_progress, options_value(other_values[5]));
					binarySensor(ups_failed, options_value(other_values[3]));
					binarySensor(ups_type_is_standby, options_value(other_values[4]));
					sensor(battery_capacity, get_battery_capacity(battery_voltage_float));
					sensor(battery_voltage_current, battery_voltage_float);
					binarySensor(power_status, (voltage_input->state == 0 ? false : true));
					sensor(output_power, (output_current->state * 6.111111));
					textSensor(power_type, (char*)(voltage_input->state > 0 ? "AC Utility Power" : "Battery Power"));
					textSensor(ups_type, (char*)(voltage_input->state > 0 ? "On-Line" : "Normal"));
					break;
				}
			}
		}
	}
};
