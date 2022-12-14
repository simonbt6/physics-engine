#include <iostream>
#include "../../src/graphics/window.hpp"
#include "../../src/graphics/context.hpp"

#include "sandbox_layer.hpp"

int main()
{
    ref<Graphics::Context> ctx = Graphics::Context::GetInstance();
    Graphics::Window* window = new Graphics::Window();
    // window->setFullScreen(true);
    // Graphics::Window* window2 = new Graphics::Window();
    window->PushLayer(new Test::SandboxLayer());
    
    ctx->PushWindow(window);
    // ctx->PushWindow(window2);
    ctx->loop();
}