import module_name
g = module_name.GraphString(True)
g.addNode("A")
g.addNode("B")
g.addNode("C")
g.addNode("D")
g.addNode("E")
g.addEdge("A", "B", 2)
g.addEdge("B", "C", 3)
g.addEdge("D", "C", 3)
g.addEdge("D", "E", 4)
x = g.connected()
print(x)
print(g.hasCycle())
g.addEdge("E", "D", 1)
print(g.hasCycle())
