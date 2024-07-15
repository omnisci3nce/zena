//! Entrypoint for running the Client

// #![cfg_attr(not(debug_assertions), windows_subsystem = "windows")] // hide console window on Windows in release

use gpui::*;

struct HelloWorld {
    text: SharedString,
    count: usize
}

actions!(client, [
    IncrCount
]);

impl HelloWorld {
    fn onclick(&mut self, event: &MouseDownEvent, cx: &mut ViewContext<Self>) {
        self.count += 1;
        cx.notify();
        ()
    }
}

impl Render for HelloWorld {
    fn render(&mut self, cx: &mut ViewContext<Self>) -> impl IntoElement {
        div()
            .flex()
            .bg(rgb(0x2e7d32))
            .size(Length::Definite(DefiniteLength::Fraction(1.0)))
            // .size(Length::Definite(Pixels(300.0).into()))
            .justify_center()
            .items_center()
            .shadow_lg()
            .border_1()
            .border_color(rgb(0x0000ff))
            .text_xl()
            .text_color(rgb(0xffffff))
            .on_mouse_down(MouseButton::Left, cx.listener(Self::onclick))
            .child(
                format!("Count: {}!", &self.count)
            )
    }
}

fn main() {
    App::new().run(|cx: &mut AppContext| {
        let bounds = Bounds::centered(None, size(px(300.0), px(300.0)), cx);
        cx.open_window(
            WindowOptions {
                // window_bounds: Some(WindowBounds::Windowed(bounds)),
                ..Default::default()
            },
            |cx| {
                cx.new_view(|_cx| HelloWorld {
                    text: "from Zena".into(),
                    count: 0,
                })
            },
        )
        .unwrap();
    });
}