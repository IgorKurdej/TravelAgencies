#pragma once

#include "libs.h"

class DbConnection
{
private:
	static DbConnection* instance;
	DbConnection();
	const std::string DATABASE_NAME = "test.db";
	sqlite3* connection = nullptr;

	void create_tables() const;
public:
	static DbConnection* get_instance();
	sqlite3* get_connection() const;

	~DbConnection();
};