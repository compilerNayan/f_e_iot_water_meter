#!/usr/bin/env python3
"""
Generate C++ preprocessor macros from device_config.ini
Reads the INI file and outputs compiler flags (-D macros)
"""

import configparser
import sys
import os
from pathlib import Path

def parse_device_config(ini_path):
    """Parse device_config.ini and return device configuration entries."""
    config = configparser.ConfigParser()

    if not os.path.exists(ini_path):
        print(f"Warning: {ini_path} not found. No device macros will be generated.", file=sys.stderr)
        return [], None, None, None

    config.read(ini_path)
    devices = []
    valve_control_pin = None
    flow_pulse_pin = None
    flow_pulses_per_liter = None

    for section_name in config.sections():
        if section_name.startswith('Switch_'):
            try:
                device_id = int(section_name.replace('Switch_', ''))
                relay_pin = config.getint(section_name, 'relay_pin')
                physical_pin = config.getint(section_name, 'physical_pin')
                devices.append(('switch', device_id, relay_pin, physical_pin))
            except (ValueError, configparser.NoOptionError) as e:
                print(f"Warning: Skipping invalid section {section_name}: {e}", file=sys.stderr)
                continue
        elif section_name == 'Valve':
            try:
                valve_control_pin = config.getint(section_name, 'control_pin')
            except (ValueError, configparser.NoOptionError) as e:
                print(f"Warning: Skipping invalid section {section_name}: {e}", file=sys.stderr)
        elif section_name == 'FlowSensor':
            try:
                flow_pulse_pin = config.getint(section_name, 'pulse_pin')
                flow_pulses_per_liter = config.getint(section_name, 'pulses_per_liter')
            except (ValueError, configparser.NoOptionError) as e:
                print(f"Warning: Skipping invalid section {section_name}: {e}", file=sys.stderr)

    return sorted(devices, key=lambda x: x[1]), valve_control_pin, flow_pulse_pin, flow_pulses_per_liter

def generate_macro_flags(devices, valve_control_pin, flow_pulse_pin, flow_pulses_per_liter, format='cmake'):
    """Generate compiler flags (-D macros) from device configurations."""
    flags = []
    for device_type, device_id, relay_pin, physical_pin in devices:
        if device_type == 'switch':
            flags.append(f"-DSWITCH_{device_id}_RELAY_PIN_INDEX={relay_pin}")
            flags.append(f"-DSWITCH_{device_id}_PHYSICAL_DEVICE_PIN_INDEX={physical_pin}")

    if valve_control_pin is not None:
        flags.append(f"-DVALVE_CONTROL_PIN_INDEX={valve_control_pin}")

    if flow_pulse_pin is not None:
        flags.append(f"-DFLOW_SENSOR_PULSE_PIN={flow_pulse_pin}")

    if flow_pulses_per_liter is not None:
        flags.append(f"-DFLOW_SENSOR_PULSES_PER_LITER={flow_pulses_per_liter}")

    return flags

def main():
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    ini_path = project_root / 'device_config.ini'

    format_type = sys.argv[1] if len(sys.argv) > 1 else 'cmake'

    devices, valve_control_pin, flow_pulse_pin, flow_pulses_per_liter = parse_device_config(ini_path)
    flags = generate_macro_flags(
        devices, valve_control_pin, flow_pulse_pin, flow_pulses_per_liter, format=format_type)

    if format_type == 'pio':
        print('\n'.join(flags))
    else:
        print(' '.join(flags))

if __name__ == '__main__':
    main()
