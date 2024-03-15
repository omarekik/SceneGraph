#include "Client.h"

#include "SceneGraph.h"
#include <iostream>
#include <sstream>
#include <syncstream>

Client::Client(const std::string& name, sng::ISceneGraph* scene_graph)
    : name(name), sceneGraph(scene_graph)
{
    if (sceneGraph != nullptr)
    {
        sceneGraph->attachClient(this);
    }
}
Client::~Client()
{
    if (sceneGraph != nullptr)
    {
        sceneGraph->detachClient(this);
    }
}

void Client::addRoot(const std::string& root_name)
{
    if (sceneGraph != nullptr)
    {
        sceneGraph->addRoot(root_name);
    }
}

void Client::addChild(const std::string& name_child,
                      const std::string& name_parent)
{
    if (sceneGraph != nullptr)
    {
        sceneGraph->addChild(name_child, name_parent);
    }
}

void Client::update(sng::ISceneNode* node)
{
    std::stringstream string_stream{};
    string_stream << "Client " << name
       << " is updating due to notification from SceneNode: " << node->getName()
       << '\n';
    string_stream << node->render();
    std::osyncstream(std::cout) << string_stream.str();
}
