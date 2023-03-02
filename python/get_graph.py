import osmnx as ox
import networkx as nx
from os.path import exists
import logging

import warnings
warnings.filterwarnings("ignore")

import sys

from osmnx import utils_geo, projection, downloader, utils, truncate, simplification, settings, graph

logging.basicConfig(filename="log.txt",
                    filemode='a',
                    format='%(asctime)s : %(message)s',
                    datefmt='%Y-%m-%d, %H:%M:%S',
                    level=logging.INFO)


useful_tags_way = ['oneway', 'lanes', 'name', 'highway',
                   'maxspeed', 'width', 'est_width', 'surface']
ox.config(use_cache=True, log_console=True, useful_tags_way=useful_tags_way)


# left of germany 5.746989 -> 5.5
# right of germany  15.122305 -> 15.25
# number of blocks of 0.75 = 13

# top of germany 54.888384 -> 55
# bottom of germany 47.185839 -> 47
# number of blocks of 0.5 -> 16

abs_min_lat = 47
abs_max_lat = 55
lat_gran = 0.5/2
lat_pad = 0.5/4

abs_min_lon = 5.5
abs_max_lon = 15.25
lon_gran = 0.75/2
lon_pad = 0.75/4

abs_min_lat = 51.3648
abs_max_lat = 51.37
lat_gran = 0.5/4
lat_pad = 0.5/6

abs_min_lon = 7.218
abs_max_lon = 7.3
lon_gran = 0.75/4
lon_pad = 0.75/6

# abs_min_lat = 47.45
# abs_max_lat = 47.5
# lat_gran = 0.5/2
# lat_pad = 0.5/4

# abs_min_lon = -122.4
# abs_max_lon = -122.3
# lon_gran = 0.75/2
# lon_pad = 0.75/4

# total blocks = 104

# radius of the earth
# R = 6371e3

# target_distance = 100e3

# Eindhoven
# center_lat = 51.461343
# center_lon = 5.474989

# Home
# center_lat = 51.481190
# center_lon = 7.431180


# file_name = "100kHause"


def getBackbone(north, south, east, west, outputName):
    polygon = utils_geo.bbox_to_poly(north, south, east, west)

    buffer_dist = 500
    poly_proj, crs_utm = projection.project_geometry(polygon)
    poly_proj_buff = poly_proj.buffer(buffer_dist)
    poly_buff, _ = projection.project_geometry(
        poly_proj_buff, crs=crs_utm, to_latlong=True)

    # download the network data from OSM within buffered polygon

    response_jsons = []

    # create overpass settings string
    overpass_settings = downloader._make_overpass_settings()

    # subdivide query polygon to get list of sub-divided polygon coord strings
    polygon_coord_strs = downloader._make_overpass_polygon_coord_strs(polygon)

    # osm_filter = downloader._get_osm_filter("bike")

    # pass each polygon exterior coordinates in the list to the API, one at a
    # time. The '>' makes it recurse so we get ways and the ways' nodes.
    for polygon_coord_str in polygon_coord_strs:
        query_str = f"{overpass_settings};(relation[\"route\"~\"^(bicycle|hiking|mtb)$\"](poly:'{polygon_coord_str}');>;);out;"
        # query_str = f"{overpass_settings};(relation{osm_filter}(poly:'{polygon_coord_str}');>;);out;"
        response_json = downloader.overpass_request(data={"data": query_str})
        response_jsons.append(response_json)
    utils.log(
        f"Got all network data within polygon from API in {len(polygon_coord_strs)} request(s)"
    )

    bidirectional = "bike" in settings.bidirectional_network_types
    G = graph._create_graph(
        response_jsons, retain_all=True, bidirectional=bidirectional)

    # truncate the graph to the extent of the polygon
    G = truncate.truncate_graph_polygon(G, polygon, False, False)

    # simplify the graph topology after truncation. don't truncate after
    # simplifying or you may have simplified out to an endpoint beyond the

    G = simplification.simplify_graph(G)

    outputGraph(G, outputName)

    # for response_json in response_jsons:
    #     for val in response_json['elements']:
    #         if val['type'] == 'relation':
    #             if 'tags' in val:
    #                 if 'route' in val['tags']:
    #                     if val['tags']['route'] == "bicycle":
    #                         for member in val['members']:
    #                             backbone_refs.add(member['ref'])


