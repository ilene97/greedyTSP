/******************************************************************
 * Author: Drew Stroshine
 * Date Created: September 18, 2015
 * Last Modification Date: September 23, 2015
 * filename: greedyTSP.cpp
 *
 * Overview:
 *	This program will determine the minimum cost path for the
 *	travelling salesman problem by using the closest edge 
 *  insertion algorithm.
 *
 * Input:
 *	Any .tsp file. For development I will be using Random30.tsp
 *  and Random40.tsp, which contain 30 and 40 ordered pairs that
 *  each pertain to a different city.
 *
 * Output:
 *	Possible minimum cost path for the cities in the input file.
 ******************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <list>
#include <ctime>
using namespace std;

struct cities {
    string cityNum;
    double x;
    double y;
};

// FUNCTION PROTOTYPES
void calcDistFromLine(list<cities> &path, list<cities> &locs);
list<cities> nearestNode(list<cities> &locs);
void displayList(list<cities> &L);
list<cities> getCoords(string fileName);
double calcDist(cities &point1, cities &point2);



int main(int argc, const char * argv[]) {
    /********** READ LOCATIONS OF CITIES FROM TSP FILE AND STORE THEM IN VECTOR *******************/
    list<cities> locsForty = getCoords("Random40.tsp");
    list<cities>::iterator it;
    
    
    /******************* INITIALIZE PATH AND FIND NEAREST NODE ***************************/
    list<cities> pathForty = nearestNode(locsForty);
    
    
    /******************* START CLOSEST EDGE INSERTION FOR 40 CITIES ***************************/
    cout << "FORTY CITIES SOLUTION" << endl;
    cout << "Path: ";
    displayList(pathForty);
    cout << endl;
    
    clock_t beginForty = clock();
    while (!locsForty.empty()) {
        calcDistFromLine(pathForty, locsForty);
        
        // Display path
        cout << "Path: ";
        displayList(pathForty);
        cout << endl;
    }
    clock_t endForty = clock();
    
    // Display algorithm duration
    double durationForty = double(endForty - beginForty) / CLOCKS_PER_SEC;
    printf("Closest Edge Insertion time for forty cities: %f\n", durationForty);
    
    // Calculate and display path distance
    double distance = 0;
    cities next;
    for (it = pathForty.begin(); it != --pathForty.end(); ++it) {
        ++it;
        next = *it;
        --it;
        
        distance += calcDist(*it, next);
    }
    printf("Forty cities path distance: %f\n", distance);
    
    
    return 0;
}


/******************************************************************
 *						  calcDistFromLine
 * This function calculates the distance from a line made by two 
 * nodes in path to a node in locs. It compares all these 
 * combinations and adds the mininum distance node to path and 
 * removes it from locs. It takes the path and locs list by 
 * reference and output is void.
 ******************************************************************/
void calcDistFromLine(list<cities> &path, list<cities> &locs) {
    // Initialize variables and iterators
    double currDist;
    double minDist = 100000;
    list<cities>::iterator itP, itL;
    double x1, x2, y1, y2;
    
    // Initialize minCityNum and edgeNode
    itL = locs.begin();
    string minCityNum = itL->cityNum;
    itP = path.begin();
    string edgeNode = itP->cityNum;
    
    // Calculate distance between a line of two nodes in path and a node in locs
    for (itP = path.begin(); itP != --path.end(); ++itP) {
        for (itL = locs.begin(); itL != locs.end(); ++itL) {
            // Set x1, y1, x2, y2 variables to corresponding nodes of path
            x1 = itP->x;
            y1 = itP->y;
            ++itP;
            x2 = itP->x;
            y2 = itP->y;
            --itP;
            
            // Calculate current distance
            currDist = abs((x2 - x1)*(y1 - itL->y) - (x1 - itL->x)*(y2 - y1));
            currDist /= sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
            
            // Show what's going on
            /*
            cout << "Distance from line " << itP->cityNum << ",";
            ++itP;
            cout << itP->cityNum << " to node " << itL->cityNum << ": ";
            --itP;
            printf("%f\n", currDist);
             */
            
            // Replace minDist and edgeNode if currDist is lower
            if (currDist < minDist) {
                minDist = currDist;
                minCityNum = itL->cityNum;
                ++itP;
                edgeNode = itP->cityNum;
                --itP;
            }
        }
    }
    
    // Display minDist and minCityNum
    /*
    printf("\nminDist is %f ", minDist);
    cout << "at loc " << minCityNum << endl;
    cout << "edgeNode: " << edgeNode << endl;
     */
    
    // Add node to correct part of path
    itL = locs.begin();
    while (itL->cityNum != minCityNum) {
        ++itL;
    }
    itP = path.begin();
    while (itP->cityNum != edgeNode) {
        ++itP;
    }
    path.insert(itP, *itL);
    locs.erase(itL);
    
}


