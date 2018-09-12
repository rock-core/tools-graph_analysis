SCIP version 4.0.0 [precision: 8 byte] [memory: block] [mode: optimized] [LP solver: SoPlex 3.0.0] [GitHash: a80a247]
Copyright (C) 2002-2017 Konrad-Zuse-Zentrum fuer Informationstechnik Berlin (ZIB)

External codes: 
  Readline 6.3         GNU library for command line editing (gnu.org/s/readline)
  SoPlex 3.0.0         Linear Programming Solver developed at Zuse Institute Berlin (soplex.zib.de) [GitHash: b0cccbd]
  CppAD 20160000.1     Algorithmic Differentiation of C++ algorithms developed by B. Bell (www.coin-or.org/CppAD)
  ZLIB 1.2.8           General purpose compression library by J. Gailly and M. Adler (zlib.net)
  GMP 6.1.0            GNU Multiple Precision Arithmetic Library developed by T. Granlund (gmplib.org)
  ZIMPL 3.3.4          Zuse Institute Mathematical Programming Language developed by T. Koch (zimpl.zib.de)

reading user parameter file <scip.set>

read problem </tmp/20180911_155048-templ/0/test.lp>
============

original problem has 46 variables (0 bin, 23 int, 0 impl, 23 cont) and 47 constraints

solve problem
=============

presolving:
(round 1, fast)       42 del vars, 41 del conss, 0 add conss, 18 chg bounds, 0 chg sides, 0 chg coeffs, 0 upgd conss, 0 impls, 2 clqs
(round 2, fast)       44 del vars, 44 del conss, 0 add conss, 19 chg bounds, 0 chg sides, 0 chg coeffs, 0 upgd conss, 0 impls, 2 clqs
(round 3, fast)       45 del vars, 47 del conss, 0 add conss, 19 chg bounds, 0 chg sides, 0 chg coeffs, 0 upgd conss, 0 impls, 0 clqs
presolving (4 rounds: 4 fast, 1 medium, 1 exhaustive):
 46 deleted vars, 47 deleted constraints, 0 added constraints, 19 tightened bounds, 0 added holes, 0 changed sides, 0 changed coefficients
 0 implications, 0 cliques
presolved problem has 0 variables (0 bin, 0 int, 0 impl, 0 cont) and 0 constraints
transformed objective value is always integral (scale: 1)
Presolving Time: 0.00

 time | node  | left  |LP iter|LP it/n| mem |mdpt |frac |vars |cons |cols |rows |cuts |confs|strbr|  dualbound   | primalbound  |  gap   
t 0.0s|     1 |     0 |     0 |     - | 726k|   0 |   - |   0 |   0 |   0 |   0 |   0 |   0 |   0 | 6.000000e+00 | 6.000000e+00 |   0.00%

SCIP Status        : problem is solved [optimal solution found]
Solving Time (sec) : 0.00
Solving Nodes      : 1
Primal Bound       : +6.00000000000000e+00 (1 solutions)
Dual Bound         : +6.00000000000000e+00
Gap                : 0.00 %

primal solution (original space):
=================================

objective value:                                    6
x1                                                  1 	(obj:0)
x6                                                  1 	(obj:1)
x8                                                  1 	(obj:1)
x14                                                 1 	(obj:1)
x16                                                 1 	(obj:1)
x18                                                 1 	(obj:1)
x21                                                 1 	(obj:1)
~r_6                                                1 	(obj:0)
~r_8                                                1 	(obj:0)
~r_1                                                1 	(obj:0)
~r_14                                               1 	(obj:0)
~r_16                                               1 	(obj:0)
~r_18                                               1 	(obj:0)
~r_21                                               1 	(obj:0)

Statistics
==========

SCIP Status        : problem is solved [optimal solution found]
Total Time         :       0.00
  solving          :       0.00
  presolving       :       0.00 (included in solving)
  reading          :       0.00
  copying          :       0.00 (0 times copied the problem)
Original Problem   :
  Problem name     : /tmp/20180911_155048-templ/0/test.lp
  Variables        : 46 (0 binary, 23 integer, 0 implicit integer, 23 continuous)
  Constraints      : 47 initial, 47 maximal
  Objective sense  : minimize
Presolved Problem  :
  Problem name     : t_/tmp/20180911_155048-templ/0/test.lp
  Variables        : 0 (0 binary, 0 integer, 0 implicit integer, 0 continuous)
  Constraints      : 0 initial, 0 maximal
