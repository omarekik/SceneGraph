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

        virtual void addRoot(const std::string& root_name) = 0;
        virtual void addChild(const std::string& name_child,
                              const std::string& name_parent) = 0;
    };

    class SceneGraph final : public ISceneGraph
    {
       private:
        std::unordered_map<std::string, SceneNode> nameToNode{};
        SceneNode* rootNode = nullptr;
        std::list<IClient*> clients{};

       public:
        void addRoot(const std::string& root_name) override;
        void addChild(const std::string& name_child,
                      const std::string& name_parent) override;
        // TODO ReParent(nodeName, newParentName) and delete(NodeName)
        void render() const;

        void update(ISceneNode* observee) override;

        void attachClient(IClient* observer) override;
        void detachClient(IClient* observer) override;
    };
}  // namespace sng
