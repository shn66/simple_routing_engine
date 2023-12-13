
#pragma once
#include <unordered_map>
#include "./includes/tinyxml2.h"
#include "./includes/nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <cmath>
#include<utility>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>

#define RADIUS 6371.0

using namespace std;
using namespace tinyxml2;
using json = nlohmann::json;

typedef pair<double, string> edge;

struct graph_node
{
    string id;
    double lat, lon;
};

class OsmRouting{
    public:
    unordered_map<string, graph_node> node_map;
    OsmRouting(const char filename[]);
    
    vector<graph_node> route(const graph_node& start, const graph_node& end);

    void save_GeoJSON( const vector<graph_node> & route);


    private:  
    unordered_map<string, vector<edge>> adjacency_list;
    
    json create_edge_feature( const vector<vector<double>>& points, const string& name);

    double compute_dist(graph_node n1, graph_node n2);
    bool is_road( XMLElement* way);
    bool is_oneway( XMLElement* way);
};