Presolvers         :   ExecTime  SetupTime  Calls  FixedVars   AggrVars   ChgTypes  ChgBounds   AddHoles    DelCons    AddCons   ChgSides   ChgCoefs
  boundshift       :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  convertinttobin  :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  domcol           :       0.00       0.00      1          0          0          0          0          0          0          0          0          0
  dualagg          :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  dualcomp         :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  dualinfer        :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  gateextraction   :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  implfree         :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  implics          :       0.00       0.00      1          0          0          0          0          0          0          0          0          0
  inttobinary      :       0.00       0.00      1          0          0          0          0          0          0          0          0          0
  qpkktref         :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  redvub           :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  stuffing         :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  trivial          :       0.00       0.00      4          2          0          0          0          0          0          0          0          0
  tworowbnd        :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  dualfix          :       0.00       0.00      4          1          0          0          0          0          0          0          0          0
  genvbounds       :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  probing          :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  pseudoobj        :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  linear           :       0.00       0.00      3          6         37          0         19          0         47          0          0          0
  components       :       0.00       0.00      0          0          0          0          0          0          0          0          0          0
  root node        :          -          -      -          0          -          -          0          -          -          -          -          -
Constraints        :     Number  MaxNumber  #Separate #Propagate    #EnfoLP    #EnfoRelax  #EnfoPS    #Check   #ResProp    Cutoffs    DomReds       Cuts    Applied      Conss   Children
  integral         :          0          0          0          0          0          0          0          6          0          0          0          0          0          0          0
  countsols        :          0          0          0          0          0          0          0          6          0          0          0          0          0          0          0
  components       :          0          0          0          0          0          0          0          0          0          0          0          0          0          0          0
Constraint Timings :  TotalTime  SetupTime   Separate  Propagate     EnfoLP     EnfoPS     EnfoRelax   Check    ResProp    SB-Prop
  integral         :       0.00       0.00       0.00       0.00       0.00       0.00       0.00       0.00       0.00       0.00
  countsols        :       0.00       0.00       0.00       0.00       0.00       0.00       0.00       0.00       0.00       0.00
  components       :       0.00       0.00       0.00       0.00       0.00       0.00       0.00       0.00       0.00       0.00
Propagators        : #Propagate   #ResProp    Cutoffs    DomReds
  dualfix          :          1          0          0          0
  genvbounds       :          0          0          0          0
  nlobbt           :          0          0          0          0
  obbt             :          0          0          0          0
  probing          :          0          0          0          0
  pseudoobj        :          0          0          0          0
  redcost          :          0          0          0          0
  rootredcost      :          0          0          0          0
  vbounds          :          0          0          0          0
Propagator Timings :  TotalTime  SetupTime   Presolve  Propagate    ResProp    SB-Prop
  dualfix          :       0.00       0.00       0.00       0.00       0.00       0.00
  genvbounds       :       0.00       0.00       0.00       0.00       0.00       0.00
  nlobbt           :       0.00       0.00       0.00       0.00       0.00       0.00
  obbt             :       0.00       0.00       0.00       0.00       0.00       0.00
  probing          :       0.00       0.00       0.00       0.00       0.00       0.00
  pseudoobj        :       0.00       0.00       0.00       0.00       0.00       0.00
  redcost          :       0.00       0.00       0.00       0.00       0.00       0.00
  rootredcost      :       0.00       0.00       0.00       0.00       0.00       0.00
  vbounds          :       0.00       0.00       0.00       0.00       0.00       0.00
Conflict Analysis  :       Time      Calls    Success    DomReds  Conflicts   Literals    Reconvs ReconvLits   Dualrays   Nonzeros   LP Iters   (pool size: [--,--])
  propagation      :       0.00          0          0          -          0        0.0          0        0.0          -          -          -
  infeasible LP    :       0.00          0          0          -          0        0.0          0        0.0          0        0.0          0
  bound exceed. LP :       0.00          0          0          -          0        0.0          0        0.0          -          -          0
  strong branching :       0.00          0          0          -          0        0.0          0        0.0          -          -          0
  pseudo solution  :       0.00          0          0          -          0        0.0          0        0.0          -          -          -
  applied globally :       0.00          -          -          0          0        0.0          -          -          0          -          -
  applied locally  :          -          -          -          0          0        0.0          -          -          0          -          -
