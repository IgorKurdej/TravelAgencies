#include "db_connection.h"

DbConnection::DbConnection()
{
	const auto result = sqlite3_open(DATABASE_NAME.c_str(), &connection);

	if (SQLITE_OK != result)
	{
		throw std::exception(sqlite3_errmsg(connection));
	}
	std::cout << "Database connection has been established!" << std::endl;
	create_tables();
}

DbConnection* DbConnection::get_instance()
{
	return instance;
}

sqlite3* DbConnection::get_connection() const
{
	return connection;
}

void DbConnection::create_tables() const
{
	const std::string sql_trip = "create table if not exists trips (id_trip integer primary key autoincrement, destination varchar(50) not null, price integer default 0, how_many_people integer default 0, id_travel_agency integer default 0);";

	char** error_massage = nullptr;
	const auto result = sqlite3_exec(connection, sql_trip.c_str(), nullptr, nullptr, error_massage);

	if (SQLITE_OK != result)
	{
		std::cout << sqlite3_errmsg(connection) << std::endl;
		sqlite3_free(error_massage);
		throw std::exception("Trips table creation exception");
	}
	std::cout << "Trips table has been created!" << std::endl;
}

DbConnection::~DbConnection()
{
	sqlite3_close(connection);
}

DbConnection* DbConnection::instance = new DbConnection();