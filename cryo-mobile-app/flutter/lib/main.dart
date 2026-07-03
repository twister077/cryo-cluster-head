import 'package:flutter/material.dart';
import 'screens/dashboard.dart';
import 'screens/history.dart';
import 'screens/settings.dart';

void main() {
  runApp(const CryoApp());
}

class CryoApp extends StatelessWidget {
  const CryoApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'CryoClusterHead',
      theme: ThemeData.dark(useMaterial3: true).copyWith(
        colorSchemeSeed: const Color(0xFF00BCD4),
      ),
      home: const HomeScreen(),
    );
  }
}

class HomeScreen extends StatefulWidget {
  const HomeScreen({super.key});

  @override
  State<HomeScreen> createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  int _selectedIndex = 0;

  static const List<Widget> _screens = [
    DashboardScreen(),
    HistoryScreen(),
    SettingsScreen(),
  ];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: _screens[_selectedIndex],
      bottomNavigationBar: NavigationBar(
        selectedIndex: _selectedIndex,
        onDestinationSelected: (i) => setState(() => _selectedIndex = i),
        destinations: const [
          NavigationDestination(icon: Icon(Icons.thermostat), label: 'Live'),
          NavigationDestination(icon: Icon(Icons.history), label: 'History'),
          NavigationDestination(icon: Icon(Icons.settings), label: 'Settings'),
        ],
      ),
    );
  }
}
