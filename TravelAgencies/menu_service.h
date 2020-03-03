#pragma once

#include "offer.h"

class MenuService
{
private:
	std::shared_ptr<Offer> o;

	static int print_menu();
	void option1() const;
	void option2() const;
	void option3() const;
	void option4() const;


public:
	MenuService(const std::string& json_filename, const std::shared_ptr<TripRepository>& trip_repository);

	void main_menu() const;

	~MenuService() = default;


};