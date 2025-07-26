#include "scenes/SceneManager.hpp"

void SceneManager::changeScene(SceneID id)
{
    if (m_current)
    {
        m_current->onExit();
        m_current.reset();
    }

    auto it = m_factories.find(id);
    if (it == m_factories.end())
    {
        LOG_ERROR("Unknown scene :(");
        return;
    }

    m_current = it->second();
    m_currentID = id;
    m_current->onEnter();
}

void SceneManager::Update()
{
    if (m_current)
        m_current->Update();
}

void SceneManager::Draw()
{
    if (m_current)
        m_current->Draw();
}

SceneID SceneManager::currentID() const
{
    return m_currentID;
}