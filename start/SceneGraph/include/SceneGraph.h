#pragma once

#include "Client.h"
#include "SceneNode.h"
#include <list>
#include <unordered_map>

// TODO add ThreadGuardImpl
namespace sng
{
    class ISceneGraph
    {
       public:
        virtual ~ISceneGraph() = default;
        virtual void update(ISceneNode* observee) = 0;
        virtual void attachClient(IClient* observer) = 0;
        virtual void detachClient(IClient* observer) = 0;
    };

    class SceneGraph final : public ISceneGraph
    {
       private:
        std::unordered_map<std::string, SceneNode> nameToNode{};
        SceneNode* rootNode = nullptr;
        std::list<IClient*> clients{};

       public:
        void addRoot(std::string root_name);
        void addChild(std::string name_child, std::string name_parent = "");
        // TODO ReParent(nodeName, newParentName) and delete(NodeName)
        void render() const;

        void update(ISceneNode* observee) override;

        void attachClient(IClient* observer) override;
        void detachClient(IClient* observer) override;
    };
}  // namespace sng
