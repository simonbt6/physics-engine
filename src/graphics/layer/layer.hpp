#pragma once

#include "../../def.hpp"

struct GLFWwindow;

namespace Graphics
{
    class Layer 
    {
    protected:
        std::string m_Name;

    public:
        Layer(const std::string& name = "Default Layer Name"): m_Name(name) {}
        virtual ~Layer() = default;

        virtual void onAttach(GLFWwindow*) {}
        virtual void onDetach() {}
        virtual void onUpdate() {}
        virtual void onEvent() {}

    };
}