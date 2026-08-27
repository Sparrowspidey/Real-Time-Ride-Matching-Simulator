#pragma once

enum class DriverStatus {
    IDLE,
    BUSY
};

enum class RiderStatus {
    IDLE,
    PENDING,
    MATCHED,
    IN_RIDE,
    COMPLETED
};