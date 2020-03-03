#include "offer.h"

Offer::Offer(const std::string& json_name, const std::shared_ptr<TripRepository>& trip_repository)
{
	this->trip_repository = trip_repository;
	this->json_filename = json_name;
	travel_agency_with_trips = fill_map();
}

offer_map Offer::fill_map() const
{
	offer_map m;
	const auto travel_agencies = TravelAgencyUtils::from_json(json_filename);

	for (const auto& ta : travel_agencies)
	{
		m.insert(std::make_pair(std::make_unique<TravelAgency>(*ta), trip_repository->find_trips_by_travel_agency_id(ta->id_agency)));
	}
	return m;
}

std::string Offer::the_most_trips() const
{
	auto max_count = 0;
	std::string s;

	for (const auto& [travel_agency, trips] : travel_agency_with_trips)
	{
		auto count = 0;

		for (const auto& t : trips)
		{
			count++;
		}

		if (count >= max_count)
		{
			max_count = count;
			s = travel_agency->name;
		}
	}

	std::cout << "Travel agency with the most trips: " << s << std::endl;

	return s;
}

std::string Offer::the_most_money_make() const
{
	auto most_money = 0;
	std::string s;

	for (const auto& pair : travel_agency_with_trips)
	{
		auto count = 0;

		for (const auto& t : pair.second)
		{
			count += (t->price * 0.1 * 0.8);
		}

		if (count >= most_money)
		{
			most_money = count;
			s = pair.first->name;
		}
	}
	std::cout << "The most money make: " << s << " - they earn: " << most_money << std::endl;

	return s;
}

std::string Offer::the_most_common_travel_destination() const
{
	std::vector<std::string> destination;
	auto word_count = 0;

	for (const auto& pair : travel_agency_with_trips)
	{
		for (const auto& t : pair.second)
		{
			destination.emplace_back(t->destination);
		}
	}

	std::sort(destination.begin(), destination.end());
	const int v_size = destination.size();
	auto word = destination[0];

	std::map<std::string, int> map;

	for (auto i = 0; i < v_size; ++i)
	{
		if (word != destination[i])
		{
			map.insert(std::make_pair(word, word_count));
			word_count = 0;
			word = destination[i];
		}
		word_count++;

	}
	map.insert(std::make_pair(word, word_count));

	auto max = 0;
	std::string s;

	for (const auto& m : map)
	{
		if (m.second > max)
		{
			s = m.first;
			max = m.second;
		}
	}

	std::cout << "The most common destination: " << s << " - " << max << std::endl;
	return s;
}

double Offer::count_avg_price(const int id) const
{
	for (const auto& [travel_agencies, trips] : travel_agency_with_trips)
	{
		if (travel_agencies->id_agency == id)
		{
			auto sum_all_money = 0;
			auto counter = 0;
			auto avg_price = 0;

			for (const auto& t : trips)
			{
				counter++;
				sum_all_money += t->price;
			}

			avg_price += sum_all_money / counter;
			return avg_price;
		}
	}
}

Trip Offer::find_trip_price_closest_to_avg(const int id) const
{
	Trip the_closest_trip{};
	auto closest_to_avg_price = 0;

	for (const auto& [travel_agencies, trips] : travel_agency_with_trips)
	{
		if (travel_agencies->id_agency == id)
		{
			const auto avg_price = count_avg_price(travel_agencies->id_agency);
			closest_to_avg_price = avg_price;

			for (const auto& t : trips)
			{
				const auto abs = std::abs(avg_price - t->price);
				if (abs < closest_to_avg_price)
				{
					closest_to_avg_price = abs;
					the_closest_trip = *t;
				}
			}
			return the_closest_trip;
		}
	}
	return the_closest_trip;
}

std::unordered_map<double, std::unique_ptr<Trip>> Offer::avg_cost_of_trips() const
{
	std::unordered_map<double, std::unique_ptr<Trip>> map;

	for (const auto& [travel_agencies, trips] : travel_agency_with_trips)
	{
		const auto id_number = travel_agencies->id_agency;
		const auto x = count_avg_price(id_number);
		auto trip = find_trip_price_closest_to_avg(id_number);
		map.emplace(x, std::make_unique<Trip>(trip));
	}

	for (const auto& pair : map)
	{
		std::cout << "Avg price of all trips in travel agency: " << pair.first << "\nThe clostest trip to avg price: " << *pair.second << std::endl;
	}

	return map;
}

std::unordered_set<std::string> Offer::get_all_countries() const
{
	std::unordered_set<std::string> countries;
	for (const auto& [travel_agency, trips] : travel_agency_with_trips)
	{
		for (const auto& trip : trips)
		{
			countries.emplace(trip->destination);
		}
	}
	return countries;
}

TravelAgency Offer::find_travel_agency_with_max_occurencies_of_country(const std::string& country) const
{
	TravelAgency max_travel_agency{};
	auto max_country_occurencies = 0;
	auto country_occurencies = 0;

	for (const auto& [travel_agency, trips] : travel_agency_with_trips)
	{
		country_occurencies = std::count_if(trips.begin(), trips.end(), [country](const auto & trip_ptr) {return trip_ptr->destination == country; });
		if (country_occurencies > max_country_occurencies)
		{
			max_country_occurencies = country_occurencies;
			max_travel_agency = *travel_agency;
		}
	}

	if (max_country_occurencies == 0)
	{
		throw std::exception(("country " + country + " not found in trips").c_str());
	}

	return max_travel_agency;
}

