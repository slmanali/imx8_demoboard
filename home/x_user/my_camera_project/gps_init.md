# Documentation for `gps_init.sh`

## Overview

`gps_init.sh` is a shell script designed to initialize and configure a GPS device by downloading Extended Prediction Orbit (EPO) data, parsing it, resetting the GPS, injecting the current time into the GPS, and finally reading GPS data. This script specifically interacts with MediaTek GPS hardware and depends on a variety of external resources and binaries.

## Requirements

- Bash shell (version 4 or later is recommended).
- `wget` for downloading files.
- Parsing tools like the provided `epo_parser` and `gps_parser` scripts.
- Serial communication with GPS through `/dev/ttymxc1`.
- Permissions to run commands as superuser (using `sudo`).

## Script Breakdown

### Function Definitions

#### `nmea_checksum`

```bash
nmea_checksum() {
    local input="$1"
    local cksum=0
    for (( i=0; i<${#input}; i++ )); do
        cksum=$(( cksum ^ $(printf "%d" "'${input:$i:1}") ))
    done
    printf "%02X" $cksum
}
```

The `nmea_checksum` function calculates the NMEA checksum for a given string. The checksum is computed as the XOR of the ASCII values of the characters within the input string. Finally, the checksum is formatted as a two-digit hexadecimal value.

#### Main Script Execution

1. **Download EPO FILE**: 
   ```bash
   EPO=${1-MTK14.EPO}
   file=${2-$EPO}
   ```
   This section defines the default EPO filename as `MTK14.EPO`, which can be overridden by providing a different filename as the first argument when calling the script. The second argument can specify an alternative filename for the EPO file.

2. **Backup Existing EPO File**:
   ```bash
   if [ -e $file ] ; then
       mv $file $file.last
   fi
   ```
   If the specified EPO file already exists, it is renamed to create a backup named `<file>.last`.

3. **Downloading EPO File**:
   ```bash
   wget -O${file} http://wepodownload.mediatek.com/EPO_GPS_3_7.DAT -t 1 -T 10 || cp $file.last $file
   ```
   The script uses `wget` to download the EPO data file from a specified URL. It sets a timeout of 10 seconds for the connection and retries the download once if it fails. If the download fails, the script restores the backup file.

4. **Extracting Information from EPO**:
   ```bash
   echo "Parsing and Extracting Information From EPO"
   home/x_user/my_camera_project/epo_parser home/x_user/my_camera_project/MTK14.EPO 
   ```
   The script calls an external parser called `epo_parser` to process the downloaded EPO file.

5. **Resetting the GPS**:
   ```bash
   echo "Reseting The GPS"
   home/x_user/my_camera_project/gps_config
   ```
   It executes the `gps_config` command to reset the GPS device.

6. **Injecting Current Time into the GPS**:
   ```bash
   NOW=$(date +"%Y,%m,%d,%H,%M,%S")
   CMD="PMTK740,$NOW"
   CKSUM=$(nmea_checksum "$CMD")
   NMEA="\$$CMD*$CKSUM\r\n"
   echo -ne "$NMEA" > /dev/ttymxc1
   ```
   - The current date and time are formatted and composed into an NMEA command `PMTK740`.
   - The checksum for this command is calculated using the `nmea_checksum` function.
   - The full command (with proper formatting) is sent to the GPS device via the serial port `/dev/ttymxc1`.

7. **Uploading EPO Data**:
   ```bash
   sudo home/x_user/my_camera_project/epo_upload_demo
   ```
   This command runs another script, `epo_upload_demo`, with superuser privileges, presumably to upload the parsed EPO data to the GPS device.

8. **Reading GPS Data**:
   ```bash
   echo "Read GPS Data"
   home/x_user/my_camera_project/gps_parser
   ```
   Finally, the script runs another parser called `gps_parser` to read and potentially output GPS data for logging or debugging purposes.

### Additional Notes

- The script contains commented-out code for downloading GPS + GLONASS data, which can be activated by un-commenting the related line.
- The script may require additional error handling and logging in a production environment to handle unexpected situations during file downloads or device communication.
- Ensure the user running the script has permission to access `/dev/ttymxc1` and execute the necessary commands without issues.
