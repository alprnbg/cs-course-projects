//
// Created by Alperen Bağ on 17.11.2018.
//

#ifndef PROJECT3_GRAPH_H
#define PROJECT3_GRAPH_H

#endif //PROJECT3_GRAPH_H

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Cluster{
public:
    Cluster(int _id): id(_id), in_degree(0){};
    int id;
    int in_degree;
    vector<int> nodes_in_cluster;
    void add_node(int node_id);
};

class Node {
public:
    Node(int _id, vector<int> _adj):id(_id), adj(_adj), index(-1), on_stack(false) {};
    int id;
    int cluster_id;
    vector<int> adj;
    int index;
    int lowlink;
    bool on_stack;
};


class Graph {
public:
    Graph(int _N):N(_N){};
    int N;
    vector<Node> nodes;         //first element will be empty for the sake of simplicity of index.
    vector<Cluster> clusters;
    void add_node(const Node& node);
    void tarjan();
    void tarjan_util(Node& node);
    void set_in_degrees();
};