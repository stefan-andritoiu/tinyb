/*
 * Author: Petre Eftime <petre.p.eftime@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <tinyb.hpp>

#include <vector>
#include <iostream>
#include <thread>

using namespace tinyb;

/** Converts a raw temperature read from the sensor to a Celsius value.
 * @param[in] raw_temp The temperature read from the sensor (two bytes)
 * @return The Celsius value of the temperature
 */
static float celsius_temp(uint16_t raw_temp)
{
    const float SCALE_LSB = 0.03125;
    return ((float)(raw_temp >> 2)) * SCALE_LSB;
}

/** This program reads the temperature from a
 * TI Sensor Tag(http://www.ti.com/ww/en/wireless_connectivity/sensortag2015/?INTC=SensorTag&HQS=sensortag)
 * Pass the MAC address of the sensor as the first parameter of the program.
 */
int main(int argc, char **argv)
{
    BluetoothManager *manager = BluetoothManager::get_bluetooth_manager();

    if (argc < 2) {
        std::cerr << "Run as: " << argv[0] << " <device_address>" << std::endl;
        exit(1);
    }

    /* Start the discovery of devices */
    bool ret = manager->start_discovery();
    std::cout << "Started = " << (ret ? "true" : "false") << std::endl;

    BluetoothDevice *sensor_tag = NULL;
    BluetoothGattService *temperature_service = NULL;

    for (int i = 0; i < 15; ++i) {
        std::cout << "Discovered devices: " << std::endl;
        /* Get the list of devices */
        auto list = manager->get_devices();

        for (auto it = list.begin(); it != list.end(); ++it) {

            std::cout << "Class = " << (*it)->get_class_name() << " ";
            std::cout << "Path = " << (*it)->get_object_path() << " ";
            std::cout << "Name = " << (*it)->get_name() << " ";
            std::cout << "Connected = " << (*it)->get_connected() << " ";
            std::cout << std::endl;

            /* Search for the device with the address given as a parameter to the program */
            if ((*it)->get_address() == argv[1])
                sensor_tag = (*it).release();
        }

        /* Free the list of devices and stop if the device was found */
        if (sensor_tag != NULL)
            break;
        /* If not, wait and try again */
        std::this_thread::sleep_for(std::chrono::seconds(4));
        std::cout << std::endl;
    }

    /* Stop the discovery (the device was found or number of tries ran out */
    ret = manager->stop_discovery();
    std::cout << "Stopped = " << (ret ? "true" : "false") << std::endl;

    if (sensor_tag != NULL) {
        /* Connect to the device and get the list of services exposed by it */
        sensor_tag->connect();
        std::cout << "Discovered services: " << std::endl;
        std::vector<std::unique_ptr<BluetoothGattService>> list;
        do {
            /* Wait for the device to come online */
            std::this_thread::sleep_for(std::chrono::seconds(4));

            list = sensor_tag->get_services();

            for (auto it = list.begin(); it != list.end(); ++it) {
                std::cout << "Class = " << (*it)->get_class_name() << " ";
                std::cout << "Path = " << (*it)->get_object_path() << " ";
                std::cout << "UUID = " << (*it)->get_uuid() << " ";
                std::cout << "Device = " << (*it)->get_device().get_object_path() << " ";
                std::cout << std::endl;

                /* Search for the temperature service, by UUID */
                if ((*it)->get_uuid() == "f000aa00-0451-4000-b000-000000000000")
                    temperature_service = (*it).release();
            }
        } while (list.empty());
    }

    BluetoothGattCharacteristic *temp_value = NULL;
    BluetoothGattCharacteristic *temp_config = NULL;
    BluetoothGattCharacteristic *temp_period = NULL;

    if (temperature_service != NULL) {
        /* If there is a temperature service on the device with the given UUID,
         * get it's characteristics, by UUID again */
        auto list = temperature_service->get_characteristics();
        std::cout << "Discovered characteristics: " << std::endl;
        for (auto it = list.begin(); it != list.end(); ++it) {

            std::cout << "Class = " << (*it)->get_class_name() << " ";
            std::cout << "Path = " << (*it)->get_object_path() << " ";
            std::cout << "UUID = " << (*it)->get_uuid() << " ";
            std::cout << "Service = " << (*it)->get_service().get_object_path() << " ";
            std::cout << std::endl;

            if ((*it)->get_uuid() == "f000aa01-0451-4000-b000-000000000000")
                temp_value = (*it).release();
            else if ((*it)->get_uuid() =="f000aa02-0451-4000-b000-000000000000")
                temp_config = (*it).release();
            else if ((*it)->get_uuid() == "f000aa03-0451-4000-b000-000000000000")
                temp_period = (*it).release();
        }
    }

    if (temp_config != NULL && temp_value != NULL && temp_period != NULL) {
        /* Activate the temperature measurements */
        std::vector<unsigned char> config_on {0x01};
        temp_config->write_value(config_on);
        while (true) {
            /* Read temperature data and display it */
            std::vector<unsigned char> response = temp_value->read_value();
            unsigned char *data;
            unsigned int size = response.size();
            if (size > 0) {
                data = response.data();

                std::cout << "Raw data=[";
                for (unsigned i = 0; i < response.size(); i++)
                    std::cout << std::hex << static_cast<int>(data[i]) << ", ";
                std::cout << "] ";

                uint16_t ambient_temp, object_temp;
                object_temp = data[0] + (data[1] << 8);
                ambient_temp = data[2] + (data[3] << 8);

                std::cout << "Ambient temp: " << celsius_temp(ambient_temp) << "C ";
                std::cout << "Object temp: " << celsius_temp(object_temp) << "C ";
                std::cout << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    /* Disconnect from the device */
    if (sensor_tag != NULL)
        sensor_tag->disconnect();
}
