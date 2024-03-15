#include "Client.h"
#include "SceneGraph.h"

int main()
{
    // Instantiate a SceneGraph
    sng::SceneGraph scene_graph;

    // Instantiate two Clients
    Client client1("Client1", &scene_graph);
    Client client2("Client2", &scene_graph);

    // Add roots and children to the scene_graph
    client1.addRoot("Root1");
    client1.addChild("Child1_1", "Root1");
    client1.addChild("Child1_2", "Root1");
    client2.addChild("Child2_1", "Child1_1");
    client2.addChild("Child2_2", "Root1");

    // Trigger a transformation change from the first Client
    // sng::matrix4 new_transformation {};
    // scene_graph.getNode("Root1")->setLocalTransformation(new_transformation);

    return 0;
}
