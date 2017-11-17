#include "SoplexSolver.hpp"

namespace graph_analysis {
namespace algorithms {

SoplexSolver::SoplexSolver()
    : LPSolver()
{
}

SoplexSolver::~SoplexSolver()
{
}

void SoplexSolver::saveProblem(const std::string& filename, ProblemFormat format) const
{
    switch(format)
    {
        case CPLEX:
            mSoplex.writeFileReal(filename.c_str(), NULL, NULL, NULL);
            break;
        default:
            break;
    }
}

void SoplexSolver::loadSolution(const std::string& filename, SolutionType format)
{
    // unknwon
}

void SoplexSolver::saveSolution(const std::string& filename, LPSolver::SolutionType format) const
{
    // unknown
}

double SoplexSolver::getObjectiveValue() const
{
    return const_cast<soplex::SoPlex*>(&mSoplex)->objValueReal();
}

double SoplexSolver::getVariableValueByColumnIdx(uint32_t idx) const
{
    //if(mSoplex.status() == soplex::SPxSolver::INFEASIBLE)
    //{
        if(mSoplex.hasPrimal())
        {
            soplex::DVector primal(mSoplex.numColsReal());
            const_cast<soplex::SoPlex*>(&mSoplex)->getPrimalReal(primal);
            return primal[idx];
        }

        if(mSoplex.hasDual())
        {
            soplex::DVector dual(mSoplex.numRowsReal());
            const_cast<soplex::SoPlex*>(&mSoplex)->getDualReal(dual);
            return dual[idx];
        }
    //}
    return mSoplex.objReal(idx);
}

LPSolver::Status SoplexSolver::run(const std::string& problem, LPSolver::ProblemFormat problemFormat, bool useCaching)
{
    LPSolver::Status status = LPSolver::STATUS_UNKNOWN;
    bool knownProblem = loadProblem(problem, problemFormat);

    if(status == LPSolver::STATUS_UNKNOWN)
    {
        soplex::SPxSolver::Status stat = mSoplex.optimize();
        switch(stat)
        {
            case soplex::SPxSolver::OPTIMAL:
                //soplex.getPrimalReal(prim);
                //soplex.getDualReal(dual);
                std::cout << "LP solved to optimality.\n";
                std::cout << "Objective value is " << mSoplex.objValueReal() << ".\n";
                std::cout << "Column names are: ";
                for(int i = 0; i < mColumnNames.size(); ++i)
                {
                    std::cout <<  mColumnNames[i] << std::endl;
                }
                //std::cout << "Primal solution is [" << prim[0] << ", " << prim[1] << "].\n";
                //std::cout << "Dual solution is [" << dual[0] << "].\n";
                return LPSolver::STATUS_OPTIMAL;
            case soplex::SPxSolver::INFEASIBLE:
                {
                    std::cout << "LP is infeasible: farkas proof" << std::endl;
                    if(mSoplex.hasDualFarkas())
                    {
                        std::cout << "Farkas proof is available" << std::endl;
                        std::cout << "    " << mSoplex.numColsReal() << std::endl;
                        double* data = new double[mSoplex.numRowsReal()];
                        soplex::Vector farkasReal(mSoplex.numRowsReal(),data);
                        if( mSoplex.getDualFarkasReal(farkasReal) )
                        {
                            //std::cout << "    farkas length: " << farkasReal << std::endl;
                            for(int i = 0; i < mSoplex.numRowsReal(); ++i)
                            {
                                if(farkasReal[i] != 0.0)
                                {
                                    std::cout << "#" << i << ":"  << farkasReal[i] << std::endl;
                                    mInfeasibleConstraints.push_back(i);
                                }
                            }
                        }

                        std::cout << "Primal: " << mSoplex.hasPrimal() << std::endl;
                        std::cout << "Basis: " << mSoplex.hasBasis() << std::endl;
                        if(mSoplex.hasBasis())
                        {
                            soplex::DVector basis(mSoplex.numColsReal());
                            //mSoplex.getBa
                        }
                        std::cout << "Primal ray: " << mSoplex.hasPrimalRay() << std::endl;
                        if(mSoplex.hasPrimalRay())
                        {
                            soplex::DVector primal(mSoplex.numColsReal());
                            mSoplex.getPrimalRayReal(primal);
                            for(int i = 0; i < mSoplex.numColsReal(); ++i)
                            {
                                if(soplex::isNotZero( primal[i]) )
                                {
                                    std::cout << "col" << i << ": " << primal[i] << std::endl;
                                }
                            }
                        }

                        std::cout << "Dual: " << mSoplex.hasDual() << std::endl;
                        if(mSoplex.hasDual())
                        {
                            soplex::DVector dual(mSoplex.numRowsReal());
                            mSoplex.getDualReal(dual);
                            for(int i = 0; i < mSoplex.numColsReal(); ++i)
                            {
                                if(soplex::isNotZero( dual[i]) )
                                {
                                    std::cout << "col" << i << ": " << dual[i] << std::endl;
                                }
                            }
                        }

                    } else {
                        std::cout << "No farkas proof available" << std::endl;
                    }
                }
                break;
            default:
                break;
        }
    }
    return LPSolver::STATUS_UNKNOWN;
}

void SoplexSolver::doLoadProblem(const std::string& filename, ProblemFormat format)
{
    switch(format)
    {
        case CPLEX:
            mSoplex.readFile(filename.c_str(), &mRowNames, &mColumnNames);
            break;
        default:
            break;
    }
}

} // end namespace algorithms
} // end namespace graph_analysis
