#include "Client.h"

#include <sstream>

std::string Client::update(sng::ISceneNode* node)
{
    std::stringstream ss{};
    ss << "Client " << name
       << " is updating due to notification from SceneNode: " << node->getName()
       << '\n';
    ss << node->render();
    return ss.str();
}
