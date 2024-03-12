#include "Client.h"
#include "SceneGraph.h"

int main()
{
    // Instantiate a SceneGraph
    sng::SceneGraph sceneGraph;

    // Instantiate two Clients
    Client client1("Client1", &sceneGraph);
    Client client2("Client2", &sceneGraph);

    // Add roots and children to the SceneGraph
    client1.addRoot("Root1");
    client1.addChild("Child1_1", "Root1");
    client1.addChild("Child1_2", "Root1");
    client2.addChild("Child2_1", "Child1_1");
    client2.addChild("Child2_2", "Root1");

    // Trigger a transformation change from the first Client
    std::string newTransformation = "New Transformation";
    // sceneGraph.getNode("Root1")->setLocalTransformation(newTransformation);

    return 0;
}
