#pragma once

#include "SquareMatrix.hpp"
#include <memory>
#include <string>
#include <unordered_set>

namespace sng
{
    class ISceneNode
    {
       public:
        virtual ~ISceneNode() = default;
        [[nodiscard]] virtual std::string getName() const = 0;
        [[nodiscard]] virtual std::string render() = 0;
    };

    class ISceneGraph;

    class SceneNode final : public ISceneNode
    {
       public:
        explicit SceneNode(const std::string& node_name,
                           ISceneGraph* scene_graph);
        ~SceneNode() override;
        SceneNode(const SceneNode& other) = delete;
        SceneNode& operator=(const SceneNode& other) = delete;
        SceneNode(SceneNode&& other) noexcept;
        SceneNode& operator=(SceneNode&& other) noexcept;

        [[nodiscard]] std::string getName() const override;
        [[nodiscard]] std::unordered_set<SceneNode*> getChildren() const;
        void setName(const std::string& node_name);
        void setParent(SceneNode* new_parent);
        void setLocalTransformation(const matrix4& transformation);
        void addChild(SceneNode* child_node);
        [[nodiscard]] bool isSubChildBFS(SceneNode* node) const;
        void deleteChild(SceneNode* child_node);
        [[nodiscard]] std::string render() override;
        // Custom comparison function comparing only the name attribute
        [[nodiscard]] bool operator==(const SceneNode& other) const;
        // Observer pattern methods
        void notifySceneGraph();

       private:
        void updateGlobalTransformation();
        [[nodiscard]] matrix4 getGlobalTransformation() const;

        std::string name{};
        matrix4 localTransformation;
        matrix4 globalTransformation;
        SceneNode* parent = nullptr;  // Pointer to parent node
        std::unordered_set<SceneNode*> children{};
        ISceneGraph* sceneGraph;
        struct ThreadGuardImpl;
        std::unique_ptr<ThreadGuardImpl> pThreadGuardImpl;
    };

}  // namespace sng
