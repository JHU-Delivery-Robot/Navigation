#include "TG30_lidar.hpp"

#include <chrono>
#include <iomanip>
#include <vector>

namespace hal {

namespace impl {

TG30Lidar::TG30Lidar() : serial(io_context), timeout(io_context), is_scanning(false) {}

bool TG30Lidar::initialize(std::string port) {
    asio::error_code err;
    serial.open(port, err);
    if (err) {
        std::cout << err.message() << std::endl;
        return false;
    }

    // Configure serial port baud rate
    serial.set_option(asio::serial_port_base::baud_rate(baud_rate), err);
    if (err) {
        std::cout << err.message() << std::endl;
        return false;
    }

    // Configure serial port to use eight data bits per byte
    serial.set_option(asio::serial_port_base::character_size(8), err);
    if (err) {
        std::cout << err.message() << std::endl;
        return false;
    }

    // Configure serial port to use one stop bit
    serial.set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one), err);
    if (err) {
        std::cout << err.message() << std::endl;
        return false;
    }

    // Configure serial port to have no parity
    serial.set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none), err);
    if (err) {
        std::cout << err.message() << std::endl;
        return false;
    }

    // Configure serial port to use no flow control
    serial.set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));
    if (err) {
        std::cout << err.message() << std::endl;
        return false;
    }

    // When TG30 lidar is in scan mode, it ignores all commands except stop command,
    // so we issue a stop scanning command just in case lidar is already scanning.
    return sendCommand(stop_scanning) && flushSerialBuffer();
}

void TG30Lidar::executeIO() {
    auto future = std::async(std::launch::async, [&]() {
        if (io_context.stopped()) {
            io_context.reset();
        }

        io_context.run();
    });

    future.wait();
}

bool TG30Lidar::flushSerialBuffer() {
    asio::error_code err;

    constexpr size_t buffer_size = 256;
    constexpr size_t timeout_ms = 20;

    uint8_t discard[buffer_size];
    bool success = true;

    std::function<void()> read_buffer;

    auto timeout_handler = [&](const asio::error_code& err) {
        if (err == asio::error::operation_aborted) {
            return;
        } else if (err) {
            std::cout << err.message() << std::endl;
            success = false;
        }

        serial.cancel();
    };

    auto read_handler = [&](const asio::error_code& err, std::size_t bytes_read) {
        if (err == asio::error::operation_aborted) {
            return;
        } else if (err) {
            std::cout << err.message() << std::endl;
            success = false;
        }

        timeout.cancel();

        if (bytes_read == buffer_size) {
            read_buffer();
        }
    };

    read_buffer = [&]() {
        timeout.expires_after(std::chrono::milliseconds(timeout_ms));
        timeout.async_wait(timeout_handler);
        asio::async_read(serial, asio::buffer(discard), asio::transfer_at_least(1), read_handler);
    };

    read_buffer();
    executeIO();

    return success;
}

bool TG30Lidar::checkResponseHeader(std::vector<uint8_t> response, RequestSpec request) {
    if (response.size() < header_size) {
        return false;
    }

    // bytes 0 and 1 are sync bytes, must be 0xA55A
    bool valid_sync_bytes = response[0] == 0xA5 && response[1] == 0x5A;
    if (!valid_sync_bytes) {
        std::cout << "Invalid sync bytes: 0x" << std::hex << (int)response[0] << (int)response[1] << std::dec << std::endl;
        return false;
    }

    // bytes 2 through 5 are message length
    uint32_t message_length = (response[5] << 8 * 3) +
                              (response[4] << 8 * 2) +
                              (response[3] << 8 * 1) +
                              (response[2] << 8 * 0);

    // upper 2 bits of byte 5 indicate whether response mode is continuous or single
    bool continuous_mode = (response[5] >> 6) == 1U;

    bool valid_response_length = continuous_mode == request.is_continuous;
    if (!request.is_continuous) {
        valid_response_length &= message_length == request.response_length;
    }

    // content type depends on the exact response format for each command
    uint8_t content_type = response[6];
    return valid_response_length && content_type == request.content_type;
}

