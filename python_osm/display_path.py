import json
import osmnx as ox
import math

import sys

R = 6371e3

file_name = "10kArbeit"

if len(sys.argv) == 1 + 1:
    file_name = sys.argv[1]

print(f"{file_name = }")

with open(f'output/{ file_name }.json') as json_file:
    data = json.load(json_file)

# target_distance = float(data['target_distance'])
# center_lat = float(data['center_lat'])
# center_lon = float(data['center_lon'])

target_distance = 10e3

center_lat = 51.4895 
center_lon = 7.40577

graph_name = data['graph_name']

print(f"{graph_name = }, {target_distance = }, {center_lat = }, {center_lon = }")

path = data['path']

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

center_lat *= 180 / math.pi
center_lon *= 180 / math.pi

G = ox.graph_from_bbox(max_lat, min_lat, max_lon, min_lon, network_type='bike')
fig, ax = ox.plot_graph_route(G, path, route_linewidth=1, node_size=0, bgcolor='k')