#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <utility>
#include <list>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;


template <typename T, typename W> 
class Edge {
    public: 
        Edge(const T& src, const T& dest, const W& weight): src(src), dest(dest), weight(weight){};
        T src, dest;
        W weight;
};



template <typename T, typename W> 
class Graph {
    private:
        int size;
        std::vector<std::vector<W>> edges;           //Contains edges
        std::map<T, int> container;             // Contains nodes and their index in the vectoro

        bool is_directed;


        bool comp_dfs(std::set<int> &visited, T x, std::set<T> &local) const
        {
            int index = container.find(x)->second;
            visited.insert(index);
            local.insert(x);

            for (auto i = container.begin(); i != container.end(); ++i) {
                if (edges[index][i->second] != -1 && visited.find(i->second) == visited.end())
                    comp_dfs(visited, i->first, local);
            }
        }


        bool __dfs(std::set<int> &visited, int x, std::set<int> &local) const
        {
            visited.insert(x);
            local.insert(x);

            for (int i = 0; i < size; ++i) {
                if (edges[x][i] != -1) {
                    if (local.find(i) == local.end() && visited.find(i) == visited.end()) {
                        if (__dfs(visited, i, local))
                            return true;
                    } else if (local.find(i) != local.end()) 
                        return true;
                }
            }

            return false;
        }


    public: 
        Graph(bool directed): is_directed(directed), size(0) {};

        void addNode(const T& data)
        {
            //First, ensure that the node doesn't already exist
            if (container.find(data) != container.end())
                throw std::runtime_error("Node already exists!");        //If the node does already exist, throw an error

            //Otherwise, add the data to the container

            for (int i = 0; i < size; ++i) {
                edges[i].push_back(-1);
            }
            edges.push_back(std::vector<W>(size+1));


            //Insert node's mapping to integer
            container.insert({data, size++});
            
            // Initialize all edges for the new node
            for (int i = 0; i < size; ++i) {
                edges[size-1][i] = -1;
            }

        }

        void printGraph()
        {
            std::cout << "  ";
            for (auto i = container.begin(); i != container.end(); ++i)
                std::cout << i->first << "  ";

            std::cout << std::endl;

            auto cont = container.begin();
            for (int i = 0; i < size; ++i) {
                std::cout << cont->first << " ";
                for (int j = 0; j < size; ++j) 
                {
                    std::cout << edges[i][j] << " ";
                }
                std::cout << "\n";
                ++cont;
            }
        }

        void addEdge(const T& src, const T& dest, const W& weight = W())
        {
            int node_size = container.size();
            bool src_found = false, dest_found = false;

            // Ensure that both the source and destination nodes exist
            if (container.find(src) == container.end())
                throw std::runtime_error("First node doesn't exist");
            if (container.find(dest) == container.end())
                throw std::runtime_error("Second node doesn't exist");

            // Ensure that both nodes aren't the same
            if (src == dest) {
                throw std::runtime_error("Can't made edge from node to itself!");
            }



            //If they both do exist, then check if an edge already exists
            if (edges[(container.find(src))->second][(container.find(dest))->second]) {
                edges[(container.find(src))->second][(container.find(dest))->second] = weight;
                //If the graph is directed, automatically add edge from destination to source
                if (!is_directed)
                edges[(container.find(dest))->second][(container.find(src))->second] = weight;
            }
            else 
                throw std::runtime_error("Edge already exists");
        }


        bool hasCycle() const
        {
            std::set<int> visited;
            std::set<int> local;
            for (int i = 0; i < size; ++i)
            {
                if (visited.find(i) == visited.end()) {
                    local.clear();
                    if (__dfs(visited, i, local))
                        return true;
                }
            }


            return false;
        }

        // Algorithm implementations 
        std::vector<int> nodeColoring() const;
        std::vector<int> edgeColoring() const;



        void completeEdges();
        std::vector<std::vector<T>> connectedComponents() const
        {
            std::set<int> visited;
            std::set<T> local;
            std::vector<std::vector<T>> answer;

            for (auto i = container.begin(); i != container.end(); ++i)
            {
                if (visited.find(i->second) == visited.end()) {
                    local.clear();
                    comp_dfs(visited, i->first, local);
                    answer.push_back(std::vector<T>());
                    for (auto j = local.begin(); j != local.end(); ++j)
                        answer[answer.size()-1].push_back(*j);
                }
            }


            return answer;
        }





        std::vector<double> katzCentrality(double alpha, double beta) const;
        std::vector<Edge<T, W>> primMST() const;
        std::vector<Edge<T, W>> kruskalMST() const;

        std::vector<T> iterativeDFS(const T& start) const
        {
        }


        std::vector<T> uniformCostSearch(const T& start, const T& goal) const;
        std::vector<T> aStarSearch(const T& start, const T& goal, std::function<double(T, T)> heuristic) const;
};




PYBIND11_MODULE(module_name, handle)
{
//    handle.doc() = "Docs";
py::class_<Graph<std::string, int>>(handle, "GraphString").def("addNode", &Graph<std::string, int>::addNode).def(py::init<bool>()).def("addEdge", &Graph<std::string, int>::addEdge).def("print", &Graph<std::string, int>::printGraph).def("hasCycle", &Graph<std::string, int>::hasCycle).def("connected", &Graph<std::string, int>::connectedComponents);
    //handle.def("addNode", &Graph<std::string, int>::addNode);
}