bool TG30Lidar::sendCommand(RequestSpec request) {
    asio::error_code err;
    uint8_t command_message[2] = {tg30_sync_byte, request.request_byte};
    asio::write(serial, asio::buffer(command_message), err);
    if (err) {
        std::cout << err.message() << std::endl;
        return false;
    }

    return true;
}

std::optional<std::vector<uint8_t>> TG30Lidar::sendRequest(RequestSpec request) {
    asio::error_code err;
    uint8_t request_message[2] = {tg30_sync_byte, request.request_byte};
    asio::write(serial, asio::buffer(request_message), err);
    if (err) {
        std::cout << err.message() << std::endl;
        return {};
    }

    std::vector<uint8_t> response_data;
    size_t response_size = header_size + request.response_length;
    response_data.resize(response_size);

    bool success = true;

    auto timeout_handler = [&](const asio::error_code& err) {
        if (err == asio::error::operation_aborted) {
            return;
        } else if (err) {
            std::cout << err.message() << std::endl;
            success = false;
        }

        serial.cancel();
    };

    auto read_handler = [&](const asio::error_code& err, std::size_t) {
        if (err == asio::error::operation_aborted) {
            return;
        } else if (err) {
            std::cout << err.message() << std::endl;
            success = false;
        }

        timeout.cancel();
    };

    timeout.expires_after(std::chrono::milliseconds(read_timeout_ms));
    timeout.async_wait(timeout_handler);
    asio::async_read(serial, asio::buffer(response_data), asio::transfer_exactly(response_size), read_handler);

    executeIO();

    if (!success) {
        return {};
    }

    bool header_ok = checkResponseHeader(response_data, request);
    if (!header_ok) {
        return {};
    }

    std::vector<uint8_t> response_content(response_data.begin() + header_size, response_data.end());
    return response_content;
}

void TG30Lidar::syncPacketHeader(int max_sync_attempts, std::function<void(bool)> callback) {
    if (max_sync_attempts == 0) {
        callback(false);
        return;
    }

    auto timeout_handler = [&, callback](const asio::error_code& err) {
        if (err == asio::error::operation_aborted) {
            return;
        } else if (err) {
            std::cout << err.message() << std::endl;
            callback(false);
            return;
        }

        serial.cancel();
        std::cout << "Sync read timeout!" << std::endl;
    };

    // advance position
    packet_sync_bytes[1] = packet_sync_bytes[0];

    auto read_handler = [&, max_sync_attempts, callback](const asio::error_code& err, std::size_t) {
        if (err == asio::error::operation_aborted) {
            return;
        } else if (err) {
            callback(false);
            return;
        }

        // sync bytes must be 0x55AA. Sent in LSB MSB order, but we read backwards
        bool valid_sync_bytes = packet_sync_bytes[0] == 0x55 && packet_sync_bytes[1] == 0xAA;
        if (valid_sync_bytes) {
            callback(true);
            return;
        }

        syncPacketHeader(max_sync_attempts - 1, callback);
    };

    timeout.expires_after(std::chrono::milliseconds(read_timeout_ms));
    timeout.async_wait(timeout_handler);
    asio::async_read(serial, asio::buffer(packet_sync_bytes), asio::transfer_exactly(1), read_handler);
}

