#include "travel_agency_utils.h"

std::vector<std::unique_ptr<TravelAgency>> TravelAgencyUtils::from_json(const std::string filename)
{
	std::ifstream reader(filename);
	json j;
	reader >> j;

	std::vector<std::unique_ptr<TravelAgency>> travel_agents;

	for (const auto& travel_agency : j)
	{
		travel_agents.emplace_back(std::make_unique<TravelAgency>(TravelAgency{
			travel_agency["id_agency"].get<int>(),
			travel_agency["name"].get<std::string>(),
			travel_agency["agency_localization"].get<std::string>()
			}));
	}

	return travel_agents;
}

std::string TravelAgencyUtils::to_json(const std::vector<std::unique_ptr<TravelAgency>>& travel_agents, const std::string& filename)
{
	std::ofstream writer(filename);
	json j_object, j_array;

	for (const auto& travel_agency : travel_agents)
	{
		j_object["id_agency"] = travel_agency->id_agency;
		j_object["name"] = travel_agency->name;
		j_object["agency_localization"] = travel_agency->agency_localization;
		j_array.emplace_back(j_object);
		j_object.clear();
	}

	auto res = j_array.dump(4);
	writer << res;
	writer.close();
	return res;
}
