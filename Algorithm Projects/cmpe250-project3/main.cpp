#include "graph.h"

int main(int argc, char* argv[]) {
    /*
      5
      2 1 2
      0
      2 2 5
      0
      1 4
    */

    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1]);
    output_file.open(argv[2]);
    int N, keys, key;
    input_file>>N;

    Graph graph(N);

    for(int i = 0 ; i < N ; i++){
        vector<int> adj;
        input_file>>keys;
        for(int j = 0 ; j < keys ; j++){
            input_file>>key;
            adj.push_back(key-1);
        }
        Node node(i, adj);
        graph.add_node(node);
    }

    graph.tarjan();
    vector<int> result;
    for(Cluster c: graph.clusters) {
        if(c.in_degree == 0){
            result.push_back(c.nodes_in_cluster[0]+1);
        }
    }
    output_file<<result.size()<<" ";
    for(int i: result)
        output_file<<i<<" ";
    input_file.close();
    output_file.close();

    return 0;
}