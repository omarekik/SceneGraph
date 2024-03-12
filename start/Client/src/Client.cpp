#include "Client.h"

#include "SceneGraph.h"
#include <iostream>
#include <sstream>
#include <syncstream>

Client::Client(std::string name, sng::ISceneGraph* scene_graph)
    : name(name), sceneGraph(scene_graph)
{
    if (sceneGraph)
    {
        sceneGraph->attachClient(this);
    }
}
Client::~Client()
{
    if (sceneGraph)
    {
        sceneGraph->detachClient(this);
    }
}

void Client::addRoot(const std::string& root_name)
{
    if (sceneGraph)
    {
        sceneGraph->addRoot(root_name);
    }
}

void Client::addChild(const std::string& name_child,
                      const std::string& name_parent)
{
    if (sceneGraph)
    {
        sceneGraph->addChild(name_child, name_parent);
    }
}

void Client::update(sng::ISceneNode* node)
{
    std::stringstream ss{};
    ss << "Client " << name
       << " is updating due to notification from SceneNode: " << node->getName()
       << '\n';
    ss << node->render();
    std::osyncstream(std::cout) << ss.str();
}
