#include "Problem.hpp"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <base-logging/Logging.hpp>

namespace graph_analysis {
namespace algorithms {
namespace lp {

Bounds::Bounds()
    : min(std::numeric_limits<double>::min())
    , max(std::numeric_limits<double>::max())
    , type(Free)
{}

Bounds::Bounds(double min, double max, BoundType type)
    : min(min)
    , max(max)
    , type(type)
{}

std::string Bounds::toLowerUpperString(const std::string& variable) const
{
    return minBound(min) + " <= " + variable + " <= " + maxBound(max);
}

std::string Bounds::minBound(double min)
{
    std::stringstream ssMin;
    if(min < std::numeric_limits<int>::min())
    {
        ssMin << "- inf ";

    } else {
        ssMin << static_cast<int>(min);
    }
    return ssMin.str();
}


std::string Bounds::maxBound(double max)
{

    std::stringstream ssMax;
    if(max > std::numeric_limits<int>::max())
    {
        ssMax << "+ inf ";

    } else {
        ssMax << static_cast<int>(max);
    }
    return ssMax.str();
}

Problem::Problem(const std::string& name, OptimizationDirection optimizationDirection)
    : mName(name)
    , mOptimizationDirection(optimizationDirection)
{}

MatrixEntry::MatrixEntry(const std::string& columName,
        double coefficient)
    : name(columName)
    , coefficient(coefficient)
{
}

Row::Row()
{}

Row::Row(const std::string& name,
        const Bounds& bounds)
    : name(name)
    , bounds(bounds)
{}

MatrixEntry::MatrixEntry()
    : coefficient(1.0)
{}

Column::Column()
    : MatrixEntry()
    , type(Integer)
{
}

Column::Column(const std::string& name,
        const Bounds& bounds, double coefficient)
    : MatrixEntry(name, coefficient)
    , bounds(bounds)
    , type(Integer)
{
}

void Problem::setRowBounds(const std::string& rowName,
        double min,
        double max)
{
    Row& row = rowByName(rowName);
    row.bounds = Bounds(min, max);
}

Row& Problem::rowByName(const std::string& rowName)
{
    std::vector<Row>::iterator it = std::find_if(mRows.begin(), mRows.end(),
            [rowName](const Row& other)
            {
                return other.name == rowName;
            });
    if(it != mRows.end())
    {
        return *it;
    }
    throw std::invalid_argument("graph_analysis::algorithms::Problem: no row"
            "'" + rowName + "'");
}

Row& Problem::rowByIdx(size_t idx, size_t offset)
{
    return mRows.at(idx + offset);
}

void Problem::addRow(const Row& row)
{
    LOG_DEBUG_S << "Adding row: " << row.name << " to existing: " <<
        mRows.size();

    std::vector<Row>::const_iterator cit = std::find_if(mRows.begin(), mRows.end(),
            [row](const Row& other)
            {
                return other.name == row.name;
            });

    if(cit != mRows.end())
    {
        throw std::invalid_argument("graph_analysis::algorithms::Problem::addRow: "
                " row '" + row.name + "' has already been added");
    }
    mRows.push_back(row);
}

void Problem::addColumn(const Column& column, bool doThrow)
{
    if(doThrow)
    {
        std::vector<Column>::const_iterator cit = std::find_if(mColumns.begin(),
                mColumns.end(), [column](const Column& other)
                {
                    return other.name == column.name;
                });
        if(cit != mColumns.end())
        {
            throw std::invalid_argument("graph_analysis::algorithms::Problem::addColumn: "
                    " column '" + column.name + "' has already been added");
        }
    }
    mColumns.push_back(column);
}

Column& Problem::columnByIdx(size_t idx, size_t offset)
{
    return mColumns.at(idx + offset);
}

std::string Problem::toString(size_t indent) const
{
    std::string hspace(indent, ' ');
    std::stringstream ss;
    ss << "\\* Problem: " << mName << " *\\" << std::endl;

    ss << hspace << "Minimize" << std::endl;
    ss << hspace << " obj:";
    size_t lineBreakCounter = 0;
    for(const Column& c : mColumns)
    {
        if(c.coefficient == 0)
        {
            continue;
        }

        if(++lineBreakCounter%12 == 0)
        {
            ss << std::endl << hspace;
        }

        ss << " +";
        if(c.coefficient != 1.0)
        {
            ss << " "  << static_cast<size_t>(c.coefficient);
        }
        ss << " " << c.name;
    }
    ss << std::endl;
    ss << std::endl;
    ss << hspace << "Subject To" << std::endl;
    size_t rowCounter = 1;
    for(const Row& row : mRows)
    {
        lineBreakCounter = 0;
        ss << hspace << " " << row.name << ":";
        for(const MatrixEntry& entry : row.entries)
        {
            if(entry.coefficient == 0)
            {
                continue;
            }

            if(++lineBreakCounter%12 == 0)
            {
                ss << std::endl << hspace;
            }
            if (entry.coefficient >= 0)
            {
                ss << " +" ;
            } else {
                ss << " -";
            }

            if( entry.coefficient != 1.0)
            {
                ss << static_cast<size_t>(fabs(entry.coefficient));
            }

            ss << " " << entry.name;

        }
        switch(row.bounds.type)
        {
            case Free:
            case LowerUpper:
                ss << " - ~" << "r_";
                ss << rowCounter++;
                ss << " = 0" << std::endl;
                break;
            case Lower:
                ss << " >= " << Bounds::minBound(row.bounds.min) << std::endl;
                break;
            case Upper:
                ss << " <= " << Bounds::maxBound(row.bounds.max) << std::endl;
                break;
            case Exact:
                {
                    if(row.bounds.min > std::numeric_limits<int>::min()
                            && row.bounds.min < std::numeric_limits<int>::max())
                    {
                        ss << " = " << row.bounds.min << std::endl;
                    } else {
                        throw
                            std::invalid_argument("graph_analysis::algorithms::lp::Problem::toString:"
                                    " exact row bound for '" + row.name + " was"
                                    "+/- infinity");
                    }
                    break;
                }
            default:
                break;
        }
    }
    ss << std::endl;

    ss << hspace << "Bounds" << std::endl;
    rowCounter = 1;
    for(const Row& row : mRows)
    {
        switch(row.bounds.type)
        {
            case Free:
            case LowerUpper:
                {
                    std::stringstream ssVar;
                    ssVar <<"~r_" << rowCounter++;
                    ss << hspace << " "
                        << row.bounds.toLowerUpperString(ssVar.str())
                        << std::endl;
                }
                break;
            default:
                break;
        }
    }
    for(const Column& column : mColumns)
    {
        ss << hspace << " " << column.bounds.toLowerUpperString(column.name) <<
            std::endl;
    }
    ss << std::endl;

    ss << hspace << "Generals" << std::endl;
    for(const Column& column : mColumns)
    {
        ss << hspace << " " << column.name << std::endl;
    }
    ss << std::endl;
    ss << "End";
    ss << std::endl;

    return ss.str();
}

std::string Problem::saveProblemToTempfile(LPSolver::ProblemFormat format)
{
    boost::filesystem::path tempDir = boost::filesystem::temp_directory_path();
    boost::filesystem::path temp = tempDir / boost::filesystem::unique_path();
    std::string filename = temp.native() + ".lp";

    std::ofstream of;
    of.open(filename);
    of << toString();
    of.close();
    return filename;
}

} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
