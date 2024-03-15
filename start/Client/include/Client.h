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
    explicit Client(const std::string& name, sng::ISceneGraph* scene_graph);
    ~Client() override;
    Client(const Client&) noexcept = default;
    Client& operator=(const Client&) noexcept = default;
    Client(Client&&) noexcept = default;
    Client& operator=(Client&&) noexcept = default;

    void addRoot(const std::string& root_name);
    void addChild(const std::string& name_child,
                  const std::string& name_parent);
    void update(sng::ISceneNode* node) override;
};