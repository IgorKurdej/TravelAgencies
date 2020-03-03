#include "travel_agency_utils.h"
#include "trip_repository.h"
#include "offer.h"
#include "menu_service.h"

int main()
{
	try {
		const std::string JSON_FILENAME = "travel_agents.json";
		const auto trip_repository = new TripRepository();

		const MenuService menu_service{ JSON_FILENAME, std::make_shared<TripRepository>(*trip_repository) };
		menu_service.main_menu();

		delete trip_repository;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cin.get();
	return 0;
}