std::map<std::string, std::unique_ptr<TravelAgency>> Offer::group_countries_with_best_travel_agencies() const
{
	std::map<std::string, std::unique_ptr<TravelAgency>> grouped;

	auto countries = get_all_countries();
	for (const auto& country : countries)
	{
		grouped.emplace(country, std::make_unique<TravelAgency>(find_travel_agency_with_max_occurencies_of_country(country)));
	}

	for (const auto& [travel_agency, trip] : grouped)
	{
		std::cout << travel_agency << ", " << *trip << std::endl;
	}

	return grouped;
}

std::vector<std::string> Offer::read_file(const std::string & file_name) const
{
	std::vector<std::string> european_countries;
	std::string line;

	std::ifstream reader;
	reader.open(file_name);
	if (reader.is_open())
	{
		while (std::getline(reader, line, ','))
		{
			european_countries.emplace_back(line);
		}
		reader.close();
	}

	return european_countries;
}

std::vector<std::unique_ptr<Trip>> Offer::trips_only_to_europe() const
{
	auto european_countries = read_file("european_countries.txt");
	std::vector<std::unique_ptr<Trip>> trips_only_to_europe;

	for (const auto& [travel_agencies, trips] : travel_agency_with_trips)
	{
		for (const auto& t : trips)
		{
			for (const auto& ec : european_countries)
			{
				if (t->destination == ec)
				{
					trips_only_to_europe.emplace_back(std::make_unique<Trip>(*t));
				}
			}
		}
	}

	for (const auto& t : trips_only_to_europe)
	{
		std::cout << *t << std::endl;
	}

	return trips_only_to_europe;
}

std::set<std::unique_ptr<Trip>> Offer::get_all_trips() const
{
	std::set<std::unique_ptr<Trip>> trips;

	for (const auto& [travel_agnecy, trip] : travel_agency_with_trips)
	{
		for (const auto& t : trip)
		{
			trips.insert(std::make_unique<Trip>(*t));
		}
	}

	return trips;
}

std::set<int> Offer::get_all_numbers_of_paricipants() const
{
	std::set<int> numbers_of_paritcipants;
	auto trips = get_all_trips();

	for (const auto& t : trips)
	{
		numbers_of_paritcipants.insert(t->how_many_people);
	}

	return numbers_of_paritcipants;
}

std::set<std::unique_ptr<Trip>> Offer::find_trips_with_same_amount_of_people(const int amount_of_people) const
{
	std::set<std::unique_ptr<Trip>> travel_agencies;

	for (const auto& [travel_agency, trip] : travel_agency_with_trips)
	{
		for (const auto& t : trip)
		{
			if (t->how_many_people == amount_of_people)
			{
				travel_agencies.emplace(std::make_unique<Trip>(*t));
			}
		}
	}

	return travel_agencies;
}

std::map<int, std::set<std::unique_ptr<Trip>>> Offer::trips_sort_by_participants() const
{
	std::map<int, std::set<std::unique_ptr<Trip>>> number_of_participants_with_trips;
	auto trips = get_all_trips();
	auto numbers_of_participants = get_all_numbers_of_paricipants();

	for (const auto& number : numbers_of_participants)
	{
		for (const auto& t : trips)
		{
			if (number == t->how_many_people)
			{
				number_of_participants_with_trips.emplace(number, find_trips_with_same_amount_of_people(number));
			}
		}
	}

	for (const auto& pair : number_of_participants_with_trips)
	{
		std::cout << pair.first << std::endl;
		for (const auto& t : pair.second)
		{
			std::cout << *t << std::endl;
		}
	}

	return number_of_participants_with_trips;
}

std::unique_ptr<Trip> Offer::find_the_most_expensive_trip(std::set<std::unique_ptr<Trip>> & trips) const
{
	std::unique_ptr<Trip> the_most_expensive_trip;
	auto max_price = 0;

	for (const auto& t : trips)
	{
		const auto actual_price = t->price;

		if (actual_price > max_price)
		{
			max_price = actual_price;
			the_most_expensive_trip = std::make_unique<Trip>(*t);
		}
	}

	return the_most_expensive_trip;
}

std::map<int, std::unique_ptr<Trip>> Offer::number_of_participants_with_the_most_expensive_trip() const
{
	std::map<int, std::unique_ptr<Trip>> number_of_participants_with_the_most_expensive_trip;
	const auto number_of_participants_with_trips = trips_sort_by_participants();

	std::set<std::unique_ptr<Trip>> set_trips;

	for (const auto& [number, trips] : number_of_participants_with_trips)
	{
		set_trips.clear();
		for (const auto& t : trips)
		{
			set_trips.emplace(std::make_unique<Trip>(*t));
		}

		number_of_participants_with_the_most_expensive_trip.emplace(number, find_the_most_expensive_trip(set_trips));
	}

	return number_of_participants_with_the_most_expensive_trip;
}

std::vector<std::unique_ptr<TravelAgency>> Offer::get_all_agencies() const
{
	std::vector<std::unique_ptr<TravelAgency>> travel_agencies;

	for (const auto& [travel_agency, trips] : travel_agency_with_trips)
	{
		travel_agencies.emplace_back(std::make_unique<TravelAgency>(*travel_agency));
	}

	return travel_agencies;
}

std::ostream& operator<<(std::ostream & out, const Offer & offer)
{
	for (const auto& pair : offer.travel_agency_with_trips)
	{
		out << "\n-----TRAVEL AGENCY-----\n";
		out << *pair.first << std::endl;
		out << "-----TRIPS-----\n";
		for (const auto& trips : pair.second)
		{
			out << *trips << std::endl;
		}
	}
	return out;
}