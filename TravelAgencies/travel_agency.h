#pragma once

#include "libs.h"

struct TravelAgency
{
	int id_agency;
	std::string name;
	std::string agency_localization;

	friend std::ostream& operator<<(std::ostream& out, const TravelAgency& ta)
	{
		return out << "ID agency: " << ta.id_agency << " | Name: " << ta.name << " | Localization: " << ta.agency_localization;
	}
};

struct TravelAgencyHasher
{
	size_t operator()(const std::unique_ptr<TravelAgency>& t) const
	{
		return std::hash<int>()(t->id_agency) + 31 * std::hash<std::string>()(t->name) + 31 * std::hash<std::string>()(t->agency_localization);
	}
};

struct TravelAgencyComparator
{
	bool operator()(const std::unique_ptr<TravelAgency>& t1, const std::unique_ptr<TravelAgency>& t2) const
	{
		return t1->id_agency == t2->id_agency && t1->name == t2->name && t1->agency_localization == t2->agency_localization;
	}
};