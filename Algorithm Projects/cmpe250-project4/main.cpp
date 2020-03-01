#include <vector>
#include <utility>
#include <iostream>
#include <queue>
#include <fstream>

using namespace std;

int grid[1000][1000];
int n_grid, m_grid, query_n, goal, source;
vector< pair< pair<int,int>,pair<int,int> > > queries;


vector<int> modified_dijsktra(int start){
    int N = m_grid*n_grid;
    priority_queue< pair<int,int>, vector< pair<int,int> >, greater < pair<int,int> > > pq;
    vector<bool> visited(N+1, false);
    vector<int> ans(N+1, 1000000001);
    ans[start] = 0;
    pq.push(pair<int,int>(0, start));

    pair <int, int> min_node_pair;
    int diff, i, j, x, y, vertex_id, min_node_id;
    while(!pq.empty()) {
        min_node_pair = pq.top();
        pq.pop();
        min_node_id = min_node_pair.second;
        if (visited[min_node_id])
            continue;
        visited[min_node_id] = true;
        i = ((min_node_id-1)/n_grid) +1;
        j = ((min_node_id - 1) % m_grid) + 1;
        if (j != 1){
            vertex_id = min_node_id-1;
            x = ((vertex_id-1)/n_grid) +1;
            y = ((vertex_id-1)%m_grid)+1;
            diff = abs(grid[i-1][j-1]-grid[x-1][y-1]);
            diff = max(diff, ans[min_node_id]);
            if(ans[vertex_id] > diff){
                ans[vertex_id] = diff;
                pq.push(pair<int,int>(diff,vertex_id));
            }
        }
        if (j != m_grid){
            vertex_id = min_node_id+1;
            x = ((vertex_id-1)/n_grid) +1;
            y = ((vertex_id-1)%m_grid)+1;
            diff = abs(grid[i-1][j-1]-grid[x-1][y-1]);
            diff = max(diff, ans[min_node_id]);
            if(ans[vertex_id] > diff){
                ans[vertex_id] = diff;
                pq.push(pair<int,int>(diff,vertex_id));
            }
        }
        if (i != 1){
            vertex_id = min_node_id-m_grid;
            x = ((vertex_id-1)/n_grid) +1;
            y = ((vertex_id-1)%m_grid)+1;
            diff = abs(grid[i-1][j-1]-grid[x-1][y-1]);
            diff = max(diff, ans[min_node_id]);
            if(ans[vertex_id] > diff){
                ans[vertex_id] = diff;
                pq.push(pair<int,int>(diff,vertex_id));
            }
        }
        if (i != n_grid){
            vertex_id = min_node_id+m_grid;
            x = ((vertex_id-1)/n_grid) +1;
            y = ((vertex_id-1)%m_grid)+1;
            diff = abs(grid[i-1][j-1]-grid[x-1][y-1]);
            diff = max(diff, ans[min_node_id]);
            if(ans[vertex_id] > diff){
                ans[vertex_id] = diff;
                pq.push(pair<int,int>(diff,vertex_id));
            }
        }
    }
    return ans;
}

int main(int argc, char* argv[]) {
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    in>>n_grid>>m_grid;
    for(int i = 0; i < n_grid; i++){
        for(int j = 0; j < m_grid; j++){
            in>>grid[i][j];
        }
    }
    in>>query_n;
    int x1,y1,x2,y2;
    for(int i = 0; i < query_n; i++){
        in>>x1>>y1>>x2>>y2;
        queries.push_back(pair< pair<int,int>,pair<int,int>> (pair<int,int>(x1,y1), pair<int,int>(x2,y2)));
    }
    in.close();
    x1 = queries[0].first.first;
    y1 = queries[0].first.second;
    x2 = queries[0].second.first;
    y2 = queries[0].second.second;
    goal = (x2-1)*m_grid+y2;
    source = (x1-1)*m_grid+y1;
    vector<int> results = modified_dijsktra(source);
    out<<results[goal]<<endl;
    for(int i = 1; i < queries.size(); i++){
        x1 = queries[i].first.first;
        y1 = queries[i].first.second;
        x2 = queries[i].second.first;
        y2 = queries[i].second.second;
        goal = (x2-1)*m_grid+y2;
        source = (x1-1)*m_grid+y1;
        if(results[goal]!= results[source]){
            out<<max(results[goal], results[source])<<endl;
        }else{
            vector<int> results2 = modified_dijsktra(source);
            out<<results2[goal]<<endl;
        }
    }

    out.close();

    return 0;
}
