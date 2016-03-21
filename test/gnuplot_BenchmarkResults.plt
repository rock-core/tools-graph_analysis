#set terminal svg enhanced size 2000 1000 fname "Times" fsize 36
set style fill transparent solid 0.5 noborder
set terminal png truecolor enhanced size 1920,1080 font "Times,36"

set yrange [0:]
set output "plot-AddNodes.png"
set title "Add Nodes"
#set style fill transparent pattern 4 bo
set xlabel "Number of nodes"
set ylabel "Time in microseconds"
plot "snap_addNodes.dat"                        using 1:(($2-$3)*10E06):(($2+$3)*10E06) title "snap raw"  w filledcurves, \
    "lemon_addNodes.dat"                        using 1:(($2-$3)*10E6):(($2+$3)*10E06) title "lemon raw" w filledcurves, \
    "snap::DirectedGraph_addNodes.dat"          using 1:(($2-$3)*10E6):(($2+$3)*10E06) title "snap ga"   w filledcurves, \
    "boost_graph::DirectedGraph_addNodes.dat"   using 1:(($2-$3)*10E6):(($2+$3)*10E06) title "boost ga"  w filledcurves, \
    "lemon::DirectedGraph_addNodes.dat"         using 1:(($2-$3)*10E6):(($2+$3)*10E06) title "lemon ga"  w filledcurves


set output "plot-GetNodes.png"
set title "Get Nodes"
set xlabel "Number of nodes"
set ylabel "Time in microseconds"
plot "snap_getNodes.dat"                        using 1:($2*10E6) title "snap raw" w lines, \
    "lemon_getNodes.dat"                        using 1:($2*10E6) title "lemon raw" w lines, \
    "snap::DirectedGraph_getNodes.dat"          using 1:($2*10E6) title "snap ga" w lines, \
    "boost_graph::DirectedGraph_getNodes.dat"   using 1:($2*10E6) title "boost ga" w lines, \
    "lemon::DirectedGraph_getNodes.dat"         using 1:($2*10E6) title "lemon ga" w lines

set output "plot-IterateNodes.png"
set title "Iterate Nodes"
set xlabel "Number of nodes"
set ylabel "Time in microseconds"
plot "snap_iterateNodes.dat"                        using 1:($2*10E06) title "snap raw" w lines, \
    "lemon_iterateNodes.dat"                        using 1:($2*10E06) title "lemon raw" w lines, \
    "snap::DirectedGraph_iterateNodes.dat"          using 1:($2*10E06) title "snap ga" w lines, \
    "boost_graph::DirectedGraph_iterateNodes.dat"   using 1:($2*10E06) title "boost ga" w lines, \
    "lemon::DirectedGraph_iterateNodes.dat"         using 1:($2*10E06) title "lemon ga" w lines

set output "plot-AddEdges.png"
set title "Add Edges"
set xlabel "Number of nodes"
set ylabel "Time in microseconds"
plot "snap_addEdges.dat"                        using 1:($2*10E06) title "snap raw" w lines, \
    "lemon_addEdges.dat"                        using 1:($2*10E06) title "lemon raw" w lines, \
    "snap::DirectedGraph_addEdges.dat"          using 1:($2*10E06) title "snap ga" w lines, \
    "boost_graph::DirectedGraph_addEdges.dat"   using 1:($2*10E06) title "boost ga" w lines, \
    "lemon::DirectedGraph_addEdges.dat"         using 1:($2*10E06) title "lemon ga" w lines

set output "plot-GetEdges.png"
set title "Get Edges"
set xlabel "Number of nodes"
set ylabel "Time in microseconds"
plot "snap::DirectedGraph_getEdges.dat"         using 1:($2*10E06) title "snap ga" w lines, \
    "boost_graph::DirectedGraph_getEdges.dat"   using 1:($2*10E06) title "boost ga" w lines, \
    "lemon::DirectedGraph_getEdges.dat"         using 1:($2*10E06) title "lemon ga" w lines
#   "snap_getEdges.dat" using 1:2 title "snap raw",
#    "lemon_getEdges.dat" using 1:2 title "lemon raw",

set output "plot-IterateEdges.png"
set title "Iterate Edges"
set xlabel "Number of nodes"
set ylabel "Time in microseconds"
plot "snap_iterateEdges.dat"                        using 1:($2*10E06) title "snap raw" w lines, \
    "lemon_iterateEdges.dat"                        using 1:($2*10E06) title "lemon raw" w lines, \
    "snap::DirectedGraph_iterateEdges.dat"          using 1:($2*10E06) title "snap ga" w lines, \
    "boost_graph::DirectedGraph_iterateEdges.dat"   using 1:($2*10E06) title "boost ga" w lines, \
    "lemon::DirectedGraph_iterateEdges.dat"         using 1:($2*10E06) title "lemon ga" w lines

set output "plot-IterateSTLEdges.png"
set title "Iterate Edges with STL"
set xlabel "Number of nodes"
set ylabel "Time in microseconds"
plot "snap_iterateEdges.dat"                            using 1:($2*10E06) title "snap raw" w lines, \
    "lemon_iterateEdges.dat"                            using 1:($2*10E06) title "lemon raw" w lines, \
    "snap::DirectedGraph_iterateStlEdges.dat"           using 1:($2*10E06) title "snap ga" w lines, \
    "boost_graph::DirectedGraph_iterateStlEdges.dat"    using 1:($2*10E06) title "boost ga" w lines, \
    "lemon::DirectedGraph_iterateStlEdges.dat"          using 1:($2*10E06) title "lemon ga" w lines