void TG30Lidar::scanHeader(std::function<void(std::optional<PacketHeader>)> callback) {
    auto timeout_handler = [&, callback](const asio::error_code& err) {
        if (err == asio::error::operation_aborted) {
            return;
        } else if (err) {
            std::cout << err.message() << std::endl;
            callback({});
            return;
        }

        std::cout << "Scan header read timeout" << std::endl;
        serial.cancel();
    };

    auto read_handler = [&, callback](const asio::error_code& err, std::size_t) {
        if (err == asio::error::operation_aborted) {
            return;
        } else if (err) {
            callback({});
            return;
        }

        timeout.cancel();

        PacketHeader header;
        header.is_start_packet = packet_header_bytes[0] & 1;
        header.number_of_samples = packet_header_bytes[1];

        if (header.is_start_packet) {
            header.scan_frequency = ((packet_header_bytes[0] >> 1) + 30.0) / 10.0;
        }

        uint16_t FSA = (packet_header_bytes[3] << 8) + packet_header_bytes[2];
        uint16_t LSA = (packet_header_bytes[5] << 8) + packet_header_bytes[4];

        header.start_angle = static_cast<double>(FSA >> 1) / 64.0;
        header.end_angle = static_cast<double>(LSA >> 1) / 64.0;

        callback(header);
    };

    auto synced_callback = [&, timeout_handler, read_handler](bool synced) {
        if (!synced) {
            std::cout << "Failed to sync packet header within 10 attempts" << std::endl;
            callback({});
            return;
        }

        timeout.expires_after(std::chrono::milliseconds(read_timeout_ms));
        timeout.async_wait(timeout_handler);
        asio::async_read(serial, asio::buffer(packet_header_bytes), asio::transfer_exactly(packet_header_size), read_handler);
    };

    syncPacketHeader(max_packet_sync_attempts, synced_callback);
}

void TG30Lidar::parseScanPacket(const PacketHeader& header) {
    double end_angle = header.end_angle >= header.start_angle
                           ? header.end_angle
                           : header.end_angle + 360;

    double delta_angle = (header.number_of_samples > 1)
                             ? (end_angle - header.start_angle) / (header.number_of_samples - 1)
                             : 0.0;

    for (int i = 0; i < header.number_of_samples; i++) {
        uint16_t distance = (scan_packet_data[2 * i + 1] << 8) + scan_packet_data[2 * i];
        double angle = header.start_angle + i * delta_angle;
        if (angle >= 360) {
            angle -= 360;
        }

        if (distance == 0) {
            continue;
        }

        in_progress_scan->push_back(SamplePoint(distance, angle));
    }
}

void TG30Lidar::readScanPacket() {
    scanHeader([&](std::optional<PacketHeader> header) {
        if (!header.has_value()) {
            std::cout << "Invalid packet header" << std::endl;
            return;
        }

        if (header->is_start_packet) {
            std::cout << "Scan freq: " << header->scan_frequency << " Hz" << std::endl;

            std::scoped_lock lock(scan_data_mutex);
            completed_scan = in_progress_scan;
            in_progress_scan = std::make_shared<std::vector<SamplePoint>>();
        }

        // resize doesn't change capacity, only size, so once the largest packet size
        // has been allocated (seems to be 80 bytes), we never need to re-allocate
        int sample_bytes_to_read = 2 * header->number_of_samples;
        scan_packet_data.resize(sample_bytes_to_read);

        auto timeout_handler = [&](const asio::error_code& err) {
            if (err == asio::error::operation_aborted) {
                return;
            } else {
                std::cerr << err.message() << std::endl;
                return;
            }

            serial.cancel();
        };

        auto read_handler = [&, header, sample_bytes_to_read](const asio::error_code& err, size_t) {
            if (err == asio::error::operation_aborted) {
                return;
            } else if (err) {
                std::cout << err.message() << std::endl;
                return;
            }

            timeout.cancel();
            parseScanPacket(header.value());

            if (is_scanning) {
                // wait for and read next packet
                readScanPacket();
            }
        };

        timeout.expires_after(std::chrono::milliseconds(read_timeout_ms));
        timeout.async_wait(timeout_handler);
        asio::async_read(serial, asio::buffer(scan_packet_data), asio::transfer_exactly(sample_bytes_to_read), read_handler);
    });
}

