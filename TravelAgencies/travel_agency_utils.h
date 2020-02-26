#pragma once

#include "travel_agency.h"

class TravelAgencyUtils
{
public:
	static std::vector<std::unique_ptr<TravelAgency>> from_json(const std::string filename);
	static std::string to_json(const std::vector<std::unique_ptr<TravelAgency>>& travel_agents, const std::string& filename);

	TravelAgencyUtils() = default;
	~TravelAgencyUtils() = default;
};

