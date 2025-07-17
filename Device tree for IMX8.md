# Device Tree for IMX8

## Overview

The device tree file used for the **i.MX8-based helmet** is:

```
imx8mp-var-dart-dt8mcustomboard3.dtb
```

This file is located in the directory:

```
C:\Users\user\Desktop\mx8_image_file\my_camera_project_v8
```

It should be placed in the following location on the target system:

```
/boot/
```

---

## Device Tree Manipulation

### Convert DTB to DTS (Decompile)

To inspect the contents of the `.dtb` file, convert it to a readable `.dts` format:

```bash
dtc -I dtb -O dts -o output.dts input.dtb
```

### Convert DTS back to DTB

After modifying the `.dts` file, recompile it into a `.dtb` file:

```bash
dtc -I dts -O dtb -o output.dtb input.dts
```

---

## Key Nodes in the Device Tree

| Component              | Node / Path                                               | Description                                      |
|------------------------|-----------------------------------------------------------|--------------------------------------------------|
| Camera                 | `ov5640_mipi`                                            | MIPI camera sensor configuration                |
| Ethernet               | `ethernet@30bf0000`                                      | Ethernet controller settings                    |
| LVDS Display           | `lvds-channel@0`, `phy@32ec0128`                         | LVDS screen configuration                       |

---

## Notes from Testing

- The current `imx8mp-var-dart-dt8mcustomboard3.dtb` works well on the testbench with non-8GB RAM configurations.
- When using the same DTB with an i.MX8 (8GB RAM version) on the testbench, **Wi-Fi does not function properly**.
- It is recommended to compare this DTB with the default:
  
  ```
  imx8mp-var-dart-dt8mcustomboard.dtb
  ```

  This will help identify differences that may affect hardware compatibility.

---

## Areas for Further Investigation

- **GPU/NPU/VPU configuration**: These components are present in the i.MX8 SoC but have not been fully explored or configured in the current device tree.
- **Variscite Documentation**: Refer to Variscite's official documentation and support resources to better understand hardware-specific device tree entries.

---