/******************************************************************
 *						  nearestNode
 * This function takes a list of cities by reference and adds the
 * nearest node to the start node to the path list. It also removes
 * this node from the list passed as a parameter.The function
 * outputs the path list.
 ******************************************************************/
list<cities> nearestNode(list<cities> &locs) {
    list<cities> path;
    list<cities>::iterator it;
    double currDist;
    double minDist = 100000;
    string minCityNum = "0";
    
    // Add starting node to path
    path.push_back(locs.front());
    
    // Calculate distance between each node and start node and save minimum distance
    for (it = ++locs.begin(); it != locs.end(); ++it) {
        currDist = calcDist(path.front(), *it);
//        printf("Distance between 0 and %d: %f\n", it->cityNum, currDist);
        
        // Check if currDist is less than minDist
        if (currDist < minDist) {
            minDist = currDist;
            minCityNum = it->cityNum;
        }
    }
    
//    // Display minDist and minCityNum
//    printf("\nminDist is %f at loc %d \n", minDist, minCityNum);
    
    // Add node with minDist to path and remove from locs
    it = locs.begin();
    while (it->cityNum != minCityNum) {
        ++it;
    }
    path.push_back(*it);
    locs.erase(it);
    
    // Add start of locs to the end of path and remove start from locs
    path.push_back(cities());
    // Setting cityNum to different value than start so that they are not confused
    path.back().cityNum = "END";
    path.back().x = locs.front().x;
    path.back().y = locs.front().y;
    locs.pop_front();
    
    return path;
}

/******************************************************************
 *						  displayList
 * This function reads a constant list of cities by reference
 * and displays the elements of the list.
 ******************************************************************/
void displayList(list<cities> &L) {
    list<cities>::iterator it = L.begin();
    
    while (it != L.end()) {
        cout << it->cityNum << " ";
        ++it;
    }
}


/******************************************************************
 *						  getCoords
 * This function reads a TSP file to retrieve the coordinate values
 * and then populates a vector of cities structures with the 
 * values. The method returns this populated vector.
 ******************************************************************/
list<cities> getCoords(string fileName) {
    // Declare variables and objects
    ifstream input;
    list<cities> coordVals;
    int dimension;
    string buffer, x, y;
    
    // Open inputFile
    input.open(fileName);
    
    if (input) {
        // Find dimension
        while (buffer != "DIMENSION:") {
            input >> buffer;
        }
        input >> buffer;
        dimension = atoi(buffer.c_str());
        
        // Find start of coordinate section
        while (buffer != "NODE_COORD_SECTION") {
            input >> buffer;
        }
        
        // Populate coordVals vector
        for (int i = 0; i < dimension; i++) {
            // Read coordinates from file
            input >> buffer;
            input >> x;
            input >> y;
            
            // Create new struct inside vector element and populate x and y members
            coordVals.push_back(cities());
            coordVals.back().cityNum = to_string(i);
            coordVals.back().x = atof(x.c_str());
            coordVals.back().y = atof(y.c_str());
        }
    }
    
    // Close input file
    input.close();
    
    return coordVals;
}


/******************************************************************
 *						  calcDist
 * This function takes two cities structures by reference,
 * calculates the distance between them, and outputs the result
 * as a double.
 ******************************************************************/
double calcDist(cities &point1, cities &point2) {
    // Calculate x part of distance formula
    double x = point2.x - point1.x;
    x *= x;
    
    // Calculate y part of distance formula
    double y = point2.y - point1.y;
    y *= y;
    
    // Return the calculated distance
    return sqrt(x+y);
}

















