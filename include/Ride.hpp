#pragma once

enum class RideStatus {
    REQUESTED,
    MATCHED,
    COMPLETED,
    CANCELLED
};

class Ride {
private:
    int id;
    int riderId;
    int pickupCityId;
    int destinationCityId;
    int requestTime;
    RideStatus status;

public:
    Ride(int id, int riderId, int pickupCityId, int destinationCityId, int requestTime);

    int getId() const;
    int getRiderId() const;
    int getPickupCityId() const;
    int getDestinationCityId() const;
    int getRequestTime() const;

    RideStatus getStatus() const;
    void setStatus(RideStatus status);
};