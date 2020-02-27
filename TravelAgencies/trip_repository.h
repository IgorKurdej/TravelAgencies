#pragma once

#include "db_connection.h"
#include "trip.h"

class TripRepository
{
private:
	sqlite3* connection;
	static std::string from_unsigned_char_to_string(const unsigned char* value);
public:
	TripRepository();

	void insert_trip(const Trip& trip) const;
	std::unordered_set<std::unique_ptr<Trip>, TripHasher, TripComparator> find_all_trips() const;
	std::unordered_set<std::unique_ptr<Trip>, TripHasher, TripComparator> find_trips_by_travel_agency_id(int id) const;

	~TripRepository();
};
