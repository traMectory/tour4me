import osmnx as ox
import math

import sys

from osmnx import utils_geo, projection, downloader, utils

backbone_refs = set()

#radius of the earth
R = 6371e3

target_distance = 100e3

# Eindhoven
# center_lat = 51.461343 
# center_lon = 5.474989

# Home
center_lat = 51.481190
center_lon = 7.431180

file_name = "100kHause"

def getBackbone(north, south, east, west):
    polygon = utils_geo.bbox_to_poly(north, south, east, west)

    
    buffer_dist = 500
    poly_proj, crs_utm = projection.project_geometry(polygon)
    poly_proj_buff = poly_proj.buffer(buffer_dist)
    poly_buff, _ = projection.project_geometry(poly_proj_buff, crs=crs_utm, to_latlong=True)

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
        query_str = f"{overpass_settings};(relation[\"route\"=\"bicycle\"](poly:'{polygon_coord_str}'););out body;>;out skel qt;"
        # query_str = f"{overpass_settings};(relation{osm_filter}(poly:'{polygon_coord_str}');>;);out;"
        response_json = downloader.overpass_request(data={"data": query_str})
        response_jsons.append(response_json)
    utils.log(
        f"Got all network data within polygon from API in {len(polygon_coord_strs)} request(s)"
    )

    for response_json in response_jsons:
        for val in response_json['elements']:
            if val['type'] == 'relation':
                if 'tags' in val:
                    if 'route' in val['tags']:
                        if val['tags']['route'] == "bicycle": 
                            for member in val['members']:   
                                backbone_refs.add(member['ref'])

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

useful_tags_way = ['oneway', 'lanes', 'name', 'highway', 'maxspeed', 'width', 'est_width','surface']
ox.config(use_cache=True, log_console=True, useful_tags_way=useful_tags_way)

process_tags = ['cycleway', 'paved', 'cobblestone', 'gravel', 'unpaved', 'compacted', 'track', 'fine_gravel', 'rock', 'pebblestone', 'unclassified', 'resedential', 'path', 'track', 'secondary']



if len(sys.argv) == 4 + 1:
    target_distance = float(sys.argv[1])
    center_lat = float(sys.argv[2])
    center_lon = float(sys.argv[3])
    file_name = sys.argv[4]

print(f"{target_distance = }, {center_lat = }, {center_lon = }, {file_name = }")

center_lat *= math.pi / 180
center_lon *= math.pi / 180

max_lat = math.asin( math.sin(center_lat)*math.cos(target_distance/(4*R)) +      math.cos(center_lat)*math.sin(target_distance/(4*R)) )
min_lat = math.asin( math.sin(center_lat)*math.cos(target_distance/(4*R)) + -1 * math.cos(center_lat)*math.sin(target_distance/(4*R)) )

max_lon = center_lon + math.atan2(      math.sin(target_distance/(4*R))*math.cos(center_lat), math.cos(target_distance/(4*R))-math.sin(center_lat)*math.sin(center_lat) )
min_lon = center_lon + math.atan2( -1 * math.sin(target_distance/(4*R))*math.cos(center_lat), math.cos(target_distance/(4*R))-math.sin(center_lat)*math.sin(center_lat) )

max_lat *= 180 / math.pi
min_lat *= 180 / math.pi

max_lon *= 180 / math.pi
min_lon *= 180 / math.pi

print(min_lat, max_lat, min_lon, max_lon)

center_lat *= 180 / math.pi
center_lon *= 180 / math.pi

getBackbone(max_lat, min_lat, max_lon, min_lon)



G = ox.graph_from_bbox(max_lat, min_lat, max_lon, min_lon, network_type='bike')
s = ox.distance.nearest_nodes(G, center_lon, center_lat)

# ox.plot_graph_route(G, [s], route_linewidth=1, node_size=0)

node_str = f"c {target_distance = }, {center_lat = }, {center_lon = }, {file_name = }\n"
node_str += f"c g $max_lat $min_lat $max_lon $min_lon $center_lat $center_lon\n"
node_str += f"c p #nodes #edges\n"
node_str += f"c n $id $lat $lon\n"
node_str += f"c e $v_id $w_id $e_cost\n"
node_str += f"c f intermediate nodes of edge\n"
node_str += f"c g tags of edge\n"
node_str += f"g {max_lat} {min_lat} {max_lon} {min_lon} {center_lat} {center_lon}\n"
# node_str += f"p {len(G.nodes)} {sum(1 if not G.get_edge_data(s, t)[0]['oneway'] else 0 for s, t, _ in G.edges)}\n"
node_str += f"p {len(G.nodes)} {len(G.edges)}\n"


for node in G.nodes:
    lat = G.nodes[node]['y']
    lon = G.nodes[node]['x']
    node_str += f"n {node} {lat} {lon}\n"

paths = []

for edge in G.edges:
    s, t, _ = edge
    data = G.get_edge_data(s, t)[0]
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

    if isinstance(ref, list):
        for re in ref:
            if re and re in backbone_refs:
                node_str += " backbone"
                break
    else:
        if ref and ref in backbone_refs:
            node_str += " backbone"

    node_str += "\n"

# ox.plot_graph_routes(G, paths, route_linewidth=1, node_size=0, bgcolor='k')

# raise ValueError('Stop')

with open(f"/home/hagedoorn/Documents/TUD/Code/AOPcpp/input/{file_name}.txt", "w") as text_file:
    text_file.write(node_str)



