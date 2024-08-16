#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

#include "widgets/player_widget.hpp"

using namespace std;
using namespace ftxui;


int main() {
    // Components
    ScreenInteractive screen = ScreenInteractive::FixedSize(100, 10);

    using namespace widgets;
    auto widget = widgets::PlayerWidget();


    screen.Loop(
        Renderer(widget,
        [&] {
            return window(text(""), widget->Render());
        }
        )
    );

    
    return 0;
}