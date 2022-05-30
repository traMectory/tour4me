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
        double lat;
        double lon;
        double distance;
        int algorithm;
        Problem problem;

        try
        {
            lat = std::stod(req.get_arg("lat"));
            lon = std::stod(req.get_arg("lon"));
            distance = std::stod(req.get_arg("dis"));
            algorithm = std::stoi(req.get_arg("algo"));

            double min_lat = lat - std::fmod((lat - abs_min_lat), lat_gran) - lat_pad;
            double max_lat = min_lat + 2 * lat_pad + lat_gran;
            double min_lon = lon - std::fmod((lon - abs_min_lon), lon_gran) - lon_pad;
            double max_lon = min_lon + 2 * lon_pad + lon_gran;


            std::stringstream stream;
            stream << std::fixed << std::setprecision(4) << "grid-" << max_lat << "-" << min_lat << "-" << max_lon << "-" << min_lon;
            std::string filename = stream.str();

            // std::string filename = "grid-" + std::to_string(max_lat) + "-" + std::to_string(min_lat) + "-" + std::to_string(max_lon) + "-" + std::to_string(min_lon);
            
            std::cout << filename << "\n";
            problem = Problem("../input/" + filename + ".txt", "../input/" + filename + "_B.txt");
            printf("Got request: lat %f, lon %f, dis %f\n", lat, lon, distance);

            for (std::string tag : all_tags)
            {
                if (req.get_arg(tag)[0] == 'd')
                {
                    problem.pref_tags.insert(tag);
                    std::cout << req.get_arg(tag) << " - " << tag << std::endl;
                }
                // std::cout << req.get_arg(tag) << " - " << tag << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            return std::shared_ptr<string_response>(new string_response("Bad input", 400, "text/plain"));
        }

        double min_lat = lat - std::fmod((lat - abs_min_lat), lat_gran) - lat_pad;
        double max_lat = min_lat + 2 * lat_pad + lat_gran;
        double min_lon = lon - std::fmod((lon - abs_min_lon), lon_gran) - lon_pad;
        double max_lon = min_lon + 2 * lon_pad + lon_gran;

        // std::string filename = "grid-" + std::to_string(max_lat) + "-" + std::to_string(min_lat) + "-" + std::to_string(max_lon) + "-" + std::to_string(min_lon);


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

        problem.path.clear();
        problem.quality = -1;
        problem.calculateProfit(&problem.graph);

        printf("%f, %f\n", start.lat, start.lon);

        // problem.pref_tags.insert("gravel");

        problem.target_distance = distance;

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
            status = SolveStatus::Unsolved;
            break;
        }
        }

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

        std::string response = "{\n";
        response += "    \"max_lat\": " + std::to_string(max_lat) + ", \n";
        response += "    \"min_lat\": " + std::to_string(min_lat) + ", \n";
        response += "    \"max_lon\": " + std::to_string(max_lon) + ", \n";
        response += "    \"min_lon\": " + std::to_string(min_lon) + ", \n";
        response += "    \"center_lat\": " + std::to_string(lat) + ", \n";
        response += "    \"center_lon\": " + std::to_string(lon) + ", \n";
        // response += "    \"center_lat\": " + std::to_string(problem.graph.center_lat) + ", \n";
        // response += "    \"center_lon\": " + std::to_string(problem.graph.center_lon) + ", \n";
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

// class backbone_data : public http_resource
// {
//     const std::shared_ptr<http_response> render_GET(const http_request &req)
//     {

//         Node start;
//         double best_distance = 1000000000000;
//         double lat = std::stod(req.get_arg("lat"));
//         double lon = std::stod(req.get_arg("lon"));

//         for (Node v : problem.backbone.v_nodes)
//         {
//             double dis = v.distance(lat, lon);
//             // printf("s: [%f, %f], v: [%f, %f], dis: [%f]", 51.4894, 7.40577);
//             if (dis < best_distance)
//             {

//                 best_distance = dis;
//                 start = v;
//             }
//         }

//         problem.start = start.id;

//         Graph *rG = reduceGraph(&problem.backbone, problem.start, 200);
//         // rG = simplifyGraph(rG, problem.start);

//         std::string response = "{\n";
//         response += "  \"backbone\": \n    [\n";

//         for (Edge *edge : rG->v_edges)
//         {

//             response += "      {\n        \"line\": [ [" + std::to_string(edge->s.lat) + "," + std::to_string(edge->s.lon) + "],";
//             // for (int j = 0; j < edge->geo_locs.size(); j++) {
//             //     // auto pair = edge->geo_locs[i.g_id < path[i+1].g_id ? j : edge->geo_locs.size() - j - 1];
//             //     response += "        [" + std::to_string(edge->geo_locs[j].first) + "," + std::to_string(edge->geo_locs[j].second) + "], \n";
//             // }
//             response += " [" + std::to_string(edge->t.lat) + "," + std::to_string(edge->t.lon) + "] ],\n";
//             response += "        \"tags\": [";
//             for (std::string tag : edge->tags)
//             {
//                 response += "\"" + tag + "\", ";
//             }
//             if (edge->tags.size() > 0)
//             {
//                 response.pop_back();
//                 response.pop_back();
//             }
//             response += "]\n      },\n";
//             // response += "        [" + std::to_string(edge->geo_locs[edge->geo_locs.size() - 1].first) + "," + std::to_string(edge->geo_locs[edge->geo_locs.size() - 1].second) + "], \n";
//         }

//         response.pop_back();
//         response.pop_back();

//         response += "\n    ]\n";
//         response += "}";

//         return std::shared_ptr<string_response>(new string_response(response, 200, "application/json"));
//     }
// };

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

    // backbone_data bdr;
    // ws.register_resource("/backbone", &bdr);

    index_resource hwr;
    ws.register_resource("/", &hwr);

    ws.start(true);

    return 0;
}
