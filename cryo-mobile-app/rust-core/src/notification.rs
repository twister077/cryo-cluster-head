#[repr(i32)]
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum Alert {
    None = 0,
    Warning = 1,
    Critical = 2,
}

impl Alert {
    pub fn is_active(self) -> bool {
        matches!(self, Alert::Warning | Alert::Critical)
    }

    pub fn should_notify(self) -> bool {
        matches!(self, Alert::Warning)
    }
}
