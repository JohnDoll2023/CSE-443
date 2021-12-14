/**
 * Copyright (C) 2021 John Doll
 * This source file contains several short (exam-style) methods that
 * involve the use of synchronization and collective communication
 * operations.
 *
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <functional>
#include <boost/mpi.hpp>

// Some convenience namespaces to streamline code
using namespace std;
using namespace boost::mpi;
namespace mpi = boost::mpi;

/**
 * This method is called on all process in an MPI program. Each This
 * method must compute and return the sum and variance of the 'value's
 * (1st parameter) on all processes.
 *
 * \note This method must use only collective communication
 * operations. Hint: all_reduce.
 */
std::tuple<double, double> getMeanAndVar(double value) {
    // declare mean and variance
    double avg = 0;
    double variance = 0;

    // create communicator
    mpi::communicator world;
    mpi::all_reduce(world, value, avg, std::plus<double>());
    // get avg
    avg /= world.size();

    const auto localVar = (value - avg) * (value - avg);
    mpi::all_reduce(world, localVar, variance, std::plus<double>());
    variance /= world.size();

    // Return the mean and variance.
    return {avg, variance};
}

/**
 * This method is called on all process in an MPI program. This
 * method must collect all the 'values's from all processes into a
 * vector and return the full list values at all processes.
 *
 * \note This method must use only collective communication
 */
std::vector<int> aggregate(int value) {
    // create communicator
    mpi::communicator world;
    std::vector<int> ret(world.size());
    mpi::gather(world, value, ret, 0);
    // send info to all processes
    mpi::broadcast(world, &ret[0], world.size(), 0);
    return ret;
}

/**
 * This method is called on every process in an MPI program to print
 * the value associated with the rank.  However, the output appears in
 * random/garbled order.
 *
 * Modify the program to print output in the order of the rank of the
 * process.
 *
 * \param[in] value The value to be printed by the process.
 *
 */
void printInOrder(const std::string& value) {
    // create communicator
    mpi::communicator world;
    for (int i = 0; (i < world.size()); i++) {
        // barrier to wait on
        world.barrier();
        if (i == world.rank()) {
            std::cout << "Value at rank #" << world.rank() << " = "
                      << std::quoted(value) << std::endl;
        }
    }
}
// End of source code
