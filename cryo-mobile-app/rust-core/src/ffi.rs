use crate::notification::Alert;
use crate::dtdt::DtdtDetector;
use std::sync::Mutex;

static DETECTOR: Mutex<Option<DtdtDetector>> = Mutex::new(None);

#[no_mangle]
pub extern "C" fn cryo_init(window_size: usize, threshold: f64) -> i32 {
    match DETECTOR.lock() {
        Ok(mut guard) => {
            *guard = Some(DtdtDetector::new(window_size, threshold));
            0
        }
        Err(_) => -1,
    }
}

#[no_mangle]
pub extern "C" fn cryo_process_temperature(temp_c: f64) -> i32 {
    let mut guard = match DETECTOR.lock() {
        Ok(g) => g,
        Err(_) => return -1,
    };
    let detector = match guard.as_mut() {
        Some(d) => d,
        None => return -2,
    };
    let reading = crate::ble_parser::SensorReading {
        temperature: temp_c,
        battery_mv: 0,
        sequence: 0,
        status: 0,
    };
    match detector.feed(reading) {
        Ok(alert) => alert as i32,
        Err(_) => -3,
    }
}

#[no_mangle]
pub extern "C" fn cryo_reset() -> i32 {
    match DETECTOR.lock() {
        Ok(mut guard) => {
            *guard = None;
            0
        }
        Err(_) => -1,
    }
}
