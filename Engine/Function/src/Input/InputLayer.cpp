#include "InputLayer.hpp"

void Engine::InputLayer::OnEvent(Event &event)
{
    PROFILE_FUNCTION();

    if (!event.IsHandled())
    {
        Engine::EventDispatcher dispatcher(event);

        // KeyEvent
        dispatcher.Dispatch<Engine::KeyEvent>([this](Engine::KeyEvent &event) {
            Engine::Input::GetInstance().SetKeyAction(event.GetCode(), event.GetAction());
            return false;
        });

        // MouseEvent
        {
            dispatcher.Dispatch<Engine::MouseButtonEvent>([this](Engine::MouseButtonEvent &event) {
                Engine::Input::GetInstance().SetKeyAction(event.GetCode(), event.GetAction());
                return false;
            });
            dispatcher.Dispatch<Engine::MouseMoveEvent>([this](Engine::MouseMoveEvent &event) {
                Engine::Input::GetInstance().SetPosition(event.GetX(), event.GetY());
                return false;
            });
            dispatcher.Dispatch<Engine::MouseScrollEvent>([this](Engine::MouseScrollEvent &event) {
                Engine::Input::GetInstance().SetScrollOffset(event.GetXOffset(), event.GetYOffset());
                return false;
            });
        }
    }
}

void Engine::InputLayer::OnPostUpdate()
{
    PROFILE_FUNCTION();

    Engine::Input::GetInstance().Clear();
}