#pragma once
#include "SceneNode.h"
#include <string>

class IClient
{
   public:
    virtual ~IClient() = default;
    virtual std::string update(sng::ISceneNode* node) = 0;
};

class Client final : public IClient
{
   private:
    std::string name{};

   public:
    explicit Client(std::string name) : name(name) {}
    std::string update(sng::ISceneNode* node) override;
};