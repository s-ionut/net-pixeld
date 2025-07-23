#pragma once

#include "IScene.hpp"

#include <memory>
#include <unordered_map>

enum class SceneID
{
    Login,
    Game
};

class SceneManager
{
public:
    template <typename SceneT, typename... CtorArgs>
    void registerScene(SceneID id, CtorArgs &...args)
    {
        m_factories[id] = [&args...]()
        {
            return std::make_unique<SceneT>(args...);
        };
    }

    void changeScene(SceneID id);

    void Update();
    void Draw();

    SceneID currentID() const;
    IScene *current() { return m_current.get(); };
    const IScene *current() const { return m_current.get(); };

private:
    SceneID m_currentID{SceneID::Login};

    std::unique_ptr<IScene> m_current;

    std::unordered_map<SceneID, std::function<std::unique_ptr<IScene>()>> m_factories;
};
