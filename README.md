# SSH Monitor

SSH Monitor is a lightweight security tool that monitors SSH connections in real time and sends Discord webhook notifications whenever an unauthorized user connects to your server.

It is designed to help system administrators quickly detect suspicious SSH activity and respond to potential unauthorized access.

---

## Features

- Real-time SSH connection monitoring
- Whitelist-based authorization system
- Instant Discord webhook notifications
- Automatic detection of unauthorized SSH logins
- Lightweight and easy to deploy
- Simple CLI management
- Easy configuration

---

## Requirements

- Linux
- Bash
- systemd
- jq
- curl
- A Discord Webhook URL

---

## Installation

SSH Monitor uses a single installation script to **install, update, reinstall, or uninstall** the application.

Run:

```bash
bash <(curl -fsSL https://raw.githubusercontent.com/Tools-Linux/SSH-Monitor/master/install.sh)
```

The installer will automatically display the available options.

---

## Configuration

The configuration file is located at:

```text
/opt/ssh-monitor/config.json
```

It contains your Discord webhook URL and the whitelist of authorized IP addresses.

---

## Command Line Interface

SSH Monitor installs the `sshmonitor` command.

### Service Management

Start the service:

```bash
sshmonitor start
```

Stop the service:

```bash
sshmonitor stop
```

Restart the service:

```bash
sshmonitor restart
```

Show service status:

```bash
sshmonitor status
```

---

### Whitelist Management

Add an IP address:

```bash
sshmonitor add <ip>
```

Example:

```bash
sshmonitor add 192.168.1.100
```

Remove an IP address:

```bash
sshmonitor remove <ip>
```

Example:

```bash
sshmonitor remove 192.168.1.100
```

Display the whitelist:

```bash
sshmonitor list
```

---

## How It Works

SSH Monitor continuously monitors SSH authentication events on your server.

Whenever a user connects through SSH:

1. The source IP address is checked against the configured whitelist.
2. If the IP is authorized, no action is taken.
3. If the IP is not whitelisted, SSH Monitor immediately sends a Discord webhook notification containing the connection details.

This allows administrators to detect and respond to unauthorized access in real time.

---

## Discord Notifications

When an unauthorized connection is detected, SSH Monitor can send information such as:

- Username
- Source IP address
- Date and time
- Hostname
- Server name

The notification format may vary depending on your configuration.

---

## Repository

GitHub Repository

https://github.com/Tools-Linux/SSH-Monitor

Report Issues

https://github.com/Tools-Linux/SSH-Monitor/issues

---

## License

This project is licensed under the MIT License.

https://github.com/Tools-Linux/SSH-Monitor/blob/master/LICENSE
