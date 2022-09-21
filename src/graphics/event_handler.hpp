#pragma once

#include <unordered_set>
#include <vector>
#include <functional>
#include <variant>

namespace Graphics
{
    class Window;
    enum class EventType
    {
        INPUT_MOUSE_RIGHT_CLICK,
        INPUT_MOUSE_LEFT_CLICK,
        INPUT_MOUSE_MIDDLE_CLICK,

        INPUT_KEYBOARD_KEY_DOWN,
        INPUT_KEYBOARD_KEY_UP,

        WINDOW_CLOSE,
        WINDOW_UPDATE
    };

    typedef std::function<void(int key, int modifier)> KeyboardCallback;
    typedef std::function<void(int key)> MouseCallback;
    typedef std::function<void(void)> Callback;

    class EventHandler
    {
    private:
        std::unordered_set<EventType, std::vector<Callback>> m_EventCallbacks;

    public:
        void bind(EventType event, Callback callback);
        void unbind(EventType event, Callback callback);

    private:
        void call(EventType event);
        friend class Window;

    };
};