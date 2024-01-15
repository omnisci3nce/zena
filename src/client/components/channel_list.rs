use egui::Ui;

use crate::model::Channel;

pub fn draw_channel_list(ui: &mut Ui, channels: &[Channel]) {
    channels.iter().for_each(|ch| {
        ui.label(format!("Channel {}: {}", ch.id, ch.name));
    });
}
