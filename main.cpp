#include <iostream>

#include "src/graphics/window.hpp"
#include "src/graphics/context.hpp"



int main(int, char**) 
{
    ref<Graphics::Context> ctx = Graphics::Context::GetInstance();
    Graphics::Window* window = new Graphics::Window();
    // Graphics::Window* window2 = new Graphics::Window();
    
    ctx->PushWindow(window);
    // ctx->PushWindow(window2);
    ctx->loop();
}
