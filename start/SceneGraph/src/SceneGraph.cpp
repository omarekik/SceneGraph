#include "SceneGraph.h"

#include <algorithm>
#include <iostream>
#include <syncstream>

namespace sng
{
    void SceneGraph::addRoot(const std::string& root_name)
    {
        if (root_name.empty() || !nameToNode.empty())
        {
            std::osyncstream(std::cout)
                << "Adding root will not be done "
                << "because the scene graph already has root or the new root "
                   "name is empty.\n";
        }
        else
        {
            // std::pair<std::string, SceneNode&&> element {root_name,
            // SceneNode(root_name, this)};
            auto [child_it, done] = nameToNode.emplace(
                std::piecewise_construct, std::forward_as_tuple(root_name),
                std::forward_as_tuple(root_name, this));
            rootNode = &(child_it->second);
        }
    }

    void SceneGraph::addChild(const std::string& name_child,
                              const std::string& name_parent)
    {
        if (nameToNode.empty())
        {
            std::osyncstream(std::cout)
                << "Scene graph is empty. First you need to add a root node.\n";
        }
        else if (name_child.empty() || nameToNode.contains(name_child))
        {
            std::osyncstream(std::cout) << "The name is unique for each node. "
                                           "Adding child will not be done "
                                        << "because the given child name is "
                                           "either empty or already used.\n";
        }
        else
        {
            auto parent_it = nameToNode.find(name_parent);
            if (name_parent.empty() || parent_it == nameToNode.end())
            {
                std::osyncstream(std::cout)
                    << "A not empty name of parent is required for adding a "
                       "child node. "
                    << "It must be a name of exist node in the scene graph.\n";
            }
            else
            {
                // std::pair<std::string, SceneNode&&> element {name_child,
                // SceneNode(name_child, this)};
                auto [child_it, done] = nameToNode.emplace(
                    std::piecewise_construct, std::forward_as_tuple(name_child),
                    std::forward_as_tuple(name_child, this));
                parent_it->second.addChild(&(child_it->second));
            }
        }
    }

    void SceneGraph::render() const
    {
        if (rootNode)
        {
            rootNode->render();
        }
        else
        {
            std::osyncstream(std::cout) << "No root node found.\n";
        }
    }

    void SceneGraph::update(ISceneNode* node)
    {
        for (auto client : clients)
        {
            client->update(node);
        }
    }

    void SceneGraph::attachClient(IClient* observer)
    {
        clients.push_back(observer);
    }

    void SceneGraph::detachClient(IClient* observer)
    {
        auto it = std::find(clients.begin(), clients.end(), observer);
        if (it != clients.end())
        {
            clients.erase(it);
        }
    }

}  // namespace sng
