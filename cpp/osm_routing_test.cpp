#include "osm_routing.h"
#include <gtest/gtest.h>



// Demonstrate some basic assertions.
TEST(OsmRouting, CheckAstar) {
  const char * filename = "map.osm";
  OsmRouting test_graph(filename);


  graph_node start = test_graph.node_map["65500635"], end = test_graph.node_map["4177973233"];

  vector<graph_node> shortest_route = test_graph.route(start, end);
  EXPECT_EQ(55, shortest_route.size());


  ifstream file;
  file.open("shortest_path.geojson");
  EXPECT_EQ(1, (bool) file);

  start = test_graph.node_map["65500635"];
  end = test_graph.node_map["1333743889"];
 
  shortest_route = test_graph.route(start, end);
  EXPECT_EQ(0, shortest_route[0].id.compare("no path"s));
}

TEST(OsmRouting, CheckGeoJSON) {
  const char * filename = "map.osm";
  OsmRouting test_graph(filename);


  graph_node start = test_graph.node_map["65500635"], end = test_graph.node_map["4177973233"];

  vector<graph_node> shortest_route = test_graph.route(start, end);
  
  ifstream file;
  file.open("shortest_path.geojson");
  EXPECT_EQ(1, (bool) file);
}


TEST(OsmRouting, CheckNodes) {
  const char * filename = "map.osm";
  OsmRouting test_graph(filename);

  graph_node start{"0"},  end = test_graph.node_map["1333743889"];

  vector<graph_node> shortest_route = test_graph.route(start, end);

  EXPECT_EQ(0, shortest_route[0].id.compare("start not found"s));

  shortest_route = test_graph.route(end, start);
  EXPECT_EQ(0, shortest_route[0].id.compare("end not found"s));
}


// int main() 
// { 
//   const char filename[] = "map.osm";
//   OsmRouting test_graph(filename);

//   // cout<<"Ran";
//   osmTests t;
//   t.run_tests(test_graph);
//   return 0;
// }