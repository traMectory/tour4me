#include "main.h"
#include <random>
#include <httpserver.hpp>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <fstream>

using namespace httpserver;

class calculate_tour : public http_resource
{
public:
    const std::shared_ptr<http_response> render_GET(const http_request &req)
    {

        auto init_time_1 = std::chrono::high_resolution_clock::now();

        double lat;
        double lon;
        double distance;
        int algorithm;
        double runningTime;
        double edgeProfitImportance;
        double coveredAreaImportance;

        Problem problem;
        std::string filename;

        lat = std::stod(req.get_arg("lat"));
        lon = std::stod(req.get_arg("lon"));
        distance = std::stod(req.get_arg("dis"));
        algorithm = std::stoi(req.get_arg("algo"));

        runningTime = std::stod(req.get_arg("rt"));
        edgeProfitImportance = std::stod(req.get_arg("ep"));
        coveredAreaImportance = std::stod(req.get_arg("ca"));

        double min_lat = lat - std::fmod((lat - abs_min_lat), lat_gran) - lat_pad;
        double max_lat = min_lat + 2 * lat_pad + lat_gran;
        double min_lon = lon - std::fmod((lon - abs_min_lon), lon_gran) - lon_pad;
        double max_lon = min_lon + 2 * lon_pad + lon_gran;

        std::stringstream stream;
        stream << std::fixed << std::setprecision(4) << "grid-" << max_lat << "-" << min_lat << "-" << max_lon << "-" << min_lon;
        filename = stream.str();

        // std::string filename = "grid-" + std::to_string(max_lat) + "-" + std::to_string(min_lat) + "-" + std::to_string(max_lon) + "-" + std::to_string(min_lon);

        std::cout << filename << "\n";
        problem = Problem("../input/" + filename + ".txt", "../input/" + filename + "_B.txt");
        printf("Got request: lat %f, lon %f, dis %f\n", lat, lon, distance);
        // problem.graph = problem.backbone;
        for (int i = 0; i < all_tags.size(); i++)
        {
            if (req.get_arg("tags")[i] == 'd')
            {
                problem.pref_tags.insert(all_tags[i].attr);
                std::cout << "desired: " << all_tags[i].attr << "\n";
            }
        }

        double best_distance = 1000000000000;
        Node start;

        for (Node v : problem.graph.v_nodes)
        {
            double dis = v.distance(lat, lon);
            // printf("s: [%f, %f], v: [%f, %f], dis: [%f]", 51.4894, 7.40577);
            if (dis < best_distance)
            {

                best_distance = dis;
                start = v;
            }
        }

        problem.start = start.id;
        problem.graph.center_lon = start.lon;
        problem.graph.center_lat = start.lat;

        problem.runningTime = runningTime;
        problem.edgeProfitImportance = edgeProfitImportance;
        problem.coveredAreaImportance = coveredAreaImportance;

        problem.path.clear();
        problem.quality = -1;
        problem.calculateProfit(&problem.graph);

        printf("%f, %f\n", start.lat, start.lon);

        // problem.pref_tags.insert("gravel");

        problem.target_distance = distance;


        auto init_time_2 = std::chrono::high_resolution_clock::now();
        auto algo_time_1 = std::chrono::high_resolution_clock::now();

        SolveStatus status;
        switch (algorithm)
        {
        case 0:
        {
            Jogger solver;
            status = solver.solve(&problem);
            break;
        }
        case 1:
        {
            ILS solver;
            status = solver.solve(&problem);
            break;
        }
        case 2:
        {
            ILP solver;
            status = solver.solve(&problem);
            break;
        }
        }
        auto algo_time_2 = std::chrono::high_resolution_clock::now();

        auto init_time_int = duration_cast<std::chrono::milliseconds>(init_time_2 - init_time_1);
        auto algo_time_int = duration_cast<std::chrono::milliseconds>(algo_time_2 - algo_time_1);

        problem.metadata.push_back("Initialization time (ms): " + std::to_string(init_time_int.count()));
        problem.metadata.push_back("Algorithm computation time (ms): " + std::to_string(algo_time_int.count()));
        problem.metadata.push_back("Profit: " + std::to_string(problem.getProfit(problem.path)) + " (theoretical upper bound: " + std::to_string(problem.target_distance) + ")");
        problem.metadata.push_back("Area: " + std::to_string(problem.getArea(problem.path)) + " (theoretical upper bound: " + std::to_string(M_PI * problem.target_distance*problem.target_distance) + ")");
        // problem.metadata.push_back("Quality: " + std::to_string(problem.getQuality(problem.path)) + " (theoretical upper bound: " + std::to_string(M_PI * problem.target_distance*problem.target_distance) + ")");


        switch (status)
        {
        case SolveStatus::Optimal:
            printf("Optimal solution found!\n");
            return std::shared_ptr<string_response>(new string_response(problem.outputToString(), 200, "application/json"));
            if (gpx)
            {
                problem.outputToGPX(filename);
            }
            break;
        case SolveStatus::Feasible:
            printf("Feasible solution found\n");
            return std::shared_ptr<string_response>(new string_response(problem.outputToString(), 200, "application/json"));
            if (gpx)
            {
                problem.outputToGPX(filename);
            }
            break;
        case SolveStatus::Unsolved:
            printf("Problem was unsolved\n");
            return std::shared_ptr<string_response>(new string_response("Not solved", 400, "text/plain"));
            break;
        }
        return std::shared_ptr<string_response>(new string_response("Not solved", 400, "text/plain"));
    }
};

