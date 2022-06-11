// Copyright (C) 2021 John Doll

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include "Kmeans.h"

// It is ok to use the following namespace delarations in C++ source
// files only. They must never be used in header files.
using namespace std;
using namespace std::string_literals;

/**
 * This method splits up the line it receives and creates a point from
 * the corresponding values.
 * 
 * \param[in] ifs The stringstream line that will be split into separate values.
 * 
 * \param[in] numCols The number of columns that will be read in per line to
 * create the point.
 * 
 * \return The point created from the line sent to this method.
 */
Point split(istringstream& ifs, int numCols) {
    // create string to get each value in the string and the point to be
    // returned
    string value;
    Point p(numCols);
    for (int i = 0; i < numCols; i++) {
        ifs >> quoted(value);
        // add each dimension to the point
        p[i] = stod(value);
    }
    return p;
}

/**
 * Reads in the file specified in the command line arguments and returns a 
 * PointList from the file.
 * 
 * \param[in] is The file stream to be read.
 * 
 * \param[in] numCols The number of columns that we will be reading per line
 * when creating points.
 * 
 * \return The PointList of all points in the file.
 */
PointList parseFile(ifstream& is, int numCols) {
    // create a PointList and line to read each line and add corresponding
    // points to
    PointList pl;
    string line;
    while (getline(is, line)) {
        // interprets line if it isn't empty and doesn't begin with #
        if (line != "" && line.substr(0, 1) != "#") {
            istringstream iss(line);
            Point p = split(iss, numCols);
            pl.push_back(p);
        }
    }
    return pl;
}

/**
 * Finds the centroid closest to the given point.
 * 
 * \param[in] p Point that we are trying to find the closest centroid for.
 * 
 * \param[in] centroids All centroids
 * 
 * \return the index in centroids of the closest centroid to point p.
 */
int getClosestCentroid(Point p, PointList centroids) {
    // set index of closest point to 0 and the intial distance to the distance
    // between the point and the first centroid
    int closest = 0;
    double smallest = distance(p, centroids[0]);
    for (size_t i = 0; i < centroids.size(); i++) {
        // if the distance between the specified centroid and the given point
        // is less than the current smallest distance, then we have a new 
        // closest centroid, so set smallest to that distance and set closest
        // equal to that location
        if (distance(p, centroids[i]) < smallest) {
            smallest = distance(p, centroids[i]);
            closest = i;
        }
    }
    return closest;
}

/**
 * Gets the points closest to a specific centroid.
 * 
 * \param[in] pl PointList of all points.
 * 
 * \param[in] centIdx Index of which centroid each point is closest to.
 * 
 * \param[in] loc The specific centroid index we are trying to find which 
 * points are closest to it. 
 * 
 * \return A PointList of all points that are closest to the specified centroid.
 */
PointList specificCentroids(PointList pl, IntVec centIdx, int loc) {
    // create PointList of potential matching points
    PointList matches;
    for (size_t i = 0; i < centIdx.size(); i++) {
        // if the centroid at i equals the location, then this point belongs to
        // the centroid we are evaluating
        if (centIdx[i] == loc) {
            matches.push_back(pl[i]);
        }
    }
    return matches;
}

/**
 * This method creates a new centroid by centering it with the points it is a 
 * centroid for.
 * 
 * \param[in] matches The PointList of all points that are closest to this
 * centroid.
 * 
 * \return The new location of the centroid.
 */
Point computeNewCentroid(PointList matches) {
    // Get the number of matches and create a new point with the correct 
    // dimensions as this will be our new centroid
    int size = matches.size();
    Point newCentroid(matches[0].size());
    // add all the points into the new centroid
    for (auto pt : matches) {
        newCentroid += pt;
    }
    // divide each dimension of the centroid by the size in order to average
    // the centroid out
    for (size_t i = 0; i < newCentroid.size(); i++) {
        newCentroid[i] /= size;
    }
    return newCentroid;
}

/**
 * Checks if the previous and current centroids are the same.
 * 
 * \param[in] prevCentroids The old PointList of centroids.
 * 
 * \param[in] centroids The new PointList of centroids.
 * 
 * \return true if they are the same and false otherwise
 */
bool centroidsSame(PointList prevCentroids, PointList centroids) {
    for (size_t i = 0; i < centroids.size(); i++) {
        for (size_t j = 0; j < centroids[0].size(); j++) {
            // if points are not equal, then return false as the PointLists are
            // not the same
            if (centroids[i][j] != prevCentroids[i][j]) {
                return false;
            }
        } 
    }
    return true;
}

/**
 * Returns the index of the closest centroids to each point.
 * 
 * \param[in] pl The PointList of all points.
 * 
 * \param[in] centroids The PointList of all centroids.
 * 
 * \param[in] iterations The number of iterations to run.
 * 
 * \return The index of the centroid closest to each corresponding point.
 */
IntVec setClosestCentroid(PointList pl, PointList centroids, int iterations) {
    // creation iteration counter and centroid index vector to be returned
    int iteration = 0;
    IntVec retCentIdx;
    while (iteration < iterations) {
        IntVec centIdx;
        // find closest centroid for each point
        for (size_t i = 0; i < pl.size(); i++) {
            centIdx.push_back(getClosestCentroid(pl.at(i), centroids));
        }
        // save current list of centroids in old list
        PointList prevCentroids = centroids;
        // rearrange centroids to be more in the center of their list of points
        for (size_t i = 0; i < centroids.size(); i++) {
            PointList matches = specificCentroids(pl, centIdx, i);
            centroids[i] = computeNewCentroid(matches);
        }
        // check if the centroid lists are the same so that we don't do 
        // unnecessary iterations
        if (centroidsSame(prevCentroids, centroids)) {
            break;
        }
        retCentIdx = centIdx;
        iteration++;
    }
    return retCentIdx;
}

// main method
int main(int argc, char *argv[]) {
    ifstream is(argv[1]);
    if (is.good()) {
        // create PointList of all points
        PointList pl = parseFile(is, stoi(argv[2]));
        IntVec centIdx;
        PointList centroids;
        // if argv[3] is greater than 0, then we have centroids to work with
        if (stoi(argv[3]) > 0) {
            // get centroids
            centroids = getInitCentroid(pl, stoi(argv[3]));
            int maxIterations = 100;
            // get which centroid the points are closest to
            centIdx = setClosestCentroid(pl, centroids, maxIterations);
        }
        // output results
        writeResults(pl, centroids, centIdx, cout);
    }
    return 0;
}

// End of source code
