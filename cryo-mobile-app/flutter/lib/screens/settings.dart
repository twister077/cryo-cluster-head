import 'package:flutter/material.dart';

class SettingsScreen extends StatefulWidget {
  const SettingsScreen({super.key});

  @override
  State<SettingsScreen> createState() => _SettingsScreenState();
}

class _SettingsScreenState extends State<SettingsScreen> {
  double _threshold = 0.5;
  int _windowSize = 5;
  bool _notificationsEnabled = true;

  int _coolerMode = 0;
  double _targetTemp = 15.0;
  final List<String> _coolerModes = ['Off', '1', '2', '3', 'Auto'];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Settings')),
      body: ListView(
        children: [
          const SizedBox(height: 8),
          _section(
            context,
            'Flare-up Detection',
            children: [
              ListTile(
                title: const Text('dT/dt Threshold'),
                subtitle: Text('${_threshold.toStringAsFixed(1)}°C/min'),
                trailing: SizedBox(
                  width: 200,
                  child: Slider(
                    value: _threshold,
                    min: 0.1,
                    max: 2.0,
                    divisions: 19,
                    onChanged: (v) => setState(() => _threshold = v),
                  ),
                ),
              ),
              ListTile(
                title: const Text('Window Size'),
                subtitle: Text('$_windowSize samples'),
                trailing: SizedBox(
                  width: 200,
                  child: Slider(
                    value: _windowSize.toDouble(),
                    min: 3,
                    max: 20,
                    divisions: 17,
                    onChanged: (v) => setState(() => _windowSize = v.toInt()),
                  ),
                ),
              ),
            ],
          ),
          _section(
            context,
            'Notifications',
            children: [
              SwitchListTile(
                title: const Text('Push Notifications'),
                subtitle: const Text('Alert on rapid temperature rise'),
                value: _notificationsEnabled,
                onChanged: (v) => setState(() => _notificationsEnabled = v),
              ),
            ],
          ),
          _section(
            context,
            'Active Cooler',
            children: [
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  children: List.generate(_coolerModes.length, (i) {
                    final selected = _coolerMode == i;
                    return ChoiceChip(
                      label: Text(_coolerModes[i]),
                      selected: selected,
                      onSelected: (v) {
                        if (v) setState(() => _coolerMode = i);
                      },
                    );
                  }),
                ),
              ),
              if (_coolerMode == 4)
                ListTile(
                  title: const Text('Target Temperature'),
                  subtitle: Text('${_targetTemp.toStringAsFixed(0)}°C'),
                  trailing: SizedBox(
                    width: 200,
                    child: Slider(
                      value: _targetTemp,
                      min: 10,
                      max: 25,
                      divisions: 15,
                      onChanged: (v) => setState(() => _targetTemp = v),
                    ),
                  ),
                ),
              ListTile(
                title: const Text('Status'),
                subtitle: Text(_coolerMode == 0
                    ? 'Off'
                    : _coolerMode == 4
                        ? 'Auto — target ${_targetTemp.toStringAsFixed(0)}°C'
                        : 'Level ${_coolerModes[_coolerMode]}'),
                leading: Icon(
                  _coolerMode == 0 ? Icons.power_settings_new : Icons.ac_unit,
                ),
              ),
            ],
          ),
          _section(
            context,
            'About',
            children: [
              ListTile(
                title: const Text('Patch'),
                subtitle: const Text('Not connected'),
                leading: const Icon(Icons.bluetooth_disabled),
              ),
              ListTile(
                title: const Text('Version'),
                subtitle: const Text('0.1.0'),
                leading: const Icon(Icons.info_outline),
              ),
            ],
          ),
        ],
      ),
    );
  }

  Widget _section(
    BuildContext context,
    String title, {
    required List<Widget> children,
  }) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Padding(
          padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
          child: Text(
            title,
            style: Theme.of(context).textTheme.titleSmall?.copyWith(
                  color: Theme.of(context).colorScheme.primary,
                ),
          ),
        ),
        Card(
          margin: const EdgeInsets.symmetric(horizontal: 12, vertical: 4),
          child: Column(children: children),
        ),
        const SizedBox(height: 8),
      ],
    );
  }
}
