#include "SceneNode.h"

#include "SceneGraph.h"
#include <mutex>
#include <shared_mutex>
#include <sstream>

namespace sng
{
    constexpr matrix4 IDENTITY4{
        {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

    constexpr matrix4 matrixMultiplication(const matrix4& A, const matrix4& B)
    {
        matrix4 result;
        for (unsigned int i = 0; i < 4; ++i)
        {
            for (unsigned int j = 0; j < 4; ++j)
            {
                result[i][j] = 0;
                for (unsigned int k = 0; k < 4; ++k)
                {
                    result[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return result;
    }

    struct SceneNode::ThreadGuardImpl
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
        ThreadGuardImpl(const ThreadGuardImpl &) = delete;
        ThreadGuardImpl & operator=(const ThreadGuardImpl &) = delete;
        ThreadGuardImpl (ThreadGuardImpl &&) = delete;
        ThreadGuardImpl & operator=(ThreadGuardImpl &&) = delete;
        std::shared_mutex localTransformationMutex;
        std::shared_mutex globalTransformationMutex;
        std::mutex nameMutex;
        std::mutex childrenMutex;
        std::mutex parentMutex;
    };

    // Implementation of SceneNode methods

    SceneNode::SceneNode(const std::string& node_name, ISceneGraph* scene_graph)
        : pThreadGuardImpl(std::make_unique<ThreadGuardImpl>()),
          name(node_name),
          localTransformation(IDENTITY4),
          globalTransformation(IDENTITY4),
          sceneGraph(scene_graph)
    {
    }

    SceneNode::~SceneNode() = default;

    void SceneNode::setParent(SceneNode* newParent)
    {
        if (newParent)
        {
            {
                std::lock_guard lock(pThreadGuardImpl->parentMutex);
                if (parent)
                {
                    parent->deleteChild(
                        this);  // Remove from the children of the old parent
                }
                parent = newParent;
            }
            newParent->addChild(this);  // Add to the children of the new parent
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
        if (parent)
        {
            matrix4 parentGlobal = parent->getGlobalTransformation();
            globalTransformation =
                matrixMultiplication(parentGlobal, localTransformation);
        }
        else
        {
            globalTransformation = localTransformation;
        }
    }

    void SceneNode::addChild(SceneNode* childNode)
    {
        {
            std::lock_guard<std::mutex> lock(pThreadGuardImpl->childrenMutex);
            children.insert(childNode);
        }
        notifySceneGraph();
    }

    void SceneNode::deleteChild(SceneNode* childNode)
    {
        {
            std::lock_guard<std::mutex> lock(pThreadGuardImpl->childrenMutex);
            auto it = children.find(childNode);
            if (it != children.end())
            {
                children.erase(it);
            }
        }
        notifySceneGraph();
    }

    std::string SceneNode::render()
    {
        // Render using globalTransformation
        updateGlobalTransformation();
        std::stringstream ss{};
        ss << "Rendering Node: " << getName() << '\n';
        for (const auto& child : getChildren())
        {
            ss << child->render();
        }
        return ss.str();
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
