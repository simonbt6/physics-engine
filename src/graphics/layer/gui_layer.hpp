#pragma once

#include "layer.hpp"
#include <unordered_map>

namespace Graphics
{
    enum class DockPosition
    {
        TOP, BOTTOM,
        LEFT, RIGHT,
        CENTER
    };

    class GUI : public Layer
    {
    private:
        std::unordered_map<std::string, DockPosition> m_DockQueue;
        
    public:
        virtual void onAttach(GLFWwindow*) override;
        virtual void onDetach() override;
        virtual void onEvent() override;
        virtual void onUpdate() override;

        virtual void dockWindow(const std::string& window, DockPosition position);

        void begin();
        void end();

    private:
        void _resetDocking();
    };
};