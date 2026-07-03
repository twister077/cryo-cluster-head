#[derive(Debug, Clone, Copy)]
pub struct SensorReading {
    pub temperature: f64,
    pub battery_mv: u16,
    pub sequence: u8,
    pub status: u8,
}

pub fn parse_ble_advertisement(data: &[u8]) -> Option<SensorReading> {
    if data.len() < 7 {
        return None;
    }

    let temp_raw = ((data[2] as u16) << 8) | data[3] as u16;
    let temperature = temp_raw as f64 / 100.0;

    let battery_mv = ((data[4] as u16) * 10) as u16;
    let sequence = data[5];
    let status = data[6];

    Some(SensorReading {
        temperature,
        battery_mv,
        sequence,
        status,
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_valid_advertisement() {
        let data = vec![0x59, 0x00, 0x37, 0x0C, 0x2C, 0x01, 0x00];
        let reading = parse_ble_advertisement(&data).unwrap();
        assert!((reading.temperature - 37.12).abs() < 0.01);
        assert_eq!(reading.battery_mv, 3000);
        assert_eq!(reading.sequence, 1);
        assert_eq!(reading.status, 0);
    }

    #[test]
    fn test_parse_too_short() {
        assert!(parse_ble_advertisement(&[0x01, 0x02]).is_none());
    }
}