# text_file = open("sample.txt", "w")
# n = text_file.write(node_str)
# text_file.close()

# path = [160374, 286338878, 670097774, 677231865, 677231868, 1837854880, 247655796, 1992319096, 1992319109, 1992319115, 1992319111, 1992319104, 1992319106, 1992319107, 1992319123, 2187277329, 243882204, 1134230127, 243090198, 243090197, 6513896073, 243090861, 246584324, 249398913, 1448929542, 249398800, 243092689, 9208316471, 275351444, 249360297, 249360419, 692238646, 249360302, 692238637, 692238598, 336361993, 275351445, 275672220, 3751855833, 3751855827, 1491580834, 275351455, 278836066, 4172705007, 164513, 316774738, 5617111352, 749921936, 69329258, 5616982997, 69329262, 277899267, 313478088, 4036708666, 277899262, 275362133, 277899275, 277899276, 277899280, 4036708659, 4036676133, 4036676128, 25528706, 9546203664, 3074381830, 3074408190, 76845983, 1291958717, 76845963, 360638222, 69329163, 69329067, 69329055, 276044225, 7392085123, 360732103, 360645773, 255161605, 32558270, 311584339, 249396642, 255159153, 249539594, 849108441, 849108265, 849108399, 849108356, 1647851185, 7035958879, 1647851187, 2472460034, 1647851194, 6600551811, 3393860067, 2482077606, 1647851180, 1647851171, 6600659507, 2023126445, 254461745, 2023126444, 6600659472, 6600659511, 268456660, 6600659457, 6828185590, 7939095781, 6600659458, 243091055, 6600659496, 6600659510, 269614115, 6600659459, 269605393, 286531803, 269605392, 268450559, 2147783251, 7992330475, 2147783264, 7992330474, 7992330473, 7617693577, 6600659474, 677231700, 52919181, 677231627, 6600659487, 6600659488, 6600659490, 677231971, 247655824, 677231968, 677231906, 677231904, 130117508, 130117546, 677231298, 677231291, 677231274, 677231273, 677230898, 52919183, 677230875, 160374, ]


# fig, ax = ox.plot_graph_route(G, path, route_linewidth=1, node_size=0, bgcolor='k')



# G = ox.graph_from_bbox(51.484, 51.373, 7.537, 7.345, network_type='bike')
# G_modified = G.copy()


# for edge in G_modified.edges:
#     v, w, _ = (edge)
#     data = G_modified.get_edge_data(v, w)[0]
#     if 'surface' in data:
#         # print()
#         # print(data)
#         try:
#             if data['surface'] in surface_tags:
            
#                 data['length'] *= gravel_grade
#         except:
#             pass
#     if 'highway' in data:
#         try:
#             if data['highway'] in desire_tags:
#                 data['length'] *= desire_grade
#         except:
#             pass
#         try:
#             if data['highway'] in avoid_tags:
#                 data['highway'] *= avoid_grade
#         except:
#             pass
#         # print(G_modified.get_edge_data(v, w)[0])
#         # print(G.get_edge_data(v, w)[0])




# #Find best second node
# paths = nx.shortest_path(G_modified, s, weight='length')

# best_path = None
# best_gravel_percentage = -1
# for i, node in enumerate(G.nodes):
#     print(f'{i}/{len(G.nodes)}',end='\r')
#     source = (G.nodes[s]['y'], G.nodes[s]['x'])

#     target = (G.nodes[node]['y'], G.nodes[node]['x'])
    
#     if goal_distance/(10*1000) < distance.distance(source, target).km < 2*goal_distance/(5*1000):
#         if node not in paths:
#             continue
#         path = paths[node]
#         gravel = gravel_length(G, path, surface_tags)

#         percentage = gravel / path_weight(G, path, weight='length')
#         if percentage > best_gravel_percentage:
#             best_gravel_percentage = percentage
#             best_path = path

# print(best_gravel_percentage)
# length_best_path = path_weight(G, best_path, weight='length')

# #best second node


# for edge in G_modified.edges:
#     v, w, _ = (edge)
#     if v in best_path or w in best_path: 
#         data = G_modified.get_edge_data(v, w)[0]
#         data['length'] += cross_penalty

# second_paths = nx.shortest_path(G_modified, best_path[-1], weight='length')

# closest_to_target_distance = 1000000000
# best_distance = 0
# best_second_node = None
# for i, node in enumerate(G.nodes):

#     if node not in paths or node not in second_paths:
#         continue

#     distance_path = length_best_path + path_weight(G, second_paths[node], weight='length') + path_weight(G, paths[node], weight='length')
#     if abs(goal_distance - distance_path) < closest_to_target_distance:
#         closest_to_target_distance = abs(goal_distance - distance_path)
#         best_second_node = node
#         best_distance = distance_path
    

#     print(f'{i}/{len(G.nodes)}, {abs(goal_distance - distance_path)}                            ',end='\r')

# third_path = paths[best_second_node].copy()
# third_path.reverse()

# final_path = best_path + second_paths[best_second_node] + third_path
# print('\n')
# print(best_distance)

# points = []
# for node in final_path:
    
#     lat = G.nodes[node]['y']
#     lon = G.nodes[node]['x']
#     points.append((lat, lon))

# generate_gpx(points)

# fig, ax = ox.plot_graph_route(G, final_path, route_linewidth=10, node_size=0, bgcolor='k')