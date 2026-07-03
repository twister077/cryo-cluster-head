import 'package:flutter_blue_plus/flutter_blue_plus.dart';

class CoolerControlService {
  static final Guid _serviceUuid = Guid('0000cryo-0000-1000-8000-00805f9b34fb');
  static final Guid _modeCharUuid = Guid('0000cry1-0000-1000-8000-00805f9b34fb');
  static final Guid _targetCharUuid = Guid('0000cry2-0000-1000-8000-00805f9b34fb');

  BluetoothDevice? _device;
  BTCharacteristic? _modeChar;
  BTCharacteristic? _targetChar;

  bool get isConnected => _device != null;

  Future<bool> connect(BluetoothDevice device) async {
    await device.connect();
    _device = device;

    final services = await device.discoverServices();
    for (final s in services) {
      if (s.uuid == _serviceUuid) {
        for (final c in s.characteristics) {
          if (c.uuid == _modeCharUuid) _modeChar = c;
          if (c.uuid == _targetCharUuid) _targetChar = c;
        }
      }
    }

    return _modeChar != null && _targetChar != null;
  }

  Future<void> setMode(int mode) async {
    if (_modeChar == null) return;
    await _modeChar!.write([mode]);
  }

  Future<void> setTargetTemp(int celsius) async {
    if (_targetChar == null) return;
    await _targetChar!.write([celsius.clamp(10, 25)]);
  }

  void disconnect() {
    _device?.disconnect();
    _device = null;
    _modeChar = null;
    _targetChar = null;
  }
}
