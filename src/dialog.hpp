#pragma once
#include <rack.hpp>
using namespace ::rack;

namespace widgetry {

struct DialogBase: OpaqueWidget
{
    using Base = OpaqueWidget;

    ModuleWidget * source{nullptr};

    DialogBase(ModuleWidget * src) : source(src) {}

    void onAction(const ActionEvent& e) override {
        e.unconsume(); // don't close menu overlay
    }

    void close() {
        MenuOverlay* overlay = getAncestorOfType<MenuOverlay>();
        if (overlay) {
            overlay->requestDelete();
        }
    }
};

template<typename TSvg>
struct SvgDialog: DialogBase
{
    ::rack::widget::FramebufferWidget* fb;
    ::rack::widget::SvgWidget* sw;

    SvgDialog(ModuleWidget* src) : DialogBase(src) {
        fb = new widget::FramebufferWidget;
        addChild(fb);
        sw = new widget::SvgWidget;
        fb->addChild(sw);

        set_svg(::rack::window::Svg::load(TSvg::background()));
    }

    void set_svg(std::shared_ptr<window::Svg> svg) {
        if (!sw->svg) {
            sw->setSvg(svg);
            box.size = sw->box.size;
            fb->box.size = sw->box.size;
            fb->setDirty();
        }
    }
};

struct TrackingZoom: ::rack::widget::ZoomWidget
{
    using Base = ::rack::widget::ZoomWidget;

    Widget * pseudo_parent{nullptr};
    Vec pseudo_pos;

    TrackingZoom(Widget *pseudo, Vec pos) : pseudo_parent(pseudo), pseudo_pos(pos) {}

    void step() override {
        Base::step();
        float current_zoom = APP->scene->rackScroll->getZoom();
        if (getZoom() != current_zoom) {
            setZoom(current_zoom);
        }
        auto abs_pos = pseudo_parent->getAbsoluteOffset(pseudo_pos);
        if (APP->scene->menuBar && APP->scene->menuBar->isVisible()) {
            abs_pos.y -= APP->scene->menuBar->box.size.y;
        }
        box.pos = APP->scene->getRelativeOffset(abs_pos, APP->scene->rackScroll);
    }
};

template <typename TDialog>
TDialog* createMinimalMenuDialog()
{
    TDialog* dialog = new TDialog();
    ::rack::createMenu()->addChild(dialog);
    return dialog;
}

template <typename TDialog>
TDialog* createDialog(ModuleWidget* source, Vec(pos), bool center = false, NVGcolor screen = NVGcolor{0})
{
    TDialog* dialog = new TDialog(source);
    if (center) {
        pos = pos.minus(dialog->box.size.div(2));
    }

    ui::MenuOverlay* menuOverlay = new ui::MenuOverlay;
    menuOverlay->bgColor = screen; // dim rack

    auto zoomer = new TrackingZoom(source, pos);
    menuOverlay->addChild(zoomer);
    zoomer->addChild(dialog);
    APP->scene->rackScroll->addChild(menuOverlay);

    return dialog;
}

template <typename TDialog>
TDialog* createMenuDialog(ModuleWidget* source, Vec(pos), bool center = false, NVGcolor screen = NVGcolor{0})
{
    TDialog* dialog = new TDialog(source);

    ::rack::Menu* menu = createMenu();
    auto overlay = menu->getAncestorOfType<MenuOverlay>();
    overlay->bgColor = screen;

    bool real_pos = pos.isFinite();
    if (real_pos) {
        menu->box.pos = source->getAbsoluteOffset(pos);
    }
    menu->addChild(dialog);
    if (real_pos && center) {
        menu->step(); // adjust size and position
        menu->box.pos = menu->box.pos.minus(menu->box.size.div(2));
    }
    return dialog;
}

}

/*
MIT License (MIT)

Copyright © 2025 Paul Chase Dempsey

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
