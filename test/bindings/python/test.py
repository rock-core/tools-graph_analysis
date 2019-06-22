import graph_analysis

def test_init_base_graph():
    graph = graph_analysis.BaseGraph()
    assert graph.order() == 0
    assert graph.size() == 0

def test_init_vertex():
    vFromLabel = "FromLabel"
    vFrom = graph_analysis.Vertex(vFromLabel)
    assert vFrom.getLabel() == vFromLabel
    assert vFrom.toString() == vFromLabel

def test_init_edge():
    vFrom = graph_analysis.Vertex("from")
    vTo = graph_analysis.Vertex("to")

    edge = graph_analysis.Edge("connection")
    edge.setSourceVertex(vFrom)
    edge.setTargetVertex(vTo)

    assert edge.getLabel() == "connection"
    assert edge.toString() == "connection"
    assert edge.getSourceVertex() == vFrom
    assert edge.getTargetVertex() == vTo

def test_graph():
    graph = graph_analysis.BaseGraph()
    vFrom = graph_analysis.Vertex("from")
    vTo = graph_analysis.Vertex("to")

    edge = graph_analysis.Edge("connection")
    edge.setSourceVertex(vFrom)
    edge.setTargetVertex(vTo)

    idx = graph.addVertex(vFrom)
    assert idx == 0
    assert graph.getVertexId(vFrom) == idx
    assert graph.getVertex(idx) == vFrom

    idx = graph.addVertex(vTo)
    assert idx == 1
    assert graph.getVertexId(vTo) == idx
    assert graph.getVertex(idx) == vTo

    assert graph.size() == 0
    assert graph.order() == 2

    graph.addEdge(edge)
    assert graph.size() == 1

    io = graph_analysis.GraphIO()
    io.write("/tmp/graph-analysis-testing-io-py.gexf", graph)

    graph.removeVertex(vFrom)
    assert graph.size() == 0
    assert graph.order() == 1

