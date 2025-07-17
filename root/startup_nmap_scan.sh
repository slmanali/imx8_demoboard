#!/bin/bash
# Wait for the Internet connection on wlan0
while ! ip -4 addr show wlan0 | grep -q inet; do
    sleep 10  # Check every 10 seconds
done
# Function to perform nmap scan
perform_scan() {
    # Using --open to only show hosts that have at least one open port
	# Example pseudocode for deriving the network and performing a scan
	IP=$(ip -4 addr show wlan0 | grep -oP '(?<=inet\s)\d+(\.\d+){3}')
	NETMASK=$(ip -4 addr show wlan0 | awk '/inet / {split($2, a, "/"); print a[2]}')
	NETWORK=$(ipcalc -n $IP/$NETMASK | awk '/Network:/ {print $2}')
	echo $IP
	echo $NETWORK
	#nmap -sP $NETWORK
    result=$(sudo nmap -sP $NETWORK --open --min-hostgroup 100)
    echo "$result"
    # Check if any hosts were up
    if echo "$result" | grep -q "Host is up"; then
        echo "At least one host is up."
        return 0  # success
    else
        return 1  # failure
    fi
}
# Initial scan
perform_scan
# If no hosts were up, keep retrying
while [ $? -ne 0 ]; do
    echo "No hosts up, retrying in 10 seconds..."
    sleep 10
    perform_scan
done