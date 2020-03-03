#pragma once

#include "trip_repository.h"
#include "travel_agency_utils.h"
#include "libs.h"

using offer_map = std::unordered_map<std::unique_ptr<TravelAgency>, std::unordered_set<std::unique_ptr<Trip>, TripHasher, TripComparator>, TravelAgencyHasher, TravelAgencyComparator>;

class Offer
{
private:
	offer_map travel_agency_with_trips;
	std::shared_ptr<TripRepository> trip_repository;
	std::string json_filename;

	double count_avg_price(int id) const;
	Trip find_trip_price_closest_to_avg(int id) const;
	std::unordered_set<std::string> get_all_countries() const;
	TravelAgency find_travel_agency_with_max_occurencies_of_country(const std::string& country) const;
	std::vector<std::string> read_file(const std::string& file_name) const;
	std::set<int> get_all_numbers_of_paricipants() const;
	std::set<std::unique_ptr<Trip>> find_trips_with_same_amount_of_people(int amount_of_people) const;
	std::unique_ptr<Trip> find_the_most_expensive_trip(std::set<std::unique_ptr<Trip>>& trips) const;

public:
	Offer(const std::string& json_name, const std::shared_ptr<TripRepository>& trip_repository);

	offer_map fill_map() const;
	std::string the_most_trips() const;
	std::string the_most_money_make() const;
	std::string the_most_common_travel_destination() const;
	std::unordered_map<double, std::unique_ptr<Trip>> avg_cost_of_trips() const;
	std::map<std::string, std::unique_ptr<TravelAgency>> group_countries_with_best_travel_agencies() const;
	std::vector<std::unique_ptr<Trip>> trips_only_to_europe() const;
	std::map<int, std::set<std::unique_ptr<Trip>>> trips_sort_by_participants() const;
	std::map<int, std::unique_ptr<Trip>> number_of_participants_with_the_most_expensive_trip() const;
	std::set<std::unique_ptr<Trip>> get_all_trips() const;
	std::vector<std::unique_ptr<TravelAgency>> get_all_agencies() const;

	friend std::ostream& operator<<(std::ostream& out, const Offer& offer);
	~Offer() = default;
};