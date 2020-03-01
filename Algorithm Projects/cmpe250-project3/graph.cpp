//
// Created by Alperen Bağ on 17.11.2018.
//

#include "graph.h"

void Cluster::add_node(int node_id) {
    nodes_in_cluster.push_back(node_id);
}

void Graph::add_node(const Node& node) {
    nodes.push_back(node);
}

//Some helpful variables for tarjan algorithm.
int util_index = 0;
int cluster_index = 0;
vector<int> stack;

void Graph::tarjan() {
    for(int i = 0 ; i < N ; i++){
        if(nodes[i].index==-1)
            tarjan_util(nodes[i]);
    }
    set_in_degrees();
}

void Graph::tarjan_util(Node& node) {
    node.index = util_index;
    node.lowlink = util_index;
    util_index++;
    stack.push_back(node.id);
    node.on_stack = true;

    for(int adj_node_index: node.adj){
        if(nodes[adj_node_index].index==-1){
            tarjan_util(nodes[adj_node_index]);
            node.lowlink = min(node.lowlink, nodes[adj_node_index].lowlink);
        }else if(nodes[adj_node_index].on_stack){
            node.lowlink = min(node.lowlink, nodes[adj_node_index].index);
        }
    }

    if(node.index == node.lowlink) {
        Cluster cluster(cluster_index);
        while (stack.back() != node.id) {
            nodes[stack.back()].on_stack = false;
            nodes[stack.back()].cluster_id = cluster_index;
            cluster.add_node(stack.back());
            stack.pop_back();
        }
        nodes[stack.back()].on_stack = false;
        nodes[stack.back()].cluster_id = cluster_index;
        cluster.add_node(stack.back());
        cluster_index++;
        stack.pop_back();
        clusters.push_back(cluster);
    }
}

void Graph::set_in_degrees() {
    for(Cluster cluster : clusters ){
        for(int node_id : cluster.nodes_in_cluster){
            for(int adj_index: nodes[node_id].adj){
                if(nodes[adj_index].cluster_id != nodes[node_id].cluster_id)
                    clusters[nodes[adj_index].cluster_id].in_degree++;
            }
        }
    }


}