def hasTag(data, tag):
    for key, value in data.items():
        # if key == "surface":
        #     print(key, value)
        if key == tag:
            return True
        if isinstance(value, list):
            for s in value:
                if s == tag:
                    return True
        if isinstance(value, dict):
            if hasTag(value, tag):
                return True
        if isinstance(value, str):
            if value == tag:
                return True
    return False

def outputGraph(graph, output_name):
    node_str = f"c g $max_lat $min_lat $max_lon $min_lon $center_lat $center_lon\n"
    node_str += f"c p #nodes #edges\n"
    node_str += f"c n $id $lat $lon\n"
    node_str += f"c e $v_id $w_id $e_cost\n"
    node_str += f"c f intermediate nodes of edge\n"
    node_str += f"c g tags of edge\n"
    node_str += f"g {max_lat} {min_lat} {max_lon} {min_lon} {(max_lat - min_lat) / 2} {(max_lon - min_lon) / 2}\n"
    # node_str += f"p {len(graph.nodes)} {sum(1 if not graph.get_edge_data(s, t)[0]['oneway'] else 0 for s, t, _ in graph.edges)}\n"
    node_str += f"p {len(graph.nodes)} {len(graph.edges)}\n"

    for node in graph.nodes:
        lat = graph.nodes[node]['y']
        lon = graph.nodes[node]['x']
        node_str += f"n {node} {lat} {lon}\n"

    paths = []

    # addedEdges = set()

    for edge in graph.edges:
        s, t, _ = edge

        if t <= s:
            continue
        #     if (s, t) in addedEdges:
        #         continue
        #     else:
        #         addedEdges.add((s, t))
        # else:
        #     if (t, s) in addedEdges:
        #         continue
        #     else:
        #         addedEdges.add((t, s))

        data = graph.get_edge_data(s, t)[0]
        # print(data)
        ref = data['osmid']
        # print(ref)
        length = data['length']
        # if not data['oneway']:
        node_str += f"e {s} {t} {length}\n"

        node_str += "f"
        if 'geometry' in data:
            if (s > t):
                for lat, lon in reversed(data['geometry'].coords):
                    node_str += f" {lat} {lon}"
            else:
                for lat, lon in data['geometry'].coords[:-1]:
                    node_str += f" {lat} {lon}"
        node_str += "\n"

        node_str += "g"
        for tag in process_tags:
            if hasTag(data, tag):
                node_str += f" {tag}"

        node_str += "\n"

    # ox.plot_graph_routes(G, paths, route_linewidth=1, node_size=0, bgcolor='k')

    # raise ValueError('Stop')

    with open(f"{output_name}.txt", "w") as text_file:
        text_file.write(node_str)


process_tags = ['cycleway', 'paved', 'cobblestone', 'gravel', 'unpaved', 'compacted', 'track', 'ground'
                'fine_gravel', 'rock', 'pebblestone', 'unclassified', 'resedential', 'path', 'track', 'secondary', 'footway']

count = 0


lat = abs_min_lat
while lat < abs_max_lat:
    lon = abs_min_lon
    while lon < abs_max_lon:
        print('hoi')

        min_lat = lat - lat_pad
        max_lat = lat + lat_gran + lat_pad
        min_lon = lon - lon_pad
        max_lon = lon + lon_gran + lon_pad


        file_name = f"grid-{ '%.4f' % max_lat }-{ '%.4f' % min_lat }-{ '%.4f' % max_lon }-{ '%.4f' % min_lon}"
        print(file_name)

        if (exists(file_name + ".txt")):
            print(f"Gridcell calculated {count}")
            logging.info(f"{count} - Grid already calculated for: {max_lat} {min_lat} {max_lon} {min_lon}")
            continue

        print(f"Calculating gridcell {count}")

        getBackbone(max_lat, min_lat, max_lon, min_lon, file_name + "_B")
        logging.info(f"{count} - Calculated backbone for {max_lat} {min_lat} {max_lon} {min_lon}")

        G = ox.graph_from_bbox(max_lat, min_lat, max_lon, min_lon)
        outputGraph(G, file_name)
        logging.info(f"{count} - Calculated main graph (n={len(G.nodes)}, m={len(G.edges)}) for {max_lat} {min_lat} {max_lon} {min_lon}")

        
        lon += lon_gran
        count += 1
    lat += lat_gran
