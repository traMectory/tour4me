#include "main.h"
#include <random>
#include <httpserver.hpp>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <fstream>

using namespace httpserver;

class url_args_resource : public http_resource {
public:
    const std::shared_ptr<http_response> render_GET(const http_request& req) {
        double lat;
        double lon;
        double distance;

        try
        {
            lat = std::stod(req.get_arg("lat"));
            lon = std::stod(req.get_arg("lon"));
            distance = std::stod(req.get_arg("dis"));
            printf("Got request: lat %f, lon %f, dis %f\n", lat, lon, distance);
        }
        catch(const std::exception& e)
        {      
            return std::shared_ptr<string_response>(new string_response("Bad input", 400, "text/plain"));
        }
        
        Problem problem("/home/hagedoorn/Documents/TUD/Code/AOPcpp/input/100kArbeit.txt");

        double best_distance = 1000000000000;
        Node start;

        for (Node v : problem.graph.v_nodes) {
            double dis = v.distance(lat, lon);
            // printf("s: [%f, %f], v: [%f, %f], dis: [%f]", 51.4894, 7.40577);
            if (dis < best_distance) {


                best_distance = dis;
                start = v;
            }
        }

        problem.start = start.id;
        problem.center_lon = start.lon;
        problem.center_lat = start.lat;

        problem.target_distance = distance;

        Jogger solver;
        SolveStatus status = solver.solve(&problem);

        switch(status)
        {
            case SolveStatus::Optimal: 
                printf("Optimal solution found!\n"); 
                return std::shared_ptr<string_response>(new string_response(problem.outputToString(), 200, "application/json"));
                if(gpx) {problem.outputToGPX(filename);} 
                break;
            case SolveStatus::Feasible: 
                printf("Feasible solution found\n"); 
                return std::shared_ptr<string_response>(new string_response(problem.outputToString(), 200, "application/json"));
                if(gpx) {problem.outputToGPX(filename);} 
                break;
            case SolveStatus::Unsolved: 
                printf("Problem was unsolved\n"); 
                return std::shared_ptr<string_response>(new string_response("Not solved", 400, "text/plain"));
                break;
        }
        return std::shared_ptr<string_response>(new string_response("Not solved", 400, "text/plain"));

    }
};

inline bool exists_test0 (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

class file_response_resource : public http_resource {
public:
    const std::shared_ptr<http_response> render_GET(const http_request& req) {
        printf("test: %d\n", exists_test0("/home/hagedoorn/Documents/TUD/Code/AOPcpp/web/main.html"));
        return std::shared_ptr<file_response>(new file_response("/home/hagedoorn/Documents/TUD/Code/AOPcpp/web/main.html", 200, "text/html"));
    }
};

int main(int argc, char** argv) {
        webserver ws = create_webserver(8080);

        url_args_resource uar;
        ws.register_resource("/tour", &uar);


        file_response_resource hwr;
        ws.register_resource("/index.html", &hwr);

        ws.start(true);
        
        return 0;
    }



// int main(int argc, char* argv[]) 
// {
//     parseOptions(argc, argv);



//     Problem problem("/home/hagedoorn/Documents/TUD/Code/AOPcpp/input/" + filename + ".txt");
//     // // Graph graph = getSmallGraph();
//     printf("Nodes: %d, Edges: %d\n", problem.getNumberOfNodes(), problem.getNumberOfEdges());

//     S_Colony solver;
    
//     SolveStatus status = solver.solve(&problem);
//     double baseLine = problem.quality;
//     bool update = true;





   
    

//     switch(status)
//     {
//         case SolveStatus::Optimal   : printf("Optimal solution found!\n"); problem.outputPath(filename); if(gpx) {problem.outputToGPX(filename);} break;
//         case SolveStatus::Feasible  : printf("Feasible solution found\n"); problem.outputPath(filename); if(gpx) {problem.outputToGPX(filename);} break;
//         case SolveStatus::Unsolved  : printf("Problem was unsolved\n"); break;
//     }

//     // problem.outputPath(path, filename);
    
//     return 0;
// }
