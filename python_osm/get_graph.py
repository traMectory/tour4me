
from typing import final
import osmnx as ox
import networkx as nx
from networkx.classes.function import path_weight
from geopy import distance

# gravel_grade = 0.1
# goal_distance = 50*1000
# desire_tags = {'unclassified'}
# desire_grade = 0.1
# avoid_tags = {'residential', 'secondary', 'tertiary', 'primary', 'motorway'}
# surface_tags = {'unclassified', 'compacted_gravel', 'compacted'}
# avoid_grade = 2
# cross_penalty = 1000

# useful_tags_way = ['oneway', 'lanes', 'name', 'highway', 'maxspeed', 'width', 'est_width','surface']
# ox.config(use_cache=True, log_console=True, useful_tags_way=useful_tags_way)

G = ox.graph_from_bbox(51.50, 51.478274, 7.437570, 7.389679, network_type='bike')

# remove = [node for node,degree in dict(G.degree()).items() if degree < 2]
# while len(remove):
#     G.remove_nodes_from(remove)
#     remove = [node for node,degree in dict(G.degree()).items() if degree < 2]

# node_str = 'std::vector<Node> nodes;\n\n'
# for node in G.nodes:
#     lat = G.nodes[node]['y']
#     lon = G.nodes[node]['x']
#     node_str += f"    nodes.push_back(Node({node}, {lat}, {lon}));"

# node_str += f"\n    Graph graph = Graph(nodes);\n\n"

# for edge in G.edges:
#     s, t, _ = edge
#     data = G.get_edge_data(s, t)[0]
#     length = data['length']
#     if not data['oneway']:
#         node_str += f"    graph.addEdge({s}, {t}, {length});"



# text_file = open("sample.txt", "w")
# n = text_file.write(node_str)
# text_file.close()

path = [31423394, 276028318, 828727311, 7160404920, 276028315, 76846032, 332545268, 7160404917, 676833831, 276028296, 276028322, 4497539607, 261071407, 261071386, 6537160170, 261071330, 6537143974, 261071340, 6537143974, 6537160170, 261071386, 495235375, 261071386, 6537160170, 261071330, 277899284, 277899288, 277899284, 244618690, 1144483342, 5730442373, 3975767536, 4497539607, 261071407, 261071386, 6537160170, 6537143974, 261071340, 6537143974, 6537160170, 261071386, 261071407, 4497539607, 276028322, 276028329, 276028322, 276028296, 276028299, 675964675, 675964649, 9195988343, 276028301, 276028304, 6217065980, 6217065981, 6217065982, 6217065981, 6217065983, 6217065981, 6217065982, 6217065981, 6217065980, 276028304, 276028301, 9195988343, 9195988339, 9195988346, 9195988353, 9195988346, 9195988338, 9195988339, 9195988343, 675964649, 675964411, 675964415, 675964411, 8907768268, 675964411, 675964415, 675964411, 675964649, 675964675, 675964414, 675964675, 276028299, 276028296, 676833831, 7160404916, 925538424, 796154255, 332545269, 774080874, 774080831, 774080874, 332545268, 76846032, 276028315, 130117455, 25528691, 7160404922, 31423394, ]


fig, ax = ox.plot_graph_route(G, path, route_linewidth=1, node_size=0, bgcolor='k')



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

# s = ox.get_nearest_node(G, (51.481191, 7.431165))
# print(s)


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