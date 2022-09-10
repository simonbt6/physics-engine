#pragma once

#include "layer.hpp"

namespace Graphics
{
    class GUI : public Layer
    {
    private:
        bool show_demo_window = true;
        bool show_another_window = true;
        
    public:
        virtual void onAttach(GLFWwindow*) override;
        virtual void onDetach() override;
        virtual void onEvent() override;
        virtual void onUpdate() override;

        void begin();
        void end();
    };
};