class graph_data : public http_resource
{
    const std::shared_ptr<http_response> render_GET(const http_request &req)
    {
        double lat = -1;
        double lon = -1;
        try
        {
            lat = std::stod(req.get_arg("lat"));
            lon = std::stod(req.get_arg("lon"));
        }
        catch (const std::exception &e)
        {
            // office location
            lat = 51.489808;
            lon = 7.406319;
        }

        double min_lat = lat - std::fmod((lat - abs_min_lat), lat_gran) - lat_pad;
        double max_lat = min_lat + 2 * lat_pad + lat_gran;
        double min_lon = lon - std::fmod((lon - abs_min_lon), lon_gran) - lon_pad;
        double max_lon = min_lon + 2 * lon_pad + lon_gran;

        std::stringstream stream;

        stream << "{\n";
        stream << "    \"max_lat\": " << max_lat << ", \n";
        stream << "    \"min_lat\": " << min_lat << ", \n";
        stream << "    \"max_lon\": " << max_lon << ", \n";
        stream << "    \"min_lon\": " << min_lon << ", \n";
        stream << "    \"center_lat\": " << lat << ", \n";
        stream << "    \"center_lon\": " << lon << ", \n";
        // stream << "    \"center_lat\": " << problem.graph.center_lat << ", \n";
        // stream << "    \"center_lon\": " << problem.graph.center_lon << ", \n";
        stream << "    \"tags\": [\n";
        for (int i = 0; i < all_tags.size() - 1; i++)
            stream << "        [\"" << all_tags[i].attr << "\", " << all_tags[i].type << "],\n";
        stream << "        [\"" << all_tags[all_tags.size() - 1].attr << "\", " << all_tags[all_tags.size() - 1].type << "]\n    ],\n";
        stream << "    \"algorithms\": [\n";
        for (int i = 0; i < algorithms.size() - 1; i++)
            stream << "        \"" << algorithms[i] << "\",\n";
        stream << "        \"" << algorithms[algorithms.size() - 1] << "\"\n    ]\n";
        stream << "}";

        std::string response = stream.str();

        return std::shared_ptr<string_response>(new string_response(response, 200, "application/json"));
    }
};

class backbone_data : public http_resource
{
    const std::shared_ptr<http_response> render_GET(const http_request &req)
    {

        Node start;
        double best_distance = 1000000000000;
        double lat = std::stod(req.get_arg("lat"));
        double lon = std::stod(req.get_arg("lon"));

        double min_lat = lat - std::fmod((lat - abs_min_lat), lat_gran) - lat_pad;
        double max_lat = min_lat + 2 * lat_pad + lat_gran;
        double min_lon = lon - std::fmod((lon - abs_min_lon), lon_gran) - lon_pad;
        double max_lon = min_lon + 2 * lon_pad + lon_gran;

        std::stringstream stream;
        stream << std::fixed << std::setprecision(4) << "grid-" << max_lat << "-" << min_lat << "-" << max_lon << "-" << min_lon;
        std::string filename = stream.str();

        Problem problem = Problem("../input/" + filename + ".txt", "../input/" + filename + "_B.txt");
        problem.graph = problem.backbone;
        for (Node v : problem.graph.v_nodes)
        {
            double dis = v.distance(lat, lon);
            // printf("s: [%f, %f], v: [%f, %f], dis: [%f]", 51.4894, 7.40577);
            if (dis < best_distance)
            {

                best_distance = dis;
                start = v;
            }
        }

        problem.start = start.id;

        Graph *rG = reduceGraph(&problem.graph, problem.start, 200);
        int prev_n = 1000000000;
        while (rG->v_nodes.size() < prev_n)
        {
            prev_n = rG->v_nodes.size();
            rG = simplifyGraph(rG, problem.start);
        }

        std::string response = "{\n";
        response += "  \"backbone\": \n    [\n";

        for (Edge *edge : rG->v_edges)
        {

            response += "      {\n        \"line\": [ [" + std::to_string(edge->s.lat) + "," + std::to_string(edge->s.lon) + "],";
            // for (int j = 0; j < edge->geo_locs.size(); j++) {
            //     // auto pair = edge->geo_locs[i.g_id < path[i+1].g_id ? j : edge->geo_locs.size() - j - 1];
            //     response += "        [" + std::to_string(edge->geo_locs[j].first) + "," + std::to_string(edge->geo_locs[j].second) + "], \n";
            // }
            response += " [" + std::to_string(edge->t.lat) + "," + std::to_string(edge->t.lon) + "] ],\n";
            response += "        \"tags\": [";
            for (std::string tag : edge->tags)
            {
                response += "\"" + tag + "\", ";
            }
            if (edge->tags.size() > 0)
            {
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

class index_resource : public http_resource
{
public:
    const std::shared_ptr<http_response> render_GET(const http_request &req)
    {
        std::cout << "Get request\n";
        return std::shared_ptr<file_response>(new file_response("../web/main.html", 200, "text/html"));
    }
};

int main(int argc, char **argv)
{
    // problem.graph = problem.backbone;

    printf("Starting server\n");

    webserver ws = create_webserver(8080);

    calculate_tour uar;
    ws.register_resource("/tour", &uar);

    graph_data gdr;
    ws.register_resource("/graphdata", &gdr);

    backbone_data bdr;
    ws.register_resource("/backbone", &bdr);

    index_resource hwr;
    ws.register_resource("/", &hwr);

    ws.start(true);

    return 0;
}
