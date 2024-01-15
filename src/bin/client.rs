//! Entrypoint for running the Client

#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")] // hide console window on Windows in release

use eframe::egui;
use zena::{client::components::channel_list::draw_channel_list, db::Db};

fn main() -> Result<(), eframe::Error> {
    pretty_env_logger::init(); // Log to stderr (if you run with `RUST_LOG=debug`).
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default().with_inner_size([320.0, 240.0]),
        ..Default::default()
    };
    eframe::run_native("Zena", options, Box::new(|_cc| Box::<MyApp>::default()))
}

struct MyApp {
    name: String,
    db: Db,
}

impl Default for MyApp {
    fn default() -> Self {
        let db = Db::init().unwrap(); // TODO: handle error
        Self {
            name: "General".to_owned(),
            db,
        }
    }
}

impl eframe::App for MyApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        let channels = self.db.get_all_channels().unwrap();

        egui::CentralPanel::default().show(ctx, |ui| {
            ui.heading("Zena Client");
            ui.horizontal(|ui| {
                let name_label = ui.label("Channel name: ");
                ui.text_edit_singleline(&mut self.name)
                    .labelled_by(name_label.id);
            });
            if ui.button("Click to create new channel").clicked() {
                let _ = self.db.create_channel(self.name.clone());
            }
            ui.spacing();
            ui.heading("Channels");

            draw_channel_list(ui, &channels);
        });
    }
}
