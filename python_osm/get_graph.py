
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

path = [31423394, 276028318, 828727311, 7160404920, 276028315, 76846032, 332545268, 774080874, 332545269, 796154255, 5627363395, 852308958, 3074381830, 9546203664, 25528706, 4036676128, 25528708, 1233658811, 270704016, 3975762804, 78544436, 3235121553, 681226407, 268484524, 684024579, 684024725, 684024579, 249361239, 277891335, 277891352, 277891466, 277891475, 277891466, 277891396, 277891455, 277891457, 683823925, 277891457, 663178514, 277891457, 663178516, 277891457, 663178514, 277891457, 277891455, 277891453, 262153310, 621424040, 304311933, 8829677659, 262153304, 164245, 262153344, 679310577, 683411124, 683411333, 683411355, 767801074, 31671575, 31813668, 31671575, 691385927, 759795057, 691385927, 691386234, 691385927, 759795057, 691385927, 691386234, 691385927, 759795057, 691385927, 31671575, 31813668, 31671575, 691385927, 691386234, 691385927, 31671575, 767801074, 767801100, 767801074, 683411355, 164245, 31671571, 7244123373, 7160911401, 360634286, 3235121772, 683879227, 3235121772, 7160911402, 3235121772, 360634286, 249361227, 164247, 25528712, 7160911398, 3235121536, 25528711, 698846586, 3975762803, 3975762804, 1233658811, 4036676140, 1233658811, 270704016, 1144483335, 1144483342, 5730442373, 3975767536, 925538413, 925538424, 7160404916, 676833831, 276028296, 276028299, 675964675, 675964649, 9195988343, 276028301, 276028304, 6217065980, 6217065981, 6217065983, 6217065981, 6217065982, 6217065981, 6217065983, 6217065981, 6217065982, 6217065981, 6217065980, 31423394, ]


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