#include "Wine.h"


Wine::Wine() : title(string()), country(string()), province(string()), variety(string()), rating(0), price(0) { }
Wine::Wine(string _name, string _country, string _province, string _variety, char _rating, int _price) :
    title(_name), country(_country), province(_province), variety(_variety), rating(_rating), price(_price) {}


string Wine::toString(int titleWid, int countryProvWid, int varietyWid) const
{
    std::stringstream wineStr;
    wineStr << std::left << std::setw(titleWid + 1) << title << "| ";
    wineStr << std::setw(countryProvWid + 3) << (province + ", " + country) << "| ";
    wineStr << std::setw(varietyWid + 1) << variety << "| ";
    wineStr << std::setw(5) << getPriceStr() << "| ";
    wineStr << std::right << std::setw(2) << std::to_string(rating) << " points";

    return wineStr.str();
}

string Wine::getTitle() const {
    return title;
}

string Wine::getCountry() const {
    return country;
}

string Wine::getProvince() const {
    return province;
}

string Wine::getVariety() const {
    return variety;
}

string Wine::getPriceStr() const
{
    if (price == 0) return string("N/A");
    return string("$" + std::to_string(price));
}

int Wine::getRating() const {
    return rating;
}



int Wine::getPrice() const {
    return price;
}


void Wine::setTitle(string _title)
{
    title = _title;
}

void Wine::setCountry(string _country)
{
    country = _country;
}

void Wine::setProvince(string _province)
{
    province = _province;
}

void Wine::setVariety(string _variety)
{
    variety = _variety;
}

void Wine::setRating(int _rating)
{
    rating = _rating;
}

void Wine::setPrice(int _price)
{
    price = _price;
}

void Wine::setValue(string s, Properties val)
{
    switch (val) {
    case Wine::Properties::VARIETY:
        variety = s;
        break;
    case Wine::Properties::COUNTRY:
        country = s;
        break;
    case Wine::Properties::TITLE:
        title = s;
        break;
    case Wine::Properties::PROVINCE:
        province = s;
        break;
    }
}

int Wine::titleComp(const Wine* w1, const Wine* w2)
{
    return w1->title.compare(w2->title);
}

int Wine::countryComp(const Wine* w1, const Wine* w2)
{
    return w1->country.compare(w2->country);
}

int Wine::provinceComp(const Wine* w1, const Wine* w2)
{
    return w1->province.compare(w2->province);
}

int Wine::varietyComp(const Wine* w1, const Wine* w2)
{
    return w1->variety.compare(w2->variety);
}

bool Wine::priceComp(const Wine* w1, const Wine* w2)
{
    if (w1->price == 0) return false;
    else if (w2->price == 0) return true;
    return w1->price < w2->price;
}

bool Wine::ratingComp(const Wine* w1, const Wine* w2)
{
    return w1->rating > w2->rating;
}

void Wine::sortWine(vector<Wine*>& wines, Properties sortBy)
{
    switch (sortBy) {
    case Wine::Properties::PRICE:
        std::sort(wines.begin(), wines.end(), Wine::priceComp);
        break;
    case Wine::Properties::RATING:
        std::sort(wines.begin(), wines.end(), Wine::ratingComp);
        break;
    }
}