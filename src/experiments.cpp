#include "experiments.h"

void runExperiments()
{
    Problem P = Problem("/home/hagedoorn/Documents/TUD/Code/tour4me/input/grid-demo.txt");

    double budgets[7] = {1 * 1000, 2.5 * 1000, 5 * 1000, 10 * 1000, 25 * 1000, 50 * 1000, 100 * 1000};
    double importances[3] = {0, 0.5, 1.0};
    // double importances[1] = {0.5};
    // double budgets[1]= {200*1000};

    double runningTimes[5] = {0.5, 1, 2, 5, 10};
    // double runningTimes[5] = {300};

    P.pref_tags = {"cycleway", "path", "track", "gravel", "compacted", "compacted_gravel", "fine_gravel", "ground"};
    // P.avoid_tags = {"residential", "cobblestone", "unpaved", "rock"};

    std::ofstream output("experiments2.csv");
    output << "timeBound, qualityImprov, budget" << std::endl;
    // {'timeBound': [], 'qualityImprov': [], 'budget': []}

    // output << "name" << "," << "lat" << "," << "lon" << "," << "timeBound" << "," << "budget" << "," << "importance" << "," << "quality" << "," << "area" << "," << "profit" << "," << "length" << "," << "time" << "," << "status" << std::endl;

    P.calculateProfit(&P.graph);

    Selection sel;
    Jogger jog;
    ILS ils;
    ILP ilp;

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int s_id = rand() % P.graph.v_nodes.size();
            P.start = s_id;
            Node start = P.graph.v_nodes[s_id];

            for (double imp : importances)
            {
                for (double B : budgets)
                {
                    P.target_distance = B;

                    P.edgeProfitImportance = 1 - imp;
                    P.coveredAreaImportance = imp;

                    P.path.clear();
                    P.quality = -1;

                    SolveStatus status = SolveStatus::Unsolved;

                    auto algo_time_1 = std::chrono::high_resolution_clock::now();

                    // status = sel.solve(&P);

                    auto algo_time_2 = std::chrono::high_resolution_clock::now();

                    auto time_int = std::chrono::duration_cast<std::chrono::milliseconds>(algo_time_2 - algo_time_1);

                    double profit = -1;
                    double area = -1;
                    double quality = -1;

                    if (status != SolveStatus::Unsolved && status != SolveStatus::Timeout)
                    {
                        profit = P.getProfit(P.path);
                        area = P.getArea(P.path);
                        quality = P.getQuality(profit, area);

                        // output << "sel"
                        //           << "," << start.lat << "," << start.lon << "," << rT << "," << B << "," << imp << "," << quality << "," << area << "," << profit << "," << P.getLength(P.path) << "," << time_int.count() << "," << static_cast<std::underlying_type<SolveStatus>::type>(status) << std::endl;
                    }
                    else
                    {
                        // output << "sel"
                        //           << "," << start.lat << "," << start.lon << "," << rT << "," << B << "," << imp << "," << quality << "," << area << "," << profit << "," << -1 << "," << time_int.count() << "," << static_cast<std::underlying_type<SolveStatus>::type>(status) << std::endl;
                    }
                    P.path.clear();
                    P.quality = -1;

                    status = SolveStatus::Unsolved;

                    algo_time_1 = std::chrono::high_resolution_clock::now();

                    // status = ilp.solve(&P);

                    algo_time_2 = std::chrono::high_resolution_clock::now();

                    time_int = std::chrono::duration_cast<std::chrono::milliseconds>(algo_time_2 - algo_time_1);

                    profit = -1;
                    area = -1;
                    quality = -1;

                    if (status != SolveStatus::Unsolved && status != SolveStatus::Timeout)
                    {
                        profit = P.getProfit(P.path);
                        area = P.getArea(P.path);
                        quality = P.getQuality(profit, area);

                        // output << "ilp"
                        //           << "," << start.lat << "," << start.lon << "," << rT << "," << B << "," << imp << "," << quality << "," << area << "," << profit << "," << P.getLength(P.path) << "," << time_int.count() << "," << static_cast<std::underlying_type<SolveStatus>::type>(status) << std::endl;
                    }
                    else
                    {
                        // output << "ilp"
                        //           << "," << start.lat << "," << start.lon << "," << rT << "," << B << "," << imp << "," << quality << "," << area << "," << profit << "," << -1 << "," << time_int.count() << "," << static_cast<std::underlying_type<SolveStatus>::type>(status) << std::endl;
                    }

                    P.path.clear();
                    P.quality = -1;
                    status = SolveStatus::Unsolved;

                    algo_time_1 = std::chrono::high_resolution_clock::now();
                    status = jog.solve(&P);


                    algo_time_2 = std::chrono::high_resolution_clock::now();

                    time_int = std::chrono::duration_cast<std::chrono::milliseconds>(algo_time_2 - algo_time_1);

                    profit = -1;
                    area = -1;
                    quality = 0;

                    if (status != SolveStatus::Unsolved && status != SolveStatus::Timeout)
                    {
                        // output << "jog"
                        //           << "," << start.lat << "," << start.lon << "," << rT << "," << B << "," << imp << "," << quality << "," << area << "," << profit << "," << P.getLength(P.path) << "," << time_int.count() << "," << static_cast<std::underlying_type<SolveStatus>::type>(status) << std::endl;
                    }
                    else
                    {
                        // output << "jog"
                        //           << "," << start.lat << "," << start.lon << "," << rT << "," << B << "," << imp << "," << quality << "," << area << "," << profit << "," << -1 << "," << time_int.count() << "," << static_cast<std::underlying_type<SolveStatus>::type>(status) << std::endl;

                        continue;
                    }
                    profit = P.getProfit(P.path);
                    area = P.getArea(P.path);
                    quality = P.getQuality(profit, area);

                    if (quality <= 0)
                    {
                        continue;
                    }


                    std::list<int> path(P.path);

                    P.path.clear();
                    std::cout << path.size() << ", " << imp << "\n";

                    for (double rT : runningTimes)
                    {

                        P.runningTime = rT;

                        for (int v : path)
                        {
                            P.path.push_back(v);
                        }
                        P.quality = quality;

                        algo_time_1 = std::chrono::high_resolution_clock::now();
                        status = ils.solve(&P);
                        algo_time_2 = std::chrono::high_resolution_clock::now();

                        time_int = std::chrono::duration_cast<std::chrono::milliseconds>(algo_time_2 - algo_time_1);

                        profit = -1;
                        area = -1;
                        double ilsquality = 0;

                        if (status != SolveStatus::Unsolved && status != SolveStatus::Timeout)
                        {
                            profit = P.getProfit(P.path);
                            area = P.getArea(P.path);
                            ilsquality = P.getQuality(profit, area);
                            output << rT << "," << (ilsquality - quality) / quality << "," << B << std::endl;
                            // output << "ils"
                            //           << "," << start.lat << "," << start.lon << "," << rT << "," << B << "," << imp << "," << quality << "," << area << "," << profit << "," << P.getLength(P.path) << "," << time_int.count() << "," << static_cast<std::underlying_type<SolveStatus>::type>(status) << std::endl;
                        }
                        else
                        {
                            // output << "ils"
                            //           << "," << start.lat << "," << start.lon << "," << rT << "," << B << "," << imp << "," << quality << "," << area << "," << profit << "," << -1 << "," << time_int.count() << "," << static_cast<std::underlying_type<SolveStatus>::type>(status) << std::endl;
                        }
                    }
                }
            }
        }
    }

    output.close();
}