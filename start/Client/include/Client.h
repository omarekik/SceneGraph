#pragma once
#include "SceneNode.h"
#include <string>

class IClient
{
   public:
    virtual ~IClient() = default;
    // Observer
    virtual void update(sng::ISceneNode* node) = 0;
};

class Client final : public IClient
{
   private:
    std::string name{};
    sng::ISceneGraph* sceneGraph;

   public:
    explicit Client(std::string name, sng::ISceneGraph* scene_graph);
    ~Client() override;
    Client(const Client&) = default;
    Client& operator=(const Client&) = default;
    Client(Client&&) = default;
    Client& operator=(Client&&) = default;

    void addRoot(const std::string& root_name);
    void addChild(const std::string& name_child,
                  const std::string& name_parent);
    void update(sng::ISceneNode* node) override;
};