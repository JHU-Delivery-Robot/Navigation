#ifndef TG30Lidar_HPP
#define TG30Lidar_HPP

#include <cstdint>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>

#include "asio.hpp"
#include "error_reporting.hpp"
#include "lidar_scanner.hpp"

namespace hal {

namespace impl {

// TG30 lidar request message spec
class RequestSpec {
public:
    constexpr RequestSpec(uint8_t request_byte, uint32_t response_length, bool is_continuous, uint8_t content_type)
        : request_byte(request_byte),
          response_length(response_length),
          is_continuous(is_continuous),
          content_type(content_type) {}

    uint8_t request_byte;
    uint32_t response_length;
    bool is_continuous;
    uint8_t content_type;
};

/**
 * Interface for communication with YdLidar TG30 lidar unit.
 */
class TG30Lidar final : public hal::LidarScanner {
public:
    class DeviceInfo {
    public:
        uint8_t model_number;
        uint8_t firmware_major_version;
        uint8_t firmware_minor_version;
        uint8_t hardware_version;
        std::string serial_number;
    };

    class DeviceHealth {
    public:
        bool ok;
        std::string status;
        uint16_t error_code;
    };

    /**
     * Construct lidar interface
     * 
     * @param error_reporting Error event instance
     */
    TG30Lidar(events::ErrorReporting error_reporting);

    /**
     * Initialize serial port communication with lidar unit
     * 
     * @param port The name/path of the serial port to use
     * @return true if initialization was successful, false otherwise
     */
    bool initialize(std::string port);

    /**
     * Begin continuously scanning until stopScanning is called.
     * 
     * @return true is successful, false on error
     */
    bool beginScanning();

    /**
     * If currently scanning, stop.
     * 
     * @return true is successful, false on error
     */
    bool stopScanning();

    /**
     * Retrives pointer with shared ownership to latest lidar scan. This data
     *  held on to as long as desired, and can be released by simply allowing
     *  the pointer to go out of scope.
     * 
     * @return pointer to latest lidar scan data
     */
    std::shared_ptr<std::vector<SamplePoint>> getLatestScan();

    /**
     * Get scan frequency of lidar. Value will in range 5.0-12.0 Hz,
     *  with precision of 0.1 Hz.
     * 
     * @param scanFrequency full scans per second
     * @return scan frequency of lidar unit if successful, empty
     *     optional on failure
     */
    std::optional<double> getScanFrequency();

    /**
     * Set scan frequency of lidar, in range 5.0-12.0 Hz. Supports
     *  precision of 0.1 Hz, specified scan frequency must within
     *  supported range (inclusive) and will be rounded to nearest
     *  tenth of a hertz.
     * 
     * @param scanFrequency full scans per second
     * @return true is successful, false on error
     */
    bool setScanFrequency(double scanFrequency);

    /**
     * Query device information
     * 
     * @return lidar device info if successful, empty optional on failure
     */
    std::optional<DeviceInfo> getDeviceInfo();

    /**
     * Query device health
     * 
     * @return lidar device health if successful, empty optional on failure
     */
    std::optional<DeviceHealth> getDeviceHealth();

private:
    // Scanning response packet header
    class PacketHeader {
    public:
        bool is_start_packet;
        double scan_frequency;
        uint8_t number_of_samples;
        double start_angle;
        double end_angle;
    };

    // Serial port baud rate - only seems to work with 512 kBd
    static constexpr unsigned int baud_rate = 512000U;

    // Serial port read timeout in milliseconds
    static constexpr unsigned int read_timeout_ms = 500;

    // TG30 message header length in bytes
    static constexpr size_t header_size = 7;

    // Details of commands/requests for TG30 lidar unit
    static constexpr uint8_t tg30_sync_byte = 0xA5;
    static constexpr RequestSpec start_scanning = RequestSpec(0x60, 0, true, 0x81);
    static constexpr RequestSpec stop_scanning = RequestSpec(0x65, 0, false, 0);
    static constexpr RequestSpec device_info = RequestSpec(0x90, 20, false, 0x04);
    static constexpr RequestSpec device_health = RequestSpec(0x91, 3, false, 0x06);
    static constexpr RequestSpec query_scan_frequency = RequestSpec(0x0D, 4, false, 0x04);
    static constexpr RequestSpec increase_scan_frequency_one = RequestSpec(0x0B, 4, false, 0x04);
    static constexpr RequestSpec decrease_scan_frequency_one = RequestSpec(0x0C, 4, false, 0x04);
    static constexpr RequestSpec increase_scan_frequency_tenth = RequestSpec(0x09, 4, false, 0x04);
    static constexpr RequestSpec decrease_scan_frequency_tenth = RequestSpec(0x0A, 4, false, 0x04);

    asio::io_context io_context;
    asio::serial_port serial;
    asio::steady_timer timeout;

    // True when lidar is in scanning mode and asynchronous thread is
    // processing scanning data. Setting to false will stop the data
    // processing thread but won't stop the lidar unit from scanning.
    bool is_scanning;

    // Storage for data read from lidar scan data packets.
    // Used as backing storage for read buffers that won't
    // leave scope between asynchronous calls.
    static constexpr int max_packet_sync_attempts = 10;
    uint8_t packet_sync_bytes[2];
    static constexpr size_t packet_header_size = 8;
    uint8_t packet_header_bytes[packet_header_size];
    std::vector<uint8_t> scan_packet_data;

    // Completed scan should only be accessed with scan_data_mutex
    // is locked, since it can be accessed from data processing thread
    // and via client
    std::mutex scan_data_mutex;
    std::shared_ptr<std::vector<SamplePoint>> completed_scan;
    std::shared_ptr<std::vector<SamplePoint>> in_progress_scan;

    // Blocking call to run ASIO IO processing until completion in separate thread
    // Useful to execute asynchronous requests and wait for results
    void executeIO();

    // Handle to asynchronous processing thread, only needed to prevent
    // the future from going out of scope and being destructed
    std::future<void> scanning_executor;

    events::ErrorReporting error_reporting;
    bool flushSerialBuffer();
    bool checkResponseHeader(std::vector<uint8_t> response, RequestSpec request);
    bool sendCommand(RequestSpec command);
    std::optional<std::vector<uint8_t>> sendRequest(RequestSpec request);
    std::optional<double> sendFrequencyCommand(RequestSpec request);
    void syncPacketHeader(int max_sync_attempts, std::function<void(bool)> callback);
    void scanHeader(std::function<void(std::optional<PacketHeader>)> callback);
    void parseScanPacket(const PacketHeader& header);
    void readScanPacket();
};

}  // namespace impl

}  // namespace hal

#endif
