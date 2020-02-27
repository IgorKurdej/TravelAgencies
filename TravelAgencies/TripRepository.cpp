#include "trip_repository.h"


std::string TripRepository::from_unsigned_char_to_string(const unsigned char* value)
{
	std::string result;

	for (int i = 0; i < strlen(reinterpret_cast<const char*>(value)); ++i)
	{
		result += value[i];
	}
	return result;
}

TripRepository::TripRepository()
{
	connection = DbConnection::get_instance()->get_connection();
}

void TripRepository::insert_trip(const Trip& trip) const
{
	const std::string sql = "insert into trips (destination, price, how_many_people, id_travel_agency) values (?, ?, ?, ?);";
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(connection, sql.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_text(stmt, 1, trip.destination.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, trip.price);
	sqlite3_bind_int(stmt, 3, trip.how_many_people);
	sqlite3_bind_int(stmt, 4, trip.id_travel_agency);

	const auto result = sqlite3_step(stmt);

	if (result != SQLITE_DONE)
	{
		sqlite3_errmsg(connection);
		throw std::exception(sqlite3_errmsg(connection));
	}
	sqlite3_finalize(stmt);
	std::cout << "Trip has been inserted!" << std::endl;
}


std::unordered_set<std::unique_ptr<Trip>, TripHasher, TripComparator> TripRepository::find_all_trips() const
{
	std::unordered_set<std::unique_ptr<Trip>, TripHasher, TripComparator> all_trips;

	const std::string sql = "select id_trip, destination, price, how_many_people, id_travel_agency from trips";
	const auto connection = DbConnection::get_instance()->get_connection();
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection, sql.c_str(), -1, &stmt, nullptr);

	auto result = 0;

	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		all_trips.insert(std::make_unique<Trip>(
			sqlite3_column_int(stmt, 0),
			from_unsigned_char_to_string(sqlite3_column_text(stmt, 1)),
			sqlite3_column_int(stmt, 2),
			sqlite3_column_int(stmt, 3),
			sqlite3_column_int(stmt, 4)
			));
	}

	if (result != SQLITE_DONE)
	{
		sqlite3_errmsg(connection);
		throw std::exception(sqlite3_errmsg(connection));
	}

	sqlite3_finalize(stmt);
	return all_trips;
}

std::unordered_set<std::unique_ptr<Trip>, TripHasher, TripComparator> TripRepository::find_trips_by_travel_agency_id(int id) const
{
	std::unordered_set<std::unique_ptr<Trip>, TripHasher, TripComparator> trips;


	const std::string sql = "select id_trip, destination, price, how_many_people, id_travel_agency from trips where id_travel_agency = ?1";
	const auto connection = DbConnection::get_instance()->get_connection();
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection, sql.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id);

	auto result = 0;
	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		trips.insert(std::make_unique<Trip>(Trip{
			sqlite3_column_int(stmt, 0),
			from_unsigned_char_to_string(sqlite3_column_text(stmt, 1)),
			sqlite3_column_int(stmt, 2),
			sqlite3_column_int(stmt, 3),
			sqlite3_column_int(stmt, 4)
			}));
	}

	if (result != SQLITE_DONE)
	{
		sqlite3_errmsg(connection);
		throw std::exception(sqlite3_errmsg(connection));
	}

	sqlite3_finalize(stmt);
	return trips;
}

TripRepository::~TripRepository()
{
	sqlite3_close(connection);
}
