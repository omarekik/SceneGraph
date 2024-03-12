#pragma once

#include <array>
#include <memory>
#include <string>
#include <unordered_set>

namespace sng
{

    using matrix4 = std::array<std::array<int, 4>, 4>;

    class ISceneNode
    {
       public:
        virtual ~ISceneNode() = default;
        virtual std::string getName() const = 0;
        virtual std::string render() = 0;
    };

    class ISceneGraph;

    class SceneNode final : public ISceneNode
    {
       public:
        explicit SceneNode(const std::string& node_name,
                           ISceneGraph* scene_graph);
        ~SceneNode() override;
        SceneNode(const SceneNode&) = delete;
        SceneNode& operator=(const SceneNode&) = delete;
        SceneNode(SceneNode&&) = default;
        SceneNode& operator=(SceneNode&&) = default;

        std::string getName() const override;
        std::unordered_set<SceneNode*> getChildren() const;
        void setName(const std::string& node_name);
        void setParent(SceneNode* newParent);
        void setLocalTransformation(const matrix4& transformation);
        void addChild(SceneNode* childNode);
        void deleteChild(SceneNode* childNode);
        std::string render() override;
        // Custom comparison function comparing only the name attribute
        bool operator==(const SceneNode& other) const;
        // Observer pattern methods
        void notifySceneGraph();

       private:
        void updateGlobalTransformation();
        matrix4 getGlobalTransformation() const;

       private:
        struct ThreadGuardImpl;
        std::unique_ptr<ThreadGuardImpl> pThreadGuardImpl;

        std::string name{};
        matrix4 localTransformation;
        matrix4 globalTransformation;
        SceneNode* parent = nullptr;  // Pointer to parent node
        std::unordered_set<SceneNode*> children{};
        ISceneGraph* sceneGraph;
    };

}  // namespace sng
