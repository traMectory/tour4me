#include "main.h"
#include <random>
#include <httpserver.hpp>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <fstream>

using namespace httpserver;

class calculate_tour : public http_resource {
public:
    const std::shared_ptr<http_response> render_GET(const http_request& req) {
        double lat;
        double lon;
        double distance;
        int algorithm;

        problem->pref_tags.clear();

        try
        {
            lat = std::stod(req.get_arg("lat"));
            lon = std::stod(req.get_arg("lon"));
            distance = std::stod(req.get_arg("dis"));
            algorithm = std::stoi(req.get_arg("algo"));
            printf("Got request: lat %f, lon %f, dis %f\n", lat, lon, distance);

            for (std::string tag : all_tags) {
                if (req.get_arg(tag)[0] == 'd') {
                    problem->pref_tags.insert(tag);
                    std::cout << req.get_arg(tag) << " - " << tag << std::endl;
                }
                // std::cout << req.get_arg(tag) << " - " << tag << std::endl;
            }
        }
        catch(const std::exception& e)
        {      
            return std::shared_ptr<string_response>(new string_response("Bad input", 400, "text/plain"));
        }
        

        double best_distance = 1000000000000;
        Node start;


        for (Node v : problem->graph.v_nodes) {
            double dis = v.distance(lat, lon);
            // printf("s: [%f, %f], v: [%f, %f], dis: [%f]", 51.4894, 7.40577);
            if (dis < best_distance) {


                best_distance = dis;
                start = v;
            }
        }

        problem->start = start.id;
        problem->graph.center_lon = start.lon;
        problem->graph.center_lat = start.lat;

        problem->path.clear();
        problem->quality = -1;
        problem->calculateProfit(&problem->graph);



        printf("%f, %f\n", start.lat, start.lon);

        // problem->pref_tags.insert("gravel");

        problem->target_distance = distance;

        SolveStatus status;

        switch(algorithm)
        {
            case 0:
            {
                Jogger solver;
                status = solver.solve(problem);
                break;
            }
            case 1:
            {
                ILS solver;
                status = solver.solve(problem);
                break;
            }
            case 2:
            {
                ILP solver;
                status = solver.solve(problem);
                break;
            }
        }

        switch(status)
        {
            case SolveStatus::Optimal: 
                printf("Optimal solution found!\n"); 
                return std::shared_ptr<string_response>(new string_response(problem->outputToString(), 200, "application/json"));
                if(gpx) {problem->outputToGPX(filename);} 
                break;
            case SolveStatus::Feasible: 
                printf("Feasible solution found\n"); 
                return std::shared_ptr<string_response>(new string_response(problem->outputToString(), 200, "application/json"));
                if(gpx) {problem->outputToGPX(filename);} 
                break;
            case SolveStatus::Unsolved: 
                printf("Problem was unsolved\n"); 
                return std::shared_ptr<string_response>(new string_response("Not solved", 400, "text/plain"));
                break;
        }
        return std::shared_ptr<string_response>(new string_response("Not solved", 400, "text/plain"));

    }
};

class graph_data : public http_resource {
    const std::shared_ptr<http_response> render_GET(const http_request& req) {
        std::string response = "{\n";
        response += "    \"max_lat\": " + std::to_string(problem->graph.max_lat) + ", \n";
        response += "    \"min_lat\": " + std::to_string(problem->graph.min_lat) + ", \n";
        response += "    \"max_lon\": " + std::to_string(problem->graph.max_lon) + ", \n";
        response += "    \"min_lon\": " + std::to_string(problem->graph.min_lon) + ", \n";
        response += "    \"center_lat\": " + std::to_string(problem->graph.center_lat) + ", \n";
        response += "    \"center_lon\": " + std::to_string(problem->graph.center_lon) + ", \n";
        response += "    \"tags\": [\n";
        for (int i = 0; i < all_tags.size() - 1; i++)
            response += "        \"" + all_tags[i] + "\",\n";
        response += "        \"" + all_tags[all_tags.size() - 1] + "\"\n    ],\n";
        response += "    \"algorithms\": [\n";
        for (int i = 0; i < algorithms.size() - 1; i++)
            response += "        \"" + algorithms[i] + "\",\n";
        response += "        \"" + algorithms[algorithms.size() - 1] + "\"\n    ]\n";
        response += "}";

        return std::shared_ptr<string_response>(new string_response(response, 200, "application/json"));
    }
};

