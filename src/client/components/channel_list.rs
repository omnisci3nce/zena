use egui::{RichText, Ui};

use crate::model::Channel;

pub fn draw_channel_list(ui: &mut Ui, channels: &[Channel], selected_channel: Option<usize>) {
    channels.iter().enumerate().for_each(|(idx, ch)| {
        let text = format!("Channel {}: {}", ch.id, ch.name);
        match selected_channel {
            Some(i) if i == idx => ui.label(RichText::new(text).strong()),
            _ => ui.label(text),
        };
    });
}
