#pragma once

#include "../def.hpp"
#include "window.hpp"

namespace Graphics 
{
    class Context 
    {
    private:
        Context();
        
        std::vector<Window*> m_Windows;

    public:
        ~Context();
             
        /**
         * @brief Initialization of the context.
         * 
         */
        void init();

        /**
         * @brief Termination of the context.
         * 
         */
        void terminate();

        void loop();

        void PushWindow(Window* window);
        void PopWindow(Window* window);

    public:

        /**
         * @brief Singleton for the context class.
         * 
         * @return ref<Context> 
         */
        static ref<Context> GetInstance();

    };
};