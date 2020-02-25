#pragma once

#include "libs.h"

struct Trip
{
	int id_trip;
	std::string destination;
	int price;
	int how_many_people;
	int id_travel_agency;

	Trip() = default;
	Trip(const int id_trip, const std::string& destination, const int price, const int how_many_people, const int id_travel_agency)
		: id_trip{ id_trip }, destination{ destination }, price{ price }, how_many_people{ how_many_people }, id_travel_agency{ id_travel_agency } {}

	Trip(const std::string& destination, const int price, const int how_many_people, const int id_travel_agency)
		: destination{ destination }, price{ price }, how_many_people{ how_many_people }, id_travel_agency{ id_travel_agency } {}

	friend std::ostream& operator<<(std::ostream& out, const Trip& t)
	{
		return out << "ID trip: " << t.id_trip << " | Destination: " << t.destination << " | Price: " << t.price << " | People: " << t.how_many_people;
	}
};

struct TripHasher
{
	size_t operator()(const std::unique_ptr<Trip>& t) const
	{
		return std::hash<int>()(t->id_trip) +
			31 * std::hash<std::string>()(t->destination) +
			31 * std::hash<int>()(t->how_many_people) +
			31 * std::hash<int>()(t->id_travel_agency) +
			31 * std::hash<int>()(t->price);
	}
};

struct TripComparator
{
	bool operator()(const std::unique_ptr<Trip>& t1, const std::unique_ptr<Trip>& t2) const
	{
		return t1->id_trip == t2->id_trip &&
			t1->destination == t2->destination &&
			t1->how_many_people == t2->how_many_people &&
			t1->id_travel_agency == t2->id_travel_agency &&
			t1->price == t2->price;
	}
};