#include "osm_routing.h"

OsmRouting::OsmRouting(const char filename[]){
    XMLDocument doc;
    doc.LoadFile(filename);

    XMLElement* root = doc.FirstChildElement("osm");
    for (XMLElement* node = root->FirstChildElement("node"); node; node = node->NextSiblingElement("node")) {
        // Build a map from node id to graph_node
        graph_node cur_node{node->Attribute("id"), stod(node->Attribute("lat"))*M_PI/180, 
                            stod(node->Attribute("lon"))*M_PI/180};

        node_map[node->Attribute("id")] = cur_node;
    }


    for (XMLElement* node = root->FirstChildElement("way"); node!=nullptr; node = node->NextSiblingElement("way"))
    {
        // Build adjacency list ways with the "highway" attribute
        if (is_road(node))
        {
            XMLElement* child = node->FirstChildElement("nd"), * next_child = node->FirstChildElement("nd")->NextSiblingElement("nd");

            while (next_child!=nullptr)
            {
                double dist = compute_dist(node_map[child->Attribute("ref")],
                                        node_map[next_child->Attribute("ref")]);

                adjacency_list[child->Attribute("ref")].push_back(make_pair(dist, next_child->Attribute("ref")));

                // If not oneway, add edge in reverse direction too!
                if (!is_oneway(node)){
                    adjacency_list[next_child->Attribute("ref")].push_back(make_pair(dist, child->Attribute("ref")));
                }
                child = next_child;
                next_child = next_child->NextSiblingElement("nd");
            }
        }
    }
    
}


double OsmRouting::compute_dist(graph_node n1, graph_node n2)
{
    double dlon = n2.lon - n1.lon, dlat = n2.lat - n1.lat;
    double a = pow(sin(dlat/2),2) + cos(n1.lat) * cos(n2.lat) * pow(sin(dlon/2),2);
    double c = 2 * asin(sqrt(a));
    double r = RADIUS; // Radius of earth in kilometers

    return c*r;
}


bool OsmRouting::is_road( XMLElement* way)
{
    for ( XMLElement* node = way->FirstChildElement("tag"); node!=nullptr; node = node->NextSiblingElement("tag")) 
    {
        const string hw = node->Attribute("k");
       if (hw=="highway") return true;
    }
    return false;
}

bool OsmRouting::is_oneway( XMLElement* way){
    for (XMLElement* node = way->FirstChildElement("tag"); node!=nullptr; node = node->NextSiblingElement("tag")) 
    {   
        string ow = node->Attribute("k");
        if (ow=="oneway"){
            ow = node->Attribute("v");
            if (ow=="yes") return true;
        }
    }
    return false;
}


vector<graph_node> OsmRouting::route(const graph_node& start, const graph_node& end)
{   
    vector<graph_node> path;

    if (node_map.find(start.id)==node_map.end()){
        graph_node no_start{"start not found"};
        path.push_back(no_start);
        return path;
    }
    else if ( node_map.find(end.id)==node_map.end() ){
        graph_node no_end{"end not found"};
        path.push_back(no_end);

        return path;
    }

    // Do A* to get shortest path
    priority_queue<edge, vector<edge>, greater<edge>> pq;

    unordered_map<string, double> dists; 

    unordered_map<string, string> prev;
    

    dists[start.id]= 0.0 + compute_dist(start,end);
    pq.push(make_pair(dists[start.id], start.id));

    while (!pq.empty()){
        graph_node node = node_map[pq.top().second];
        pq.pop();
        if (node.id!=start.id && dists.find(node.id)==dists.end()) continue;

        for (auto& it : adjacency_list[node.id])
        {
            double dist_heur = dists[node.id]+it.first+compute_dist(node_map[it.second], end);
            if (dists.find(it.second)==dists.end() || dists[it.second] > dist_heur){
                pq.push(make_pair(dist_heur, it.second));
                dists[it.second]=dist_heur;
                prev[it.second]=node.id;
            }        
        }
    }


    if (dists.find(end.id)!=dists.end()){     
        graph_node cur = end;

        while (cur.id!= start.id){
            path.push_back(cur);
            cur = node_map[prev[cur.id]];
        }
        path.push_back(start);

        reverse(path.begin(), path.end());

        save_GeoJSON(path);
    }
    else{
        graph_node no_path{"no path"};
        path.push_back(no_path);
    }
    
    return path;

}

json OsmRouting::create_edge_feature(const vector<vector<double>>& points, const string& name) {
    return {
        {"type", "Feature"},
        {"geometry", {
            {"type", "LineString"},
            {"coordinates", points}
        }},
        {"properties", {
            {"name", name}
        }}
    };
}

void OsmRouting::save_GeoJSON(const vector<graph_node>& route){
    string base_edge = "edge";

    // Create a FeatureCollection
    json featureCollection = {
        {"type", "FeatureCollection"},
        {"features", json::array()} // Initialize as an empty array
    };
    
    for (int i = 0; i<route.size()-1; i++){
        vector<vector<double>> edge;
        edge.push_back({route[i].lon*180/M_PI, route[i].lat*180/M_PI});
        edge.push_back({route[i+1].lon*180/M_PI, route[i+1].lat*180/M_PI});

        string edge_name = base_edge + to_string(i);

        featureCollection["features"].push_back(create_edge_feature(edge, edge_name));

    }

    ofstream file("shortest_path.geojson");
    file << featureCollection.dump(4); // pretty printing
    file.close();

    cout << "GeoJSON file created" << endl;
}


