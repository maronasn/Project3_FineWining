#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;

class Wine {
private:
    // Wine Properties 
    string title;
    string country;
    string province;
    string variety;
    char rating;
    int price;
public:
    enum class Properties { NONE, VARIETY, COUNTRY, TITLE, PROVINCE, RATING, PRICE };

    // used by the RedBlackTree class for comparing keys
    static int titleComp(const Wine* w1, const Wine* w2);
    static int countryComp(const Wine* w1, const Wine* w2);
    static int provinceComp(const Wine* w1, const Wine* w2);
    static int varietyComp(const Wine* w1, const Wine* w2);

    // used as Compare binary function std::sort function
    static bool priceComp(const Wine* w1, const Wine* w2);
    static bool ratingComp(const Wine* w1, const Wine* w2);

    // used to order search results by either rating or price
    static void sortWine(vector<Wine*>& wines, Properties sortBy);

    Wine();
    Wine(std::string _name, string _country, string _province, string _variety, char _rating, int _price);

    // accessor functions
    string getTitle() const;
    string getCountry() const;
    string getProvince() const;
    string getVariety() const;
    string getPriceStr() const;
    int getRating() const;
    int getPrice() const;

    // for converting Wine object to string for printing. 
    string toString(int titleWid, int CocountryProvWid, int varietyWid) const;

    // manipulator functions 
    void setTitle(string _name);
    void setCountry(string _country);
    void setProvince(string _province);
    void setVariety(string _variety);
    void setRating(int _rating);
    void setPrice(int _price);
    void setValue(string s, Properties val);
};