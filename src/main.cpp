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

        problem->pref_tags.clear();

        try
        {
            lat = std::stod(req.get_arg("lat"));
            lon = std::stod(req.get_arg("lon"));
            distance = std::stod(req.get_arg("dis"));
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
        problem->center_lon = start.lon;
        problem->center_lat = start.lat;

        problem->path.clear();
        problem->quality = -1;

        problem->calculateProfit();



        printf("%f, %f\n", start.lat, start.lon);

        // problem->pref_tags.insert("gravel");

        problem->target_distance = distance;

        Jogger solver;
        SolveStatus status = solver.solve(problem);

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
        response += "    \"center_lat\": " + std::to_string(problem->center_lat) + ", \n";
        response += "    \"center_lon\": " + std::to_string(problem->center_lon) + ", \n";
        response += "    \"tags\": [\n";
        for (int i = 0; i < all_tags.size() - 1; i++)
            response += "        \"" + all_tags[i] + "\",\n";
        response += "        \"" + all_tags[all_tags.size() - 1] + "\"\n    ]\n";
        response += "}";
        return std::shared_ptr<string_response>(new string_response(response, 200, "application/json"));
    }
};

class file_response_resource : public http_resource {
public:
    const std::shared_ptr<http_response> render_GET(const http_request& req) {
        return std::shared_ptr<file_response>(new file_response("/home/hagedoorn/Documents/TUD/Code/AOPcpp/web/main.html", 200, "text/html"));
    }
};

int main(int argc, char** argv) {
        problem = new Problem("/home/hagedoorn/Documents/TUD/Code/AOPcpp/input/100kNijkerk.txt");
        printf("1\n");
        webserver ws = create_webserver(8080);

        // problem->pref_tags.insert("gravel");
        // problem->pref_tags.insert("unpaved");
        // problem->pref_tags.insert("compacted");
        // problem->pref_tags.insert("track");
        // problem->pref_tags.insert("fine_gravel");
        // problem->pref_tags.insert("rock");
        // problem->pref_tags.insert("pebblestone");

        problem->pref_tags.insert("unclassified");
        problem->pref_tags.insert("cycleway");

        calculate_tour uar;
        ws.register_resource("/tour", &uar);

        graph_data gdr;
        ws.register_resource("/graphdata", &gdr);


        file_response_resource hwr;
        ws.register_resource("/index.html", &hwr);

        ws.start(true);
        
        return 0;
}