Separators         :   ExecTime  SetupTime      Calls    Cutoffs    DomReds       Cuts    Applied      Conss
  cut pool         :       0.00                     0          -          -          0          -          -    (maximal pool size: 0)
  cgmip            :       0.00       0.00          0          0          0          0          0          0
  clique           :       0.00       0.00          0          0          0          0          0          0
  closecuts        :       0.00       0.00          0          0          0          0          0          0
  cmir             :       0.00       0.00          0          0          0          0          0          0
  convexproj       :       0.00       0.00          0          0          0          0          0          0
  disjunctive      :       0.00       0.00          0          0          0          0          0          0
  eccuts           :       0.00       0.00          0          0          0          0          0          0
  flowcover        :       0.00       0.00          0          0          0          0          0          0
  gauge            :       0.00       0.00          0          0          0          0          0          0
  gomory           :       0.00       0.00          0          0          0          0          0          0
  impliedbounds    :       0.00       0.00          0          0          0          0          0          0
  intobj           :       0.00       0.00          0          0          0          0          0          0
  mcf              :       0.00       0.00          0          0          0          0          0          0
  oddcycle         :       0.00       0.00          0          0          0          0          0          0
  rapidlearning    :       0.00       0.00          0          0          0          0          0          0
  strongcg         :       0.00       0.00          0          0          0          0          0          0
  zerohalf         :       0.00       0.00          0          0          0          0          0          0
Pricers            :   ExecTime  SetupTime      Calls       Vars
  problem variables:       0.00          -          0          0
Branching Rules    :   ExecTime  SetupTime   BranchLP  BranchExt   BranchPS    Cutoffs    DomReds       Cuts      Conss   Children
  allfullstrong    :       0.00       0.00          0          0          0          0          0          0          0          0
  cloud            :       0.00       0.00          0          0          0          0          0          0          0          0
  distribution     :       0.00       0.00          0          0          0          0          0          0          0          0
  fullstrong       :       0.00       0.00          0          0          0          0          0          0          0          0
  inference        :       0.00       0.00          0          0          0          0          0          0          0          0
  leastinf         :       0.00       0.00          0          0          0          0          0          0          0          0
  mostinf          :       0.00       0.00          0          0          0          0          0          0          0          0
  multaggr         :       0.00       0.00          0          0          0          0          0          0          0          0
  nodereopt        :       0.00       0.00          0          0          0          0          0          0          0          0
  pscost           :       0.00       0.00          0          0          0          0          0          0          0          0
  random           :       0.00       0.00          0          0          0          0          0          0          0          0
  relpscost        :       0.00       0.00          0          0          0          0          0          0          0          0
Primal Heuristics  :   ExecTime  SetupTime      Calls      Found       Best
  LP solutions     :       0.00          -          -          0          0
  relax solutions  :       0.00          -          -          0          0
  pseudo solutions :       0.00          -          -          0          0
  strong branching :       0.00          -          -          0          0
  actconsdiving    :       0.00       0.00          0          0          0
  bound            :       0.00       0.00          0          0          0
  clique           :       0.00       0.00          0          0          0
  coefdiving       :       0.00       0.00          0          0          0
  completesol      :       0.00       0.00          0          0          0
  crossover        :       0.00       0.00          0          0          0
  dins             :       0.00       0.00          0          0          0
  distributiondivin:       0.00       0.00          0          0          0
  dualval          :       0.00       0.00          0          0          0
  feaspump         :       0.00       0.00          0          0          0
  fixandinfer      :       0.00       0.00          0          0          0
  fracdiving       :       0.00       0.00          0          0          0
  gins             :       0.00       0.00          0          0          0
  guideddiving     :       0.00       0.00          0          0          0
  indicator        :       0.00       0.00          0          0          0
  intdiving        :       0.00       0.00          0          0          0
  intshifting      :       0.00       0.00          0          0          0
  linesearchdiving :       0.00       0.00          0          0          0
  localbranching   :       0.00       0.00          0          0          0
  locks            :       0.00       0.00          0          0          0
  lpface           :       0.00       0.00          0          0          0
  multistart       :       0.00       0.00          0          0          0
  mutation         :       0.00       0.00          0          0          0
  nlpdiving        :       0.00       0.00          0          0          0
  objpscostdiving  :       0.00       0.00          0          0          0
  octane           :       0.00       0.00          0          0          0
  ofins            :       0.00       0.00          0          0          0
  oneopt           :       0.00       0.00          0          0          0
  proximity        :       0.00       0.00          0          0          0
  pscostdiving     :       0.00       0.00          0          0          0
  randrounding     :       0.00       0.00          0          0          0
  rens             :       0.00       0.00          0          0          0
  reoptsols        :       0.00       0.00          0          0          0
  repair           :       0.00       0.00          0          0          0
  rins             :       0.00       0.00          0          0          0
  rootsoldiving    :       0.00       0.00          0          0          0
  rounding         :       0.00       0.00          0          0          0
  shiftandpropagate:       0.00       0.00          0          0          0
  shifting         :       0.00       0.00          0          0          0
  simplerounding   :       0.00       0.00          0          0          0
  subnlp           :       0.00       0.00          0          0          0
  trivial          :       0.00       0.00          2          1          1
  trivialnegation  :       0.00       0.00          0          0          0
  trysol           :       0.00       0.00          0          0          0
  twoopt           :       0.00       0.00          0          0          0
  undercover       :       0.00       0.00          0          0          0
  vbounds          :       0.00       0.00          0          0          0
  veclendiving     :       0.00       0.00          0          0          0
  zeroobj          :       0.00       0.00          0          0          0
  zirounding       :       0.00       0.00          0          0          0
  other solutions  :          -          -          -          0          -
