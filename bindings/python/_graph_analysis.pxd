from libcpp.memory cimport shared_ptr
from libcpp.string cimport string
from cython.operator cimport dereference as deref

cdef extern from "GraphElement.hpp" namespace "graph_analysis":
    cdef cppclass GraphElement:
        GraphElement()
        string getLabel()
        string getClassname()
        string toString()
        string toString(int)

cdef extern from "Vertex.hpp" namespace "graph_analysis":
    cdef cppclass Vertex(GraphElement):
        Vertex()
        Vertex(string)
        shared_ptr[Vertex] clone()

cdef extern from "Edge.hpp" namespace "graph_analysis":
    cdef cppclass Edge(GraphElement):
        Edge()
        Edge(string)
        Edge(shared_ptr[Vertex], shared_ptr[Vertex], string)
        shared_ptr[Edge] clone()
        void setSourceVertex(shared_ptr[Vertex])
        void setTargetVertex(shared_ptr[Vertex])

        shared_ptr[Vertex] getSourceVertex()
        shared_ptr[Vertex] getTargetVertex()


cdef extern from "BaseGraph.hpp" namespace "graph_analysis":
## http://docs.cython.org/en/latest/src/userguide/wrapping_CPlusPlus.html#static-member-method

    cdef cppclass BaseGraph:
        enum ImplementationType:
            BOOST_DIRECTED_GRAPH = 0,
            LEMON_DIRECTED_GRAPH = 1,
            SNAP_DIRECTED_GRAPH = 2,
            IMPLEMENTATION_TYPE_END = 3

        BaseGraph()

        BaseGraph(ImplementationType,bool)

        @staticmethod
        shared_ptr[BaseGraph] getInstance()

        int addVertex(shared_ptr[Vertex])
        void removeVertex(shared_ptr[Vertex])
        bint contains(shared_ptr[Vertex])
        int getVertexId(shared_ptr[Vertex])
        shared_ptr[Vertex] getVertex(int)

        int addEdge(shared_ptr[Edge])
        void removeEdge(shared_ptr[Edge])
        int removeEdges(shared_ptr[Vertex],shared_ptr[Vertex])
        bint contains(shared_ptr[Edge])
        int getEdgeId(shared_ptr[Edge])
        shared_ptr[Edge] getEdge(int)

        int size()
        int order()

cdef extern from "GraphIO.hpp" namespace "graph_analysis::io":
    cdef cppclass GraphIO:
        @staticmethod
        string write(string, shared_ptr[BaseGraph])

        @staticmethod
        void read(string, shared_ptr[BaseGraph])

cdef extern from "utils/MD5.hpp" namespace "graph_analysis::utils":
    cdef cppclass MD5Digest:
        @staticmethod
        string md5Sum(string)

