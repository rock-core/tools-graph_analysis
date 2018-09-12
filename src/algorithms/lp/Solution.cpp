#include "Solution.hpp"
#include <sstream>
#include <iomanip>
#include <base-logging/Logging.hpp>

namespace graph_analysis {
namespace algorithms {
namespace lp {

double Solution::getColumnValue(const std::string& columName) const
{

    std::map<std::string, double>::const_iterator cit = mColumnValues.find(columName);
    if(cit != mColumnValues.end())
    {
        return cit->second;
    }

    // e.g. SCIP solver does  not explictly state value that differ from 0.0
    LOG_INFO_S << "graph_analysis::algorithms::lp::Solution::getColumnValue: "
           "could not find column value for '" + columName + "' -- defaulting to 0.0";
    return 0.0;
}

std::string Solution::toString(size_t indent) const
{
    std::string hspace(indent,' ');
    std::stringstream ss;
    ss << hspace << "Problem:" << std::endl;
    ss << hspace << "Rows:       " << getNumberOfRows() << std::endl;
    ss << hspace << "Columns:    " << getNumberOfColumns() << std::endl;
    ss << hspace << "Non-zeros:  " << getNumberOfNonZeros() << std::endl;
    ss << hspace << "Status:     " << LPSolver::StatusTxt[ getStatus() ] << std::endl;
    ss << hspace << "Objective:  " << getObjectiveValue() << std::endl;
    ss << std::endl;

    ss << hspace << "Column      Activity" << std::endl;
    for(const std::pair<std::string, double> p : mColumnValues)
    {
        ss << hspace << std::setw(10) << p.first
            << std::setw(10) << p.second << std::endl;
    }

    return ss.str();
}

} // end namespace lp
} // end namespace algorithms
} // end graph_analysis
