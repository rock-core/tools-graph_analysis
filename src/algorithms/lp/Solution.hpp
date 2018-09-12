#ifndef GRAPH_ANALYSIS_ALGORITHMS_LP_SOLUTION_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_LP_SOLUTION_HPP

#include <map>
#include <string>
#include "../LPSolver.hpp"

namespace graph_analysis {
namespace algorithms {
namespace lp {

/**
 * Representation class for an LP Solution
 */
class Solution
{
public:
    double getColumnValue(const std::string& columnName) const;
    void setColumnValue(const std::string& columnName, double value) { mColumnValues[columnName] = value; }

    double getObjectiveValue() const { return mObjectiveValue; }
    void setObjectiveValue(double value) { mObjectiveValue = value; }

    void setNumberOfRows(size_t n) { mNumberOfRows = n; }
    size_t getNumberOfRows() const { return mNumberOfRows; }

    void setNumberOfColumns(size_t n) { mNumberOfColumns = n; }
    size_t getNumberOfColumns() const { return mNumberOfColumns; }

    void setNumberOfNonZeros(size_t n) { mNumberOfNonZeros = n; }
    size_t getNumberOfNonZeros() const { return mNumberOfNonZeros; }

    void setStatus(LPSolver::Status status) { mStatus = status; }
    LPSolver::Status getStatus() const { return mStatus; }

    std::string toString(size_t indent = 0) const;

private:
    std::map<std::string, double> mColumnValues;
    double mObjectiveValue;
    LPSolver::Status mStatus;
    size_t mNumberOfRows;
    size_t mNumberOfColumns;
    size_t mNumberOfNonZeros;

};

} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_LP_SOLUTION_HPP
