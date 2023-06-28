/*
 * Configuration.h
 *
 *  Created on: 07.05.2023
 *      Author: Maik
 */
#pragma once

#include "data/Workpiece.h"
#include <string>
#include <vector>


#define DEFAULT_CONFIG_FILE_PATH "/tmp/esep_2.1/esep_conf.txt"
#define LINENUM_ORDER            1
#define LINENUM_OFFSET           2
#define LINENUM_REF              3

struct Calibration {
    int calOffset;
    int calRef;
};

class Configuration {
  public:
    static Configuration &getInstance() {
        static Configuration instance;
        return instance;
    }
    void setConfigFilePath(std::string filePath);

    /**
     * Reads the configuration from the set config file path (or default path).
     * If the file does not exist, a new one will be created with default
     * values.
     *
     * Structure of the file:
     * 1 | ORDER=[Desired Workpiece Order]
     * 2 | CAL_OFFSET=[Calibrated ADC offset value for HeightSensor]
     * 3 | CAL_REF=[Calibrated ADC reference value (@ 25.0 mm) for HeightSensor]
     *
     * @return true if reading the file was successful.
     */
    bool readConfigFromFile();

    /**
     * Sets the type of the system the Configuration is running on.
     *
     * @param isMaster true=Master, false=Slave
     */
    void setMaster(bool isMaster);

    /**
     * Gets the type of the system
     *
     * @return true if Master, false if Slave
     */
    bool systemIsMaster();

    /**
     * Sets if a pusher is mounted for sorting out.
     *
     * @param pusherIsMounted true: Pusher is mounted, false: Switch is mounted
     */
    void setPusherMounted(bool pusherIsMounted);

    /**
     * Checks if a pusher is mounted for sorting out.
     *
     * @return true if pusher is mounted, false: switch is mounted
     */
    bool pusherMounted();

    /**
     * Sets the desired workpiece order. Will usually be read out of the config
     * file
     *
     * @param order Desired workpiece order as list of 3 workpieces
     */
    void setDesiredWorkpieceOrder(std::vector<WorkpieceType> order);

    /**
     * Gets the desired order in which workpieces should arrive at the end of
     * FBM2
     *
     * @return the desired workpiece order
     */
    std::vector<WorkpieceType> getDesiredOrder();

    /**
     * Gets the currently calibrated values.
     *
     * @return current calibration
     */
    Calibration getCalibration();

    /**
     * Sets the calibrated value for ADC offset (belt height)
     *
     * @param offset Value to save for calibration
     */
    void setOffsetCalibration(int offset);

    /**
     * Sets the calibrated value for ADC reference (@ 25.0 mm)
     *
     * @param refHigh Value to save for calibration
     */
    void setReferenceCalibration(int refHigh);

    /**
     * Saves all currently stored configuration values to config file.
     */
    void saveCurrentConfigToFile();

    bool calibrationValid();

  private:
    Configuration();
    virtual ~Configuration();
    Configuration(const Configuration &) = delete;
    Configuration &operator=(const Configuration &) = delete;
    std::string configFilePath;
    std::vector<WorkpieceType> order;
    bool isMaster{true};
    bool hasPusher{false};
    Calibration cal;
    void writeLineToConfigFile(int lineNumber, const std::string &newContent);
};
