#include "SceneGraph.h"

#include <algorithm>
#include <iostream>
#include <mutex>
#include <syncstream>

namespace sng
{
    struct SceneGraph::GraphThreadImpl final
    {
        std::mutex nameToNodeMutex;
        std::mutex rootNodeMutex;
        std::mutex clientsMutex;
    };

    SceneGraph::SceneGraph()
        : pGraphThreadImpl(std::make_unique<GraphThreadImpl>())
    {
    }
    SceneGraph::~SceneGraph() = default;
    SceneGraph::SceneGraph(SceneGraph&& other) noexcept = default;
    SceneGraph& SceneGraph::operator=(SceneGraph&& other) noexcept = default;

    void SceneGraph::addRoot(const std::string& root_name)
    {
        std::lock_guard lock(pGraphThreadImpl->nameToNodeMutex);
        if (root_name.empty() || !nameToNode.empty())
        {
            std::osyncstream(std::cout)
                << "Adding root will not be done because the scene graph "
                   "already has root or the new root name is empty. Actual "
                   "input is: root_name = "
                << root_name << ".\n";
        }
        else
        {
            auto [child_it, done] = nameToNode.emplace(
                std::piecewise_construct, std::forward_as_tuple(root_name),
                std::forward_as_tuple(root_name, this, nullptr));
            std::lock_guard lock(pGraphThreadImpl->rootNodeMutex);
            rootNode = &(child_it->second);
        }
    }

    void SceneGraph::addChild(const std::string& name_child,
                              const std::string& name_parent)
    {
        std::lock_guard lock(pGraphThreadImpl->nameToNodeMutex);
        if (nameToNode.empty())
        {
            std::osyncstream(std::cout)
                << "Scene graph is empty. First you need to add a root node.\n";
        }
        else if (name_child.empty() || nameToNode.contains(name_child))
        {
            std::osyncstream(std::cout)
                << "The name is unique for each node. Adding child will not be "
                   "done because the given child name is either empty or "
                   "already used. Actual input is: name_child = "
                << name_child << ".\n";
        }
        else
        {
            auto parent_it = nameToNode.find(name_parent);
            if (name_parent.empty() || parent_it == nameToNode.end())
            {
                std::osyncstream(std::cout)
                    << "A not empty name of parent is required for adding a "
                       "child node. It must be a name of existing node in the "
                       "scene graph. Actual input is: name_child = "
                    << name_child << ", name_parent = " << name_parent << ".\n";
            }
            else
            {
                auto [child_it, done] = nameToNode.emplace(
                    std::piecewise_construct, std::forward_as_tuple(name_child),
                    std::forward_as_tuple(name_child, this,
                                          &(parent_it->second)));
                parent_it->second.addChild(&(child_it->second));
            }
        }
    }
    void SceneGraph::reParent(const std::string& name_child,
                              const std::string& name_new_parent)
    {
        bool is_empty_name_to_node{};
        std::unordered_map<std::string, sng::SceneNode>::iterator child_it{};
        std::unordered_map<std::string, sng::SceneNode>::iterator
            new_parent_it{};
        std::unordered_map<std::string, sng::SceneNode>::iterator
            name_to_node_end_it{};
        {
            std::lock_guard lock(pGraphThreadImpl->nameToNodeMutex);
            is_empty_name_to_node = nameToNode.empty();
            child_it = nameToNode.find(name_child);
            new_parent_it = nameToNode.find(name_new_parent);
            name_to_node_end_it = nameToNode.end();
        }

        if (is_empty_name_to_node)
        {
            std::osyncstream(std::cout)
                << "Scene graph is empty. First you need to add a root node.\n";
        }
        else if (name_child.empty() || name_new_parent.empty())
        {
            std::osyncstream(std::cout)
                << "Child name and new parent name must be not empty string.\n";
        }
        else
        {
            if (child_it == name_to_node_end_it ||
                new_parent_it == name_to_node_end_it)
            {
                std::osyncstream(std::cout)
                    << "Child node and new parent node must be existing in "
                       "scene graph with mentioned names.\n";
            }
            else if (child_it->second.isSubChildBFS(&(new_parent_it->second)))
            {
                std::osyncstream(std::cout)
                    << "New parent must be not a sub node of child. This means "
                       "that it does not appear in all the subtree under "
                       "child.\n";
            }
            else
            {
                child_it->second.setParent(&(new_parent_it->second));
            }
        }
    }

    void SceneGraph::render() const
    {
        SceneNode* root_node = nullptr;
        {
            std::lock_guard lock(pGraphThreadImpl->rootNodeMutex);
            root_node = rootNode;
        }
        if (root_node != nullptr)
        {
            std::osyncstream(std::cout) << root_node->render();
        }
        else
        {
            std::osyncstream(std::cout) << "No root node found.\n";
        }
    }

    void SceneGraph::update(ISceneNode* observee)
    {
        std::list<IClient*> clients_copy{};
        {
            std::lock_guard lock(pGraphThreadImpl->clientsMutex);
            clients_copy = clients;
        }
        for (auto* client : clients_copy)
        {
            client->update(observee);
        }
    }

    void SceneGraph::attachClient(IClient* observer)
    {
        std::lock_guard lock(pGraphThreadImpl->clientsMutex);
        clients.push_back(observer);
    }

    void SceneGraph::detachClient(IClient* observer)
    {
        std::lock_guard lock(pGraphThreadImpl->clientsMutex);
        auto client_it = std::find(clients.begin(), clients.end(), observer);
        if (client_it != clients.end())
        {
            clients.erase(client_it);
        }
    }

}  // namespace sng
