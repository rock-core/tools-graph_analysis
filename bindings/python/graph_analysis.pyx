# distutils: language = c++
# cython: language_level=2

from cython.operator cimport dereference as deref
from libcpp.memory cimport shared_ptr
from libcpp.string cimport string
from _graph_analysis cimport Vertex as _Vertex
from _graph_analysis cimport Edge as _Edge
from _graph_analysis cimport BaseGraph as _BaseGraph
from _graph_analysis cimport MD5Digest as _MD5Digest
from _graph_analysis cimport GraphIO as _GraphIO
from _graph_analysis cimport GraphElement as _GraphElement

cdef class MD5Digest:
    @staticmethod
    def md5Sum(txt):
        return _MD5Digest.md5Sum(txt)

cdef class Vertex:
    cdef shared_ptr[_Vertex] thisptr

    def __init__(self, label = ""):
        self.thisptr.reset(new _Vertex(label.encode("utf-8")))

    cdef _Vertex __vertex(self):
        return deref(self.thisptr.get())

    def getLabel(self):
        return self.__vertex().getLabel().decode("utf-8")

    def toString(self, indent = 0):
        return self.__vertex().toString(indent).decode("utf-8")

    def __eq__(self, Vertex other):
        return self.thisptr == other.thisptr

    def __ne__(self, Vertex other):
        return not self.__eq__(other)

cdef class Edge:
    cdef shared_ptr[_Edge] thisptr

    def __init__(self, label = ""):
        self.thisptr.reset(new _Edge(label.encode("utf-8")))

    cdef _Edge __edge(self):
        return deref(self.thisptr.get())

    def getLabel(self):
        return self.__edge().getLabel().decode("utf-8")

    def toString(self, indent = 0):
        return self.__edge().toString(indent).decode("utf-8")

    cpdef setSourceVertex(self, Vertex v):
        deref(self.thisptr.get()).setSourceVertex(v.thisptr)

    cpdef setTargetVertex(self, Vertex v):
        deref(self.thisptr.get()).setTargetVertex(v.thisptr)

    cdef shared_ptr[_Vertex] __getSourceVertex(self):
        return deref(self.thisptr.get()).getSourceVertex()

    cdef shared_ptr[_Vertex] __getTargetVertex(self):
        return deref(self.thisptr.get()).getTargetVertex()

    def getSourceVertex(self):
        v = Vertex()
        v.thisptr = self.__getSourceVertex()
        return v

    def getTargetVertex(self):
        v = Vertex()
        v.thisptr = self.__getTargetVertex()
        return v

    def __eq__(self, Edge other):
        return self.thisptr == other.thisptr

    def __ne__(self, Edge other):
        return not self.__eq__(other)

cdef class BaseGraph:
    cdef shared_ptr[_BaseGraph] thisptr

    def __init__(self):
        self.thisptr = _BaseGraph.getInstance()

    cpdef addVertex(self, Vertex v):
        cdef shared_ptr[_Vertex] ptr = v.thisptr
        return deref(self.thisptr.get()).addVertex(ptr)

    cpdef removeVertex(self, Vertex v):
        cdef shared_ptr[_Vertex] ptr = v.thisptr
        return deref(self.thisptr.get()).removeVertex(ptr)

    cpdef addEdge(self, Edge e):
        cdef shared_ptr[_Edge] ptr = e.thisptr
        return deref(self.thisptr.get()).addEdge(ptr)

    cpdef removeEdge(self, Edge e):
        cdef shared_ptr[_Edge] ptr = e.thisptr
        return deref(self.thisptr.get()).removeEdge(ptr)

    cpdef removeEdges(self, Vertex a, Vertex b):
        cdef shared_ptr[_Vertex] ptrA = a.thisptr
        cdef shared_ptr[_Vertex] ptrB = b.thisptr
        return deref(self.thisptr.get()).removeEdges(ptrA,ptrB)

    cpdef bint containsEdge(self, Edge e):
        cdef shared_ptr[_Edge] ptr = e.thisptr
        return deref(self.thisptr.get()).contains(ptr)

    cpdef bint containsVertex(self, Vertex v):
        cdef shared_ptr[_Vertex] ptr = v.thisptr
        return deref(self.thisptr.get()).contains(ptr)

    cpdef getVertexId(self, Vertex v):
        cdef shared_ptr[_Vertex] ptr = v.thisptr
        return deref(self.thisptr.get()).getVertexId(ptr)

    cpdef getEdgeId(self, Edge e):
        cdef shared_ptr[_Edge] ptr = e.thisptr
        return deref(self.thisptr.get()).getEdgeId(ptr)

    cpdef Vertex getVertex(self, int vid):
        cdef shared_ptr[_Vertex] ptr = deref(self.thisptr.get()).getVertex(vid)
        v = Vertex()
        v.thisptr = ptr
        return v

    cpdef Edge getEdge(self, int eid):
        cdef shared_ptr[_Edge] ptr = deref(self.thisptr.get()).getEdge(eid)
        e = Edge()
        e.thisptr = ptr
        return e

    def order(self):
        return deref(self.thisptr.get()).order()

    def size(self):
        return deref(self.thisptr.get()).size()

cdef class GraphIO:
    cpdef write(self, filename, BaseGraph g):
        name = filename.encode("utf-8")
        cdef shared_ptr[_BaseGraph] ptr = g.thisptr
        _GraphIO.write(name, ptr)


    
