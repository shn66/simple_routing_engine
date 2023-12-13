### Routing Engine

# Description
A simple routing engine that can find the shortest path between two points on a map. The map is represented as a graph, where each node is a point on the map and each edge is a road between two points. The length of an edge is the distance between two points. 

`map.osm` is an XML file road network data from Open Street Maps containing nodes and ways in the area. A way is a path between nodes. A way can be a road, a river, a railway, etc. Each way has a list of nodes that it connects. Each node has a latitude and longitude.

The routing engine should does the following:
* Read the map data from 'map.osm' XML file
* Build a graph from the map data
* A function API that accepts start and end points
* Finds the shortest path between the two points on the map using A*

The output of the program is saved as a [geojson](https://en.wikipedia.org/wiki/GeoJSON) file. 


# Build Instructions

1) cd into unzipped folder and build docker container using
    docker build -t osm_routing .

2) Run tests using 
    docker run -it osm_routing