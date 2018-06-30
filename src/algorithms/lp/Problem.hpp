#ifndef GRAPH_ANALYSIS_ALGORITHMS_LP_PROBLEM_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_LP_PROBLEM_HPP

#include <string>
#include <limits>
#include <vector>
#include <map>
#include "../LPSolver.hpp"

namespace graph_analysis {
namespace algorithms {
namespace lp {

typedef std::string Variable;
typedef std::vector<Variable> Variables;
typedef std::map<Variable, double> ObjectiveFunction;
enum ValueType  { Integer, Continuous, Binary };
enum BoundType { Free, Lower, Upper, LowerUpper, Exact };
enum OptimizationDirection { OPTIMIZE_MIN, OPTIMIZE_MAX };

struct Bounds
{
    Bounds();
    Bounds(double min, double max, BoundType type = LowerUpper);

    double min;
    double max;

    BoundType type;

    static std::string minBound(double min);
    static std::string maxBound(double max);

    std::string toLowerUpperString(const std::string& variable) const;
};

struct MatrixEntry
{
    MatrixEntry();
    MatrixEntry(const std::string& columName,
            double coefficient);

    std::string name;
    double coefficient;
};

struct Column : MatrixEntry
{
    Column();
    Column(const std::string& name,
            const Bounds& bounds,
            double coefficient = 1.0);

    Bounds bounds;
    ValueType type;
};

struct Row
{
    Row();
    Row(const std::string& name,
            const Bounds& bounds);

    std::string name;
    Bounds bounds;
    std::vector<MatrixEntry> entries;
};


class Problem
{
public:

    /**
     * \param name Name of the problem
     * \param optimizationDirection Direction of the optimization, i.e. minimize
     * or maximize
     */
    Problem(const std::string& name,
            OptimizationDirection optimizationDirection = OPTIMIZE_MIN);

    void setRowBounds(const std::string& row, double min, double max);
    void setRowBoundsExact(const std::string& row, double exact) { setRowBounds(row, exact, exact); }
    void addRow(const Row& row);
    Row& rowByName(const std::string& name);
    /**
     * Use offset to account for start idx like GLPK does at 1
     */
    Row& rowByIdx(size_t idx, size_t offset = -1);


    void setColumnBounds(const std::string& col, double min, double max);
    void setColumnBoundsExact(const std::string& col, double exact) { setColumnBounds(col, exact, exact); }
    void addColumn(const Column& column, bool doThrow = false);
    /**
     * Use offset to account for start idx like GLPK does at 1
     */
    Column& columnByIdx(size_t idx, size_t offset = -1);

    std::string saveProblemToTempfile(LPSolver::ProblemFormat format = LPSolver::CPLEX);
    std::string toString(size_t indent = 0) const;

private:
    std::string mName;
    std::vector<Column> mColumns;
    std::vector<Row> mRows;

    OptimizationDirection mOptimizationDirection;

};
} // end namespace lp
} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_LP_PROBLEM_HPP
