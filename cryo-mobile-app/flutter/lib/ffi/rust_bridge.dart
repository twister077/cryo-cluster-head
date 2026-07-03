import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:flutter/foundation.dart' show Platform;

final DynamicLibrary _native = Platform.isAndroid
    ? DynamicLibrary.open('libcryo_core.so')
    : DynamicLibrary.process();

typedef CryoInitNative = Int32 Function(Uint64 windowSize, Double threshold);
typedef CryoInitDart = int Function(int windowSize, double threshold);

typedef CryoProcessNative = Int32 Function(Double tempC);
typedef CryoProcessDart = int Function(double tempC);

typedef CryoResetNative = Int32 Function();
typedef CryoResetDart = int Function();

final CryoInitDart _cryoInit = _native
    .lookupFunction<CryoInitNative, CryoInitDart>('cryo_init');

final CryoProcessDart _cryoProcess = _native
    .lookupFunction<CryoProcessNative, CryoProcessDart>('cryo_process_temperature');

final CryoResetDart _cryoReset = _native
    .lookupFunction<CryoResetNative, CryoResetDart>('cryo_reset');

class RustBridge {
  int init(int windowSize, double threshold) => _cryoInit(windowSize, threshold);
  int processTemperature(double tempC) => _cryoProcess(tempC);
  int reset() => _cryoReset();
}
