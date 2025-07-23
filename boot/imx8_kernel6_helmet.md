# Device Tree Source (DTS) Documentation for Variscite DART-MX8M-PLUS

This documentation explains the device tree source file `imx8_kernel6_helmet.dts`, which describes the hardware layout for the Variscite DART-MX8M-PLUS platform, which utilizes the NXP i.MX 8M Plus SoC (System on Chip). The device tree structure allows the operating system to understand the hardware configurations and properties of the system.

## File Header

```dts
/dts-v1/;
```
- **/dts-v1/**: Indicates this file is a Device Tree Source (DTS) file, version 1.

## Root Node

```dts
/ {
	interrupt-parent = <0x01>;
	#address-cells = <0x02>;
	#size-cells = <0x02>;
	compatible = "variscite,imx8mp-var-dart\0fsl,imx8mp";
	model = "Variscite DART-MX8M-PLUS on DT8MCustomBoard 2.x";
```

- **Interrupt Parent**: `interrupt-parent` specifies the parent interrupt controller.
- **Address Cells**: The `#address-cells` and `#size-cells` properties define how address and size properties are represented in child nodes.
- **Compatible**: A list of strings for compatibility. The first string indicates the specific product, and the second indicates the general family.
- **Model**: Provides a human-readable model name for the hardware.

### Aliases

The `aliases` node provides shorthand references for various hardware components:

```dts
	aliases {
		ethernet0 = "/soc@0/bus@30800000/ethernet@30bf0000";
		i2c0 = "/soc@0/bus@30800000/i2c@30a20000";
		gpio0 = "/soc@0/bus@30000000/gpio@30200000";
	    ...
	};
```

- Each entry maps a logical name (e.g., `ethernet0`) to its full device tree path, allowing simpler references throughout the device tree.

### CPU Nodes

Details about the CPU cores:

```dts
	cpus {
		#address-cells = <0x01>;
		#size-cells = <0x00>;

		idle-states {
			entry-method = "psci"; // Power State Coordination Interface
			cpu-pd-wait {
				compatible = "arm,idle-state";
				...
			};
		};

		cpu@0 {
			device_type = "cpu";
			compatible = "arm,cortex-a53";
			reg = <0x00>; // Index of CPU
			...
		};

		// Additional CPUs (cpu@1, cpu@2, cpu@3) defined similarly.
	};
```

- Each `cpu@[0-3]` node gives properties relevant to the ARM Cortex-A53 cores, including cache sizes, idle states, and other CPU-specific attributes.
- The idle states node configures how the CPU enters lower power states when idle.

### Display Subsystem

```dts
	display-subsystem {
		compatible = "fsl,imx-display-subsystem";
		ports = <0x08 0x09 0x0a>;
	};
```

- Defines the display subsystem, with ports referenced for output connections.

### Memory Configuration

```dts
	memory@40000000 {
		device_type = "memory";
		reg = <0x00 0x40000000 0x00 0xc0000000 0x01 0x00 0x00 0xc0000000>;
	};
```

- The `memory` node specifies the starting address and size of the accessible memory.

### Peripheral Devices

The device tree outlines multiple peripheral devices, including:

- **GPIO Controllers**: Configured under the `bus@30000000` node with various GPIO settings and ranges.
- **I2C, SPI, UART**: These controllers are defined with their respective addresses, interrupts, and clock configurations.
  
Each peripheral node might contain:
- `compatible`: Denotes the type of device.
- `reg`: Address and size of the device.
- `interrupts`: Interrupt configurations.
- `clocks`: Associated clock definitions.

### Power Management

Power domains and management settings are specified as follows:

```dts
	power-domains {
		compatible = "simple-bus";

		audiomix-pd {
			compatible = "fsl,imx8m-pm-domain";
			...
		};
	};
```

- This section defines different power domains which can be turned off or on as needed, contributing to energy efficiency.

### Clock Definitions

Several clocks are defined which control the clock frequencies for various components:

```dts
	clock-osc-32k {
		compatible = "fixed-clock";
		#clock-cells = <0x00>;
		clock-frequency = <0x8000>;
		clock-output-names = "osc_32k";
	};
```

### Thermal Zones

Temperature sensors and management settings are defined to ensure the device can monitor and react to overheating:

```dts
	thermal-zones {
		cpu-thermal {
			...
		};

		soc-thermal {
			...
		};
	};
```

### Backlight and Display Parameters

Display configurations, including backlight settings and panel specifications, are also contained in the device tree:

```dts
	backlight {
		compatible = "pwm-backlight";
		pwms = <0xa5 0x00 0xf4240 0x00>;
		...
	};

	panel {
		compatible = "sgd,gktw70sdae4se\0panel-lvds";
		...
	};
```

### Key Assignments

GPIO keys such as buttons are defined with their respective labels and interrupts:

```dts
	gpio-keys {
		compatible = "gpio-keys";
		status = "okay";

		back {
			label = "Back";
			...
		};
	};
```

### Summary

This `imx8_kernel6_helmet.dts` file provides a comprehensive description of the hardware configuration for the Variscite DART-MX8M-PLUS platform, detailing CPU configurations, peripheral devices, memory layout, power domains, clock definitions, thermal management, and user input devices, enabling the Linux operating system to appropriately interface with the hardware.

---