#include "Client.h"
#include "SceneGraph.h"
#include <iostream>
#include <thread>

int main()
{
    // Instantiate a SceneGraph
    sng::SceneGraph scene_graph;

    // Instantiate two Clients
    Client client1("Client1", &scene_graph);
    // Add roots and children to the scene_graph
    client1.addRoot("Root1");
    auto work = [&scene_graph](const int WORK_INDEX)
    {
        Client client2("Client" + std::to_string(WORK_INDEX), &scene_graph);
        client2.addChild("Node" + std::to_string(WORK_INDEX) + "_1", "Root1");
        const size_t DEPTH{5};
        const size_t WIDTH{5};
        for (size_t node_index{2}; node_index < DEPTH; ++node_index)
        {
            client2.addChild("Node" + std::to_string(WORK_INDEX) + '_' +
                                 std::to_string(node_index),
                             "Node" + std::to_string(WORK_INDEX) + '_' +
                                 std::to_string(node_index - 1));
        }
        for (size_t node_index{DEPTH}; node_index < (DEPTH + WIDTH);
             ++node_index)
        {
            client2.addChild("Node" + std::to_string(WORK_INDEX) + '_' +
                                 std::to_string(node_index),
                             "Node" + std::to_string(WORK_INDEX) + "_3");
        }
    };
    {
        std::jthread worker2(work, 2);
        std::jthread worker3(work, 3);
        work(1);
    }

    std::cout << "End of demo. \n";

    // Trigger a transformation change from the first Client
    // sng::matrix4 new_transformation {};
    // scene_graph.setLocalTransformation("Root1", new_transformation);

    return 0;
}
