use crate::notification::Alert;
use crate::ble_parser::SensorReading;
use thiserror::Error;

#[derive(Error, Debug)]
pub enum DtdtError {
    #[error("detector not initialized")]
    NotInitialized,
    #[error("insufficient samples")]
    InsufficientSamples,
}

pub struct DtdtDetector {
    readings: Vec<SensorReading>,
    window_size: usize,
    threshold: f64,
    consecutive_triggers: u8,
    debounce_count: u8,
}

impl DtdtDetector {
    pub fn new(window_size: usize, threshold: f64) -> Self {
        Self {
            readings: Vec::with_capacity(window_size),
            window_size,
            threshold,
            consecutive_triggers: 0,
            debounce_count: 2,
        }
    }

    pub fn feed(&mut self, reading: SensorReading) -> Result<Alert, DtdtError> {
        self.readings.push(reading);
        if self.readings.len() > self.window_size {
            self.readings.remove(0);
        }

        if self.readings.len() < 3 {
            return Ok(Alert::None);
        }

        let slope = self.compute_slope();
        if slope > self.threshold {
            self.consecutive_triggers += 1;
            if self.consecutive_triggers >= self.debounce_count {
                self.consecutive_triggers = 0;
                Ok(Alert::Warning)
            } else {
                Ok(Alert::None)
            }
        } else {
            self.consecutive_triggers = 0;
            Ok(Alert::None)
        }
    }

    fn compute_slope(&self) -> f64 {
        let n = self.readings.len() as f64;
        let mut sum_x = 0.0;
        let mut sum_y = 0.0;
        let mut sum_xy = 0.0;
        let mut sum_xx = 0.0;

        for (i, r) in self.readings.iter().enumerate() {
            let x = i as f64;
            let y = r.temperature;
            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_xx += x * x;
        }

        let denom = n * sum_xx - sum_x * sum_x;
        if denom.abs() < 1e-10 {
            return 0.0;
        }

        (n * sum_xy - sum_x * sum_y) / denom
    }

    pub fn reset(&mut self) {
        self.readings.clear();
        self.consecutive_triggers = 0;
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_no_alert_on_stable_temp() {
        let mut det = DtdtDetector::new(5, 0.5);
        for _ in 0..10 {
            let r = SensorReading {
                temperature: 37.0,
                battery_mv: 3000,
                sequence: 0,
                status: 0,
            };
            assert!(matches!(det.feed(r).unwrap(), Alert::None));
        }
    }

    #[test]
    fn test_alert_on_rapid_spike() {
        let mut det = DtdtDetector::new(5, 0.3);
        let temps = vec![36.8, 36.9, 37.2, 37.6, 38.1, 38.5];
        let mut alerts = 0;
        for t in temps {
            let r = SensorReading {
                temperature: t,
                battery_mv: 3000,
                sequence: 0,
                status: 0,
            };
            if matches!(det.feed(r).unwrap(), Alert::Warning) {
                alerts += 1;
            }
        }
        assert!(alerts > 0);
    }
}
