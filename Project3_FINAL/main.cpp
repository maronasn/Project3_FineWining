#include <fstream>
#include <tuple>
#include <chrono>
#include <iostream>
#include "Wine.h"
#include "HashTable.h"
#include "RedBlackTree.h"

using namespace std;

vector<Wine*> wineCellar; // Global vector that holds pointers to dynaimically allocated wine data.
void readWineCSV(); // Reads wine data into wineCellar vector.
tuple <Wine::Properties, bool, bool > getUserSpecifications(); // Gets user input and returns specification for preformSearch function.
void preformSearch(tuple<Wine::Properties, bool, bool> userSpecifications);
void printResults(vector<Wine*> RBTreeResults);
void deleteWines(); // Deallocates pointers stored in wineCellar and clear out wine cellar.
void loadbar(float percentage); // Used to show progress in Red-Black Tree and Hash Table construction.
bool yesOrNoReq(string outputReq); // Get user response for (y/n) questions.

void readWineCSV() {
    if (!wineCellar.empty()) deleteWines();

    string title;
    string country;
    string variety;
    string province;
    string price;
    string points;

    ifstream file("winemag-data-130k-v2.csv");
    if (file.is_open()) {
        string line;
        getline(file, line);
        while (getline(file, line)) {
            istringstream stream(line);
            getline(stream, title, ',');
            getline(stream, country, ',');
            getline(stream, variety, ',');
            getline(stream, province, ',');
            getline(stream, price, ',');
            getline(stream, points, ',');

            wineCellar.push_back(new Wine(title, country, province, variety, (char)stoi(points), stoi(price)));
        }
        file.close();
    }
}