bool TG30Lidar::beginScanning() {
    std::optional<std::vector<uint8_t>> response = sendRequest(start_scanning);
    if (!response.has_value()) {
        return false;
    }

    is_scanning = true;

    {
        std::scoped_lock lock(scan_data_mutex);
        completed_scan = nullptr;
        in_progress_scan = std::make_shared<std::vector<SamplePoint>>();
    }

    readScanPacket();

    // Pump IO loop in new thread
    scanning_executor = std::async(std::launch::async, [&]() {
        if (io_context.stopped()) {
            io_context.reset();
        }

        io_context.run();
    });

    return true;
}

bool TG30Lidar::stopScanning() {
    is_scanning = false;

    serial.cancel();
    bool success = sendCommand(stop_scanning) && flushSerialBuffer();

    return success;
}

std::shared_ptr<std::vector<TG30Lidar::SamplePoint>> TG30Lidar::getLatestScan() {
    std::scoped_lock lock(scan_data_mutex);
    return completed_scan;
}

std::optional<double> TG30Lidar::sendFrequencyCommand(RequestSpec request) {
    std::optional<std::vector<uint8_t>> content = sendRequest(request);
    if (!content) {
        return {};
    }

    std::vector<uint8_t> data = content.value();
    uint32_t raw = data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
    double frequency = static_cast<double>(raw) / 100.0;

    return frequency;
}

std::optional<double> TG30Lidar::getScanFrequency() {
    return sendFrequencyCommand(query_scan_frequency);
}

bool TG30Lidar::setScanFrequency(double scan_frequency) {
    if (scan_frequency < 5.0 || scan_frequency > 12.0) {
        return false;
    }

    scan_frequency = static_cast<double>(static_cast<int>(scan_frequency * 10)) / 10;

    std::optional<double> current_frequency = sendFrequencyCommand(query_scan_frequency);

    while (current_frequency.has_value() && abs(current_frequency.value() - scan_frequency) > 0.05) {
        double delta = scan_frequency - current_frequency.value();
        if (delta >= 1.0) {
            current_frequency = sendFrequencyCommand(increase_scan_frequency_one);
        } else if (delta <= -1.0) {
            current_frequency = sendFrequencyCommand(decrease_scan_frequency_one);
        } else if (delta > 0.0) {
            current_frequency = sendFrequencyCommand(increase_scan_frequency_tenth);
        } else if (delta < 0.0) {
            current_frequency = sendFrequencyCommand(decrease_scan_frequency_tenth);
        }
    }

    return current_frequency.has_value();
}

std::optional<TG30Lidar::DeviceInfo> TG30Lidar::getDeviceInfo() {
    std::optional<std::vector<uint8_t>> content = sendRequest(device_info);
    if (!content) {
        return {};
    }

    std::vector<uint8_t> content_bytes = content.value();

    TG30Lidar::DeviceInfo device_info;

    device_info.model_number = content_bytes[0];
    device_info.firmware_major_version = content_bytes[1];
    device_info.firmware_minor_version = content_bytes[2];
    device_info.hardware_version = content_bytes[3];

    std::stringstream serial_number_ss;
    serial_number_ss << std::hex << std::setw(2) << std::setfill('0');

    for (int i = 4; i < 4 + 16; i++) {
        serial_number_ss << (int)content_bytes[i];
    }

    device_info.serial_number = serial_number_ss.str();

    return device_info;
}

std::optional<TG30Lidar::DeviceHealth> TG30Lidar::getDeviceHealth() {
    std::optional<std::vector<uint8_t>> content = sendRequest(device_health);
    if (!content) {
        return {};
    }

    std::vector<uint8_t> content_bytes = content.value();

    TG30Lidar::DeviceHealth device_health;
    device_health.error_code = (content_bytes[2] << 8) + content_bytes[1];

    uint8_t status_code = content_bytes[0];
    switch (status_code) {
        case 0:
            device_health.status = "Nominal";
            device_health.ok = true;
            break;
        case 1:
            device_health.status = "Warning";
            device_health.ok = false;
            break;
        case 2:
        default:
            device_health.status = "Error";
            device_health.ok = false;
            break;
    }

    return device_health;
}

}  // namespace impl

}  // namespace hal
