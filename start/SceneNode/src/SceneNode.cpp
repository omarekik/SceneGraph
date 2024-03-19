#include "SceneNode.h"

#include "SceneGraph.h"
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <sstream>

namespace sng
{
    struct SceneNode::NodeThreadImpl final
    {
        std::shared_mutex localTransformationMutex;
        std::shared_mutex globalTransformationMutex;
        std::mutex nameMutex;
        std::mutex childrenMutex;
        std::mutex parentMutex;
    };

    SceneNode::SceneNode(const std::string& node_name, ISceneGraph* scene_graph,
                         SceneNode* parent)
        : name(node_name),
          localTransformation(matrix4::identity()),
          globalTransformation(matrix4::identity()),
          parent(parent),
          sceneGraph(scene_graph),
          pNodeThreadImpl(std::make_unique<NodeThreadImpl>())
    {
    }

    SceneNode::SceneNode(SceneNode&&) noexcept = default;
    SceneNode& SceneNode::operator=(SceneNode&&) noexcept = default;
    SceneNode::~SceneNode() = default;

    void SceneNode::setParent(SceneNode* new_parent)
    {
        if (new_parent != nullptr)
        {
            {
                std::lock_guard lock(pNodeThreadImpl->parentMutex);
                if (parent != nullptr)
                {
                    parent->deleteChild(
                        this);  // Remove from the children of the old parent
                }
                parent = new_parent;
            }
            new_parent->addChild(
                this);  // Add to the children of the new parent
            notifySceneGraph();
        }
    }

    void SceneNode::setLocalTransformation(const matrix4& transformation)
    {
        {
            std::unique_lock lock(pNodeThreadImpl->localTransformationMutex);
            localTransformation = transformation;
        }
        notifySceneGraph();
    }

    void SceneNode::updateGlobalTransformation()
    {
        std::unique_lock parent_lock(pNodeThreadImpl->parentMutex,
                                     std::defer_lock);
        std::unique_lock local_lock(pNodeThreadImpl->localTransformationMutex,
                                    std::defer_lock);
        std::shared_lock global_lock(pNodeThreadImpl->globalTransformationMutex,
                                     std::defer_lock);
        std::scoped_lock lock(parent_lock, local_lock, global_lock);
        if (parent != nullptr)
        {
            matrix4 parent_global = parent->getGlobalTransformation();
            globalTransformation = parent_global * localTransformation;
        }
        else
        {
            globalTransformation = localTransformation;
        }
    }

    void SceneNode::addChild(SceneNode* child_node)
    {
        {
            std::lock_guard<std::mutex> lock(pNodeThreadImpl->childrenMutex);
            children.insert(child_node);
        }
        notifySceneGraph();
    }

    bool SceneNode::isSubChildBFS(SceneNode* node) const
    {
        if (node == nullptr)
        {
            return false;
        }
        std::queue<const SceneNode*> sub_children_queue{};
        sub_children_queue.push(this);
        while (!sub_children_queue.empty())
        {
            const SceneNode* current = sub_children_queue.front();
            sub_children_queue.pop();
            if (current == node)
            {
                return true;
            }
            // Iterate through the children of the current node to fill the
            // queue
            for (SceneNode* child : current->getChildren())
            {
                sub_children_queue.push(child);
            }
        }
        return false;  // Return false if the given node is not found in the
                       // subtree
    }

    void SceneNode::deleteChild(SceneNode* child_node)
    {
        {
            std::lock_guard<std::mutex> lock(pNodeThreadImpl->childrenMutex);
            auto child_it = children.find(child_node);
            if (child_it != children.end())
            {
                children.erase(child_it);
            }
        }
        notifySceneGraph();
    }

    std::string SceneNode::render()
    {
        // Render using globalTransformation
        updateGlobalTransformation();
        std::stringstream string_stream{};
        std::string parent_name{};
        {
            std::lock_guard lock(pNodeThreadImpl->parentMutex);
            if (parent != nullptr)
            {
                parent_name = parent->getName();
            }
        }
        string_stream << "Rendering Node: " << getName() << " child of "
                      << parent_name << '\n';
        for (const auto& child : getChildren())
        {
            string_stream << child->render();
        }
        return string_stream.str();
    }

    matrix4 SceneNode::getGlobalTransformation() const
    {
        std::unique_lock lock(pNodeThreadImpl->globalTransformationMutex);
        return globalTransformation;
    }

    void SceneNode::setName(const std::string& node_name)
    {
        {
            std::lock_guard<std::mutex> lock(pNodeThreadImpl->nameMutex);
            name = node_name;
        }
        notifySceneGraph();
    }

    std::string SceneNode::getName() const
    {
        std::lock_guard<std::mutex> lock(pNodeThreadImpl->nameMutex);
        return name;
    }

    std::unordered_set<SceneNode*> SceneNode::getChildren() const
    {
        std::lock_guard<std::mutex> lock(pNodeThreadImpl->childrenMutex);
        return children;
    }

    void SceneNode::notifySceneGraph()
    {
        sceneGraph->update(this);
    }

    bool SceneNode::operator==(const SceneNode& other) const
    {
        return name == other.name;
    }

}  // namespace sng
