import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';

class DashboardScreen extends StatefulWidget {
  const DashboardScreen({super.key});

  @override
  State<DashboardScreen> createState() => _DashboardScreenState();
}

class _DashboardScreenState extends State<DashboardScreen> {
  List<ScanResult> _devices = [];
  bool _scanning = false;
  ScanResult? _connected;

  @override
  void initState() {
    super.initState();
    FlutterBluePlus.setLogLevel(LogLevel.verbosity);
  }

  Future<void> _startScan() async {
    setState(() => _scanning = true);
    _devices.clear();

    await FlutterBluePlus.startScan(
      withKeywords: ['CryoPatch'],
      timeout: const Duration(seconds: 15),
    );

    var subscription = FlutterBluePlus.scanResults.listen((results) {
      setState(() => _devices = results);
    });

    await Future.delayed(const Duration(seconds: 15));
    await subscription.cancel();
    setState(() => _scanning = false);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('CryoClusterHead'),
        actions: [
          if (_connected != null)
            IconButton(
              icon: const Icon(Icons.bluetooth_connected),
              onPressed: () {},
            ),
        ],
      ),
      body: Column(
        children: [
          if (_connected == null) ...[
            Padding(
              padding: const EdgeInsets.all(16),
              child: FilledButton.icon(
                onPressed: _scanning ? null : _startScan,
                icon: _scanning
                    ? const SizedBox(
                        width: 16,
                        height: 16,
                        child: CircularProgressIndicator(strokeWidth: 2),
                      )
                    : const Icon(Icons.bluetooth_searching),
                label: Text(_scanning ? 'Scanning...' : 'Find Patch'),
              ),
            ),
            Expanded(
              child: ListView.builder(
                itemCount: _devices.length,
                itemBuilder: (context, i) {
                  final d = _devices[i];
                  return ListTile(
                    leading: const Icon(Icons.bluetooth),
                    title: Text(d.device.platformName.isNotEmpty
                        ? d.device.platformName
                        : 'Unknown'),
                    subtitle: Text('${d.rssi} dBm'),
                    trailing: FilledButton.tonal(
                      onPressed: () async {
                        await d.device.connect();
                        setState(() => _connected = d);
                      },
                      child: const Text('Connect'),
                    ),
                  );
                },
              ),
            ),
          ] else ...[
            Expanded(
              child: Center(
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Text(
                      '37.2°C',
                      style: Theme.of(context)
                          .textTheme
                          .displayLarge
                          ?.copyWith(fontSize: 72),
                    ),
                    const SizedBox(height: 8),
                    Text(
                      'dT/dt: +0.02°C/min',
                      style: Theme.of(context).textTheme.titleLarge,
                    ),
                    const SizedBox(height: 24),
                    Chip(
                      avatar: const Icon(Icons.check_circle, color: Colors.green),
                      label: const Text('Normal'),
                    ),
                    const SizedBox(height: 32),
                    SizedBox(
                      width: 200,
                      child: LinearProgressIndicator(
                        value: 0.3,
                        minHeight: 8,
                        borderRadius: BorderRadius.circular(4),
                      ),
                    ),
                    const SizedBox(height: 8),
                    const Text('Battery: 3.0V'),
                  ],
                ),
              ),
            ),
          ],
        ],
      ),
    );
  }
}
