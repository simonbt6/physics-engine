#include "layer_stack.hpp"

using namespace Graphics;

LayerStack::~LayerStack()
{
    for (Layer* layer : m_Layers)
    {
        layer->onDetach();
        delete layer;
    }
}

void LayerStack::Push(Layer* layer)
{
    m_Layers.emplace(m_Layers.begin() + m_InsIndex, layer);
    m_InsIndex++;
}

void LayerStack::Pop(Layer* layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_InsIndex, layer);
    if (it != m_Layers.begin() + m_InsIndex)
    {
        layer->onDetach();
        m_Layers.erase(it);
        m_InsIndex--;
    }
}