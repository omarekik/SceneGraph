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
        SceneNode(const SceneNode&) = delete;
        SceneNode& operator=(const SceneNode&) = delete;
        SceneNode(SceneNode&&) noexcept;
        SceneNode& operator=(SceneNode&&) noexcept;

        [[nodiscard]] std::string getName() const override;
        [[nodiscard]] std::unordered_set<SceneNode*> getChildren() const;
        void setName(const std::string& node_name);
        void setParent(SceneNode* newParent);
        void setLocalTransformation(const matrix4& transformation);
        void addChild(SceneNode* childNode);
        void deleteChild(SceneNode* childNode);
        [[nodiscard]] std::string render() override;
        // Custom comparison function comparing only the name attribute
        [[nodiscard]] bool operator==(const SceneNode& other) const;
        // Observer pattern methods
        void notifySceneGraph();

       private:
        void updateGlobalTransformation();
        [[nodiscard]] matrix4 getGlobalTransformation() const;

       private:
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
