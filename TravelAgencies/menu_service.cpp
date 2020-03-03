#include "menu_service.h"


MenuService::MenuService(const std::string& json_filename, const std::shared_ptr<TripRepository>& trip_repository)
{
	o = std::make_shared<Offer>(json_filename, trip_repository);
}


void MenuService::option1() const
{
	std::cout << "---------- ALL TRAVEL AGENCIES ----------\n";

	const auto travel_agencies = o->get_all_agencies();
	for (const auto& ta : travel_agencies)
	{
		std::cout << *ta << std::endl;
	}
}

void MenuService::option2() const
{
	std::cout << "---------- ALL TRIPS ----------\n";

	const auto trips = o->get_all_trips();
	for (const auto& t : trips)
	{
		std::cout << *t << std::endl;
	}
}

void MenuService::option3() const
{
	std::cout << "---------- TRAVEL AGNECIES WITH TRIPS ----------\n";

	auto travel_agencies_with_trips = o->fill_map();
	for (const auto& [ta, t] : travel_agencies_with_trips)
	{
		std::cout << "\n============ Travel agency ============\n";
		std::cout << *ta << std::endl;
		std::cout << "\n============ " << ta->name << " Trips ============\n";
		for (const auto& trips : t)
		{
			std::cout << *trips << std::endl;
		}
	}
}

void MenuService::option4() const
{

	std::cout << "---------- STATISTICS	----------\n";

	const auto the_most_trips = o->the_most_trips();
	std::cout << "------------------------" << std::endl;
	const auto the_most_money_make = o->the_most_money_make();
	std::cout << "------------------------" << std::endl;
	const auto the_most_common_travel_destination = o->the_most_common_travel_destination();
	std::cout << "------------------------" << std::endl;
	std::cout << "Avg cost of trip and the nearest trip to this\n";
	const auto avg_cost_of_trips = o->avg_cost_of_trips();
	std::cout << "------------------------" << std::endl;
	std::cout << "Group countries with best travel agencies\n";
	const auto group_countries_with_best_travel_agencies = o->group_countries_with_best_travel_agencies();
	std::cout << "------------------------" << std::endl;
	std::cout << "Trips only to Europe\n";
	const auto trips_only_to_europe = o->trips_only_to_europe();
	std::cout << "------------------------" << std::endl;
	std::cout << "Trips sort by participants\n";
	const auto trips_sort_by_participants = o->trips_sort_by_participants();

}

int MenuService::print_menu()
{
	int choice;

	std::cout << "------------------------ MAIN_MENU -----------------------\n";
	std::cout << "1. Travel agencies" << std::endl;
	std::cout << "2. Trips" << std::endl;
	std::cout << "3. Travel agencies with trips" << std::endl;
	std::cout << "4. Statistics" << std::endl;
	std::cout << "5. Exit" << std::endl;

	std::cout << "Your choice: ";
	std::cin >> choice; std::cin.get();

	return choice;
}

void MenuService::main_menu() const
{
	do
	{
		const auto choice = print_menu();
		switch (choice)
		{
		case 1:
			option1();
			break;
		case 2:
			option2();
			break;
		case 3:
			option3();
			break;
		case 4:
			option4();
			break;
		case 5:
			std::cout << "Have a nice day!" << std::endl;
			return;
		default:
			std::cout << "There is no such an option!" << std::endl;
		}
	} while (true);
}
