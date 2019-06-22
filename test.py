import graph_analysis

def test_graph():
    print("Test vertex")
    graph = graph_analysis.BaseGraph()
    print("Order: {}".format(graph.order()))
    print("Size: {}".format(graph.size()))
    v = graph_analysis.Vertex("MyVertex".encode("utf-8"))
    print("Test label: {}".format(v.getLabel()))

    print("Add {}".format(graph.addVertex(v)))
    print("New Size: {}".format(graph.size()))
    print("New Order: {}".format(graph.order()))
    #print( graph_analysis.MD5Digest.md5Sum("test") )
    io = graph_analysis.GraphIO()
    io.write(b"test-py.gexf", graph)
