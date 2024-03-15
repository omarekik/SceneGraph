#include "SceneNode.h"

#include "SceneGraph.h"
#include <mutex>
#include <shared_mutex>
#include <sstream>

namespace sng
{
    constexpr matrix4 IDENTITY4{
        {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

    constexpr matrix4 matrix_multiplication(const matrix4& left, const matrix4& right)
    {
        matrix4 result;
        for (unsigned int i = 0; i < 4; ++i)
        {
            for (unsigned int j = 0; j < 4; ++j)
            {
                result[i][j] = 0;
                for (unsigned int k = 0; k < 4; ++k)
                {
                    result[i][j] += left[i][k] * right[k][j];
                }
            }
        }
        return result;
    }

    struct SceneNode::ThreadGuardImpl final
    {  // not copyable/movable as mutex member
        ThreadGuardImpl() = default;
        ~ThreadGuardImpl()
        {
            localTransformationMutex.unlock();
            globalTransformationMutex.unlock();
            nameMutex.unlock();
            childrenMutex.unlock();
            parentMutex.unlock();
        }
        ThreadGuardImpl(const ThreadGuardImpl&) = delete;
        ThreadGuardImpl& operator=(const ThreadGuardImpl&) = delete;
        ThreadGuardImpl(ThreadGuardImpl&&) = delete;
        ThreadGuardImpl& operator=(ThreadGuardImpl&&) = delete;
        std::shared_mutex localTransformationMutex;
        std::shared_mutex globalTransformationMutex;
        std::mutex nameMutex;
        std::mutex childrenMutex;
        std::mutex parentMutex;
    };

    // Implementation of SceneNode methods

    SceneNode::SceneNode(const std::string& node_name, ISceneGraph* scene_graph)
        : name(node_name),
          localTransformation(IDENTITY4),
          globalTransformation(IDENTITY4),
          sceneGraph(scene_graph),
          pThreadGuardImpl(std::make_unique<ThreadGuardImpl>())
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
                std::lock_guard lock(pThreadGuardImpl->parentMutex);
                if (parent != nullptr)
                {
                    parent->deleteChild(
                        this);  // Remove from the children of the old parent
                }
                parent = new_parent;
            }
            new_parent->addChild(this);  // Add to the children of the new parent
            notifySceneGraph();
        }
    }

    void SceneNode::setLocalTransformation(const matrix4& transformation)
    {
        {
            std::unique_lock lock(pThreadGuardImpl->localTransformationMutex);
            localTransformation = transformation;
        }
        notifySceneGraph();
    }

    void SceneNode::updateGlobalTransformation()
    {
        std::unique_lock parent_lock(pThreadGuardImpl->parentMutex,
                                     std::defer_lock);
        std::unique_lock local_lock(pThreadGuardImpl->localTransformationMutex,
                                    std::defer_lock);
        std::shared_lock global_lock(
            pThreadGuardImpl->globalTransformationMutex, std::defer_lock);
        std::scoped_lock lock(parent_lock, local_lock, global_lock);
        if (parent != nullptr)
        {
            matrix4 parent_global = parent->getGlobalTransformation();
            globalTransformation =
                matrix_multiplication(parent_global, localTransformation);
        }
        else
        {
            globalTransformation = localTransformation;
        }
    }

    void SceneNode::addChild(SceneNode* child_node)
    {
        {
            std::lock_guard<std::mutex> lock(pThreadGuardImpl->childrenMutex);
            children.insert(child_node);
        }
        notifySceneGraph();
    }

    void SceneNode::deleteChild(SceneNode* child_node)
    {
        {
            std::lock_guard<std::mutex> lock(pThreadGuardImpl->childrenMutex);
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
        string_stream << "Rendering Node: " << getName() << '\n';
        for (const auto& child : getChildren())
        {
            string_stream << child->render();
        }
        return string_stream.str();
    }

    matrix4 SceneNode::getGlobalTransformation() const
    {
        std::unique_lock lock(pThreadGuardImpl->globalTransformationMutex);
        return globalTransformation;
    }

    void SceneNode::setName(const std::string& node_name)
    {
        {
            std::lock_guard<std::mutex> lock(pThreadGuardImpl->nameMutex);
            name = node_name;
        }
        notifySceneGraph();
    }

    std::string SceneNode::getName() const
    {
        std::lock_guard<std::mutex> lock(pThreadGuardImpl->nameMutex);
        return name;
    }

    std::unordered_set<SceneNode*> SceneNode::getChildren() const
    {
        std::lock_guard<std::mutex> lock(pThreadGuardImpl->childrenMutex);
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