class backbone_data : public http_resource {
    const std::shared_ptr<http_response> render_GET(const http_request& req) {

        Node start;
        double best_distance = 1000000000000;
        double lat = std::stod(req.get_arg("lat"));
        double lon = std::stod(req.get_arg("lon"));

        for (Node v : problem->backbone.v_nodes) {
            double dis = v.distance(lat, lon);
            // printf("s: [%f, %f], v: [%f, %f], dis: [%f]", 51.4894, 7.40577);
            if (dis < best_distance) {


                best_distance = dis;
                start = v;
            }
        }

        problem->start = start.id;

        Graph* rG = reduceGraph(&problem->backbone, problem->start, 200);
        // rG = simplifyGraph(rG, problem->start);

        std::string response = "{\n";
        response += "  \"backbone\": \n    [\n";

        for (Edge* edge : rG->v_edges) {

            response += "      {\n        \"line\": [ [" + std::to_string(edge->s.lat) + "," + std::to_string(edge->s.lon) + "],";
            // for (int j = 0; j < edge->geo_locs.size(); j++) {
            //     // auto pair = edge->geo_locs[i.g_id < path[i+1].g_id ? j : edge->geo_locs.size() - j - 1];
            //     response += "        [" + std::to_string(edge->geo_locs[j].first) + "," + std::to_string(edge->geo_locs[j].second) + "], \n";
            // }
            response += " [" + std::to_string(edge->t.lat) + "," + std::to_string(edge->t.lon) + "] ],\n";
            response += "        \"tags\": [";
            for (std::string tag : edge->tags) {
                response += "\"" + tag + "\", ";
            }
            if (edge->tags.size() > 0) {
                response.pop_back();
                response.pop_back();
            }
            response += "]\n      },\n";
            // response += "        [" + std::to_string(edge->geo_locs[edge->geo_locs.size() - 1].first) + "," + std::to_string(edge->geo_locs[edge->geo_locs.size() - 1].second) + "], \n";
        }

        response.pop_back();
        response.pop_back();

        response += "\n    ]\n";
        response += "}";

        return std::shared_ptr<string_response>(new string_response(response, 200, "application/json"));
    }
};

class index_resource : public http_resource {
public:
    const std::shared_ptr<http_response> render_GET(const http_request& req) {
        std::cout << "Get request\n";
        return std::shared_ptr<file_response>(new file_response("/home/hagedoorn/Documents/TUD/Code/AOPcpp/web/main.html", 200, "text/html"));
    }
};

int main(int argc, char** argv) {
        std::string filename = "50kHause";
        problem = new Problem("/home/hagedoorn/Documents/TUD/Code/AOPcpp/input/"+filename+".txt", "/home/hagedoorn/Documents/TUD/Code/AOPcpp/input/"+filename+"_B.txt");

        // problem->graph = problem->backbone;

        printf("Calculating shortest path pairs\n");
        problem->fillShortestPath(filename);

//         printf("Starting server\n");
    
//         webserver ws = create_webserver(8080);

//         calculate_tour uar;
//         ws.register_resource("/tour", &uar);

//         graph_data gdr;
//         ws.register_resource("/graphdata", &gdr);

//         backbone_data bdr;
//         ws.register_resource("/backbone", &bdr);

//         index_resource hwr;
//         ws.register_resource("/index.html", &hwr);

//         ws.start(true);
        
//         return 0;
}