tuple<Wine::Properties, bool, bool> getUserSpecifications() {
    int input = 0;
    // Keeps track of what info is being asked of the user. 
    bool gettingSearch = true;
    bool gettingDataStruct = false;

    // User specification variables to be returned.
    Wine::Properties searchBy = Wine::Properties::NONE;
    bool useRBTree = false;
    bool useHashTable = false;

    while (gettingSearch) {
        cout << "Menu Options" << endl;
        cout << endl;
        cout << "1. Search by wine variety" << endl;
        cout << "2. Search by country" << endl;
        cout << "3. Search by title" << endl;
        cout << "4. Search by province" << endl;
        cout << "5. Exit" << endl;
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;

        if (cin.fail()) {
            cout << "Invalid Input. Try again. " << endl;
            cout << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (input) {
        case 1:
            searchBy = Wine::Properties::VARIETY;
            cout << "Search by Wine Variety.\n" << endl;
            gettingSearch = false;
            gettingDataStruct = true;
            break;
        case 2:
            searchBy = Wine::Properties::COUNTRY;
            cout << "Search by Wine Country.\n" << endl;
            gettingSearch = false;
            gettingDataStruct = true;
            break;
        case 3:
            searchBy = Wine::Properties::TITLE;
            cout << "Search by Wine Title.\n" << endl;
            gettingSearch = false;
            gettingDataStruct = true;
            break;
        case 4:
            searchBy = Wine::Properties::PROVINCE;
            cout << "Search by Wine Province.\n" << endl;
            gettingSearch = false;
            gettingDataStruct = true;
            break;
        case 5:
            gettingSearch = false;
            gettingDataStruct = false;
            continue;
        default:
            cout << "Invalid Input. Try again." << endl;
            cout << endl;
        }

        while (gettingDataStruct) {
            useRBTree = false;
            useHashTable = false;
            cout << "Which Data Structure To Test?" << endl;
            cout << "1. Use Red-Black Tree" << endl;
            cout << "2. Use Hash Table" << endl;
            cout << "3. Use Both Data Structures" << endl;
            cout << "4. Go Back" << endl;
            cin >> input;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << endl;

            if (cin.fail()) {
                cout << "Invalid Input. Try again." << endl;
                cout << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (input) {
            case 1:
                useRBTree = true;
                gettingDataStruct = false;
                break;
            case 2:
                useHashTable = true;
                gettingDataStruct = false;
                break;
            case 3:
                useRBTree = true;
                useHashTable = true;
                gettingDataStruct = false;
                break;
            case 4:
                gettingSearch = true;
                gettingDataStruct = false;
                continue;
            default:
                cout << "Invalid Input. Try again." << endl;
                cout << endl;
            }
        }
    }
    // Returned tuple is read by perform search function.
    return make_tuple(searchBy, useRBTree, useHashTable);  .
}

// Receives inputs from getUserSpecification function.
void preformSearch(tuple<Wine::Properties, bool, bool> userSpecifications) {
    Wine::Properties searchBy;
    bool useRBTree;
    bool useHashTable;
    tie(searchBy, useRBTree, useHashTable) = userSpecifications;
    string searchKey;

    switch (searchBy) {
    case Wine::Properties::NONE:
        return;
    case Wine::Properties::VARIETY:
        cout << "Enter Wine Variety to Search: ";
        break;
    case Wine::Properties::COUNTRY:
        cout << "Enter Country to Search: ";
        break;
    case Wine::Properties::TITLE:
        cout << "Enter Wine Title to Search: ";
        break;
    case Wine::Properties::PROVINCE:
        cout << "Enter Province/State to Search: ";
        break;
    }
    getline(cin, searchKey);
    cout << endl;

    // Used to store construction and search times for each data structure. 
    chrono::milliseconds RBTConstructTime, HTConstructTime;
    chrono::microseconds RBTSearchTime, HTSearchTime;

    // Used to store search results for each data structure. 
    vector<Wine*> RBTSearchResults, HTSearchResults;

    if (useRBTree) {
        // RBTree construction:
        auto RBTConstructStart = chrono::high_resolution_clock::now();
        cout << "Constructing Red Black Tree (" << wineCellar.size() << " elements):" << endl;
        RedBlackTree rbTree(searchBy);
        for (unsigned int i = 0; i < wineCellar.size(); i++) {
            rbTree.insert(wineCellar[i]);
            if ((i + 1) % (wineCellar.size() / 100) == 0)
                loadbar((float)(i + 1) / wineCellar.size());
        }
        loadbar(1.0);
        auto RBTConstructStop = chrono::high_resolution_clock::now();
        RBTConstructTime = chrono::duration_cast<chrono::milliseconds> (RBTConstructStop - RBTConstructStart);
        cout << endl;

        // RBTree search:
        auto RBTSearhStart = chrono::high_resolution_clock::now();
        cout << "Searching Red Black Tree now for \"" << searchKey << "\"... ";
        Wine wineSearchKey;
        wineSearchKey.setValue(searchKey, searchBy);
        rbTree.search(&wineSearchKey, RBTSearchResults);
        cout << "Done" << endl;
        auto RBTSearchStop = chrono::high_resolution_clock::now();
        RBTSearchTime = chrono::duration_cast<chrono::microseconds> (RBTSearchStop - RBTSearhStart);
        cout << endl;
    }

    if (useHashTable) {
        // HashTable Construction:
        auto HTConstructStart = chrono::high_resolution_clock::now();
        HashTable hashTable(searchBy);
        cout << "Constructing Hash Table (" << wineCellar.size() << " elements):" << endl;
        for (unsigned int i = 0; i < wineCellar.size(); i++) {
            hashTable.insert(wineCellar[i]);
            if ((i + 1) % (wineCellar.size() / 100) == 0)
                loadbar((float)(i + 1) / wineCellar.size());
        }
        loadbar(1.0);
        auto HTConstructStop = chrono::high_resolution_clock::now();
        HTConstructTime = chrono::duration_cast<chrono::milliseconds> (HTConstructStop - HTConstructStart);
        cout << endl;

        // HashTable Search:
        auto HTSearchStart = chrono::high_resolution_clock::now();
        cout << "Searching Hash Table now for \"" << searchKey << "\"... ";
        hashTable.search(searchKey, HTSearchResults);
        cout << "Done" << endl;
        auto HTSearchStop = chrono::high_resolution_clock::now();
        HTSearchTime = chrono::duration_cast<chrono::microseconds> (HTSearchStop - HTSearchStart);
        cout << endl;
    }

    // Displays the construction and search times.
    if (useRBTree) {
        cout << "Red-Black Tree Results" << endl;
        cout << setw(21) << "Construction time: " << RBTConstructTime.count() << " ms." << endl;
        cout << setw(21) << "Search time: " << RBTSearchTime.count() << " microseconds" << endl;
        cout << "\tFound " << RBTSearchResults.size() << " matches!" << endl;
        cout << endl;
    }
    if (useHashTable) {
        cout << "Hash Table Results" << endl;
        cout << setw(21) << "Construction time: " << HTConstructTime.count() << " ms." << endl;
        cout << setw(21) << "Search time: " << HTSearchTime.count() << " microseconds." << endl;
        cout << "\tFound " << HTSearchResults.size() << " matches!" << endl;
        cout << endl;
    }

    // Prompt to print results.
    if (!RBTSearchResults.empty()) {
        if (yesOrNoReq("Print out results? (y/n) "))
            printResults(RBTSearchResults);
    }
    if (!HTSearchResults.empty()) {
        if (yesOrNoReq("Print out results? (y/n) "))
            printResults(HTSearchResults);
    }
}

// Iterates through results based on number selection.
void printResults(vector<Wine*> results)
{
    int numPrinted[] = { 10, 25, 50, 100 };
    int input = 0;
    int size = results.size();
    bool gettingNumPrinted = true;
    bool gettingSortBy = false;

    int numToPrint = 0;
    Wine::Properties sortBy = Wine::Properties::NONE;
    int optionChoice = 1;
    while (gettingNumPrinted) {
        if (results.size() > 10)
        {
            cout << "Print results: " << endl;
            for (int i : numPrinted) {
                if (size > i)
                    cout << optionChoice++ << ". Print top " << i << " results." << endl;
                else
                    break;
            }
            cout << optionChoice << ". Print all results." << endl;
            cin >> input;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << endl;

            if (cin.fail()) {
                cout << "Invalid Input. Try again." << endl;
                cout << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (input < 0 || input > optionChoice) {
                cout << "Invalid Input. Try again." << endl;
                cout << endl;
            }
            else if (input == optionChoice) {
                numToPrint = size;
                gettingNumPrinted = false;
                if (results.size() != 1)
                    gettingSortBy = true;
            }
            else {
                numToPrint = numPrinted[input - 1];
                gettingNumPrinted = false;
                if (results.size() != 1)
                    gettingSortBy = true;
            }
        }
        else {
            numToPrint = size;
            gettingNumPrinted = false;
            if (results.size() != 1)
                gettingSortBy = true;
        }
        while (gettingSortBy) {
            cout << "Print top results by: " << endl;
            cout << "1. Best Prices" << endl;
            cout << "2. Top Rated" << endl;
            cin >> input;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << endl;

            if (cin.fail()) {
                cout << "Invalid Input. Try again." << endl;
                cout << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (input) {
            case 1:
                sortBy = Wine::Properties::PRICE;
                gettingSortBy = false;
                break;
            case 2:
                sortBy = Wine::Properties::RATING;
                gettingSortBy = false;
                break;
            default:
                cout << "Invalid Input. Try again. " << endl;
            }
        }
    }

    Wine::sortWine(results, sortBy);

    // Finds width of each column in table to be printed.
    int maxTitleWid = 6;
    int maxProvCountryWid = 18;
    int maxVarietyWid = 8;
    for (int i = 0; i < numToPrint; i++) {
        int titleWid = results[i]->getTitle().size();
        int provCountryWid = results[i]->getCountry().size() + results[i]->getProvince().size();
        int varietyWid = results[i]->getVariety().size();
        if (titleWid > maxTitleWid)
            maxTitleWid = titleWid;
        if (provCountryWid > maxProvCountryWid)
            maxProvCountryWid = provCountryWid;
        if (varietyWid > maxVarietyWid)
            maxVarietyWid = varietyWid;
    }
    // Prints table for results.
    cout << left << setw(maxTitleWid + 6) << "     Title" << "| " <<
        setw(maxProvCountryWid + 3) << "Province, Country" << "| " <<
        setw(maxVarietyWid + 1) << "Variety" << "| " <<
        setw(5) << "$" << "| Rating" << endl;
    cout << setfill('=') << setw(maxTitleWid + maxProvCountryWid + maxVarietyWid + 31) << "" << endl;
    cout << setfill(' ');
    for (int i = 0; i < numToPrint; i++) {
        cout << setw(5) << to_string(i + 1) + ". " << results[i]->toString(maxTitleWid, maxProvCountryWid, maxVarietyWid) << endl;
    }
    cout << endl;
}

void deleteWines() {
    for (Wine* wine : wineCellar) {
        delete wine;
    }
    wineCellar.clear();
}

void loadbar(float percentage)
{
    int barWidth = 70;
    int pos = barWidth * percentage;

    cout << " [";
    for (int i = 0; i < barWidth; i++) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(percentage * 100.0) << "%";
    if (percentage != 1)
        cout << "\r";
    else
        cout << " Done" << endl;
    cout.flush();
}

bool yesOrNoReq(string outputReq)
{
    while (true)
    {
        string input;
        cout << outputReq;
        cin >> input;
        transform(input.begin(), input.end(), input.begin(), tolower);
        if (input == "y" || input == "yes") {
            cout << endl;
            return true;
        }
        else if (input == "n" || input == "no") {
            cout << endl;
            return false;
        }
        else {
            cout << "Invalid Response. Try again. " << endl;
            cout << endl;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}


int main() {
    readWineCSV();

    while (true) {
        preformSearch(getUserSpecifications());
        if (yesOrNoReq("Do you want to perform another search? (y/n) ")) {
            if (yesOrNoReq("Do you want to clear the console of past results? (y/n) "))
                cout << "\x1b[2J\x1b[H"; // Clears console and returns cursor back to home position.
            else
                cout << endl;
        }
        else
            break;
    }

    deleteWines();
    return 0;
}