Diving Statistics  :      Calls      Nodes   LP Iters Backtracks   MinDepth   MaxDepth   AvgDepth  NLeafSols  MinSolDpt  MaxSolDpt  AvgSolDpt
  actconsdiving    :          0          -          -          -          -          -          -          -          -          -          -
  coefdiving       :          0          -          -          -          -          -          -          -          -          -          -
  distributiondivin:          0          -          -          -          -          -          -          -          -          -          -
  fracdiving       :          0          -          -          -          -          -          -          -          -          -          -
  guideddiving     :          0          -          -          -          -          -          -          -          -          -          -
  linesearchdiving :          0          -          -          -          -          -          -          -          -          -          -
  pscostdiving     :          0          -          -          -          -          -          -          -          -          -          -
  veclendiving     :          0          -          -          -          -          -          -          -          -          -          -
LP                 :       Time      Calls Iterations  Iter/call   Iter/sec  Time-0-It Calls-0-It    ItLimit
  primal LP        :       0.00          0          0       0.00          -       0.00          0
  dual LP          :       0.00          0          0       0.00          -       0.00          0
  lex dual LP      :       0.00          0          0       0.00          -
  barrier LP       :       0.00          0          0       0.00          -       0.00          0
  diving/probing LP:       0.00          0          0       0.00          -
  strong branching :       0.00          0          0       0.00          -          -          -          0
    (at root node) :          -          0          0       0.00          -
  conflict analysis:       0.00          0          0       0.00          -
B&B Tree           :
  number of runs   :          1
  nodes            :          1 (0 internal, 1 leaves)
  feasible leaves  :          0
  infeas. leaves   :          0
  objective leaves :          0
  nodes (total)    :          1 (0 internal, 1 leaves)
  nodes left       :          0
  max depth        :          0
  max depth (total):          0
  backtracks       :          0 (0.0%)
  early backtracks :          0 (0.0%)
  nodes exc. ref.  :          0 (0.0%)
  delayed cutoffs  :          0
  repropagations   :          0 (0 domain reductions, 0 cutoffs)
  avg switch length:       2.00
  switching time   :       0.00
Root Node          :
  First LP value   :          -
  First LP Iters   :          0
  First LP Time    :       0.00
  Final Dual Bound : +6.00000000000000e+00
  Final Root Iters :          0
  Root LP Estimate :                     -
Solution           :
  Solutions found  :          1 (1 improvements)
  First Solution   : +6.00000000000000e+00   (in run 1, after 1 nodes, 0.00 seconds, depth 0, found by <trivial>)
  Gap First Sol.   :       0.00 %
  Gap Last Sol.    :       0.00 %
  Primal Bound     : +6.00000000000000e+00   (in run 1, after 1 nodes, 0.00 seconds, depth 0, found by <trivial>)
  Dual Bound       : +6.00000000000000e+00
  Gap              :       0.00 %
  Avg. Gap         :       0.00 % (0.00 primal-dual integral)
SCIP version 4.0.0 [precision: 8 byte] [memory: block] [mode: optimized] [LP solver: SoPlex 3.0.0] [GitHash: a80a247]
Copyright (C) 2002-2017 Konrad-Zuse-Zentrum fuer Informationstechnik Berlin (ZIB)

External codes: 
  Readline 6.3         GNU library for command line editing (gnu.org/s/readline)
  SoPlex 3.0.0         Linear Programming Solver developed at Zuse Institute Berlin (soplex.zib.de) [GitHash: b0cccbd]
  CppAD 20160000.1     Algorithmic Differentiation of C++ algorithms developed by B. Bell (www.coin-or.org/CppAD)
  ZLIB 1.2.8           General purpose compression library by J. Gailly and M. Adler (zlib.net)
  GMP 6.1.0            GNU Multiple Precision Arithmetic Library developed by T. Granlund (gmplib.org)
  ZIMPL 3.3.4          Zuse Institute Mathematical Programming Language developed by T. Koch (zimpl.zib.de)

reading user parameter file <scip.set>

read problem <test/data/lp/p0.lp>
============

file <test/data/lp/p0.lp> not found
