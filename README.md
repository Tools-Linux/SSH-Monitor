# SSH Monitor

SSH Monitor is a lightweight security tool that monitors SSH connections in real time and sends Discord webhook notifications whenever an unauthorized user connects to your server.

It is designed to help system administrators quickly detect suspicious SSH activity and respond to potential unauthorized access.

---

## Features

* Real-time SSH connection monitoring
* Whitelist-based authorization system
* Instant Discord webhook notifications
* Automatic detection of unauthorized SSH logins
* Lightweight and easy to deploy
* Simple configuration

---

## Requirements

* Linux
* Bash
* curl
* A Discord Webhook URL

---

## Installation

SSH Monitor uses a single installation script to **install, update, reinstall, or uninstall** the application.

Run the following command:

```bash
bash <(curl -fsSL https://raw.githubusercontent.com/Docker-Update/SSH-Monitor/main/install.sh)
```

The installer will automatically display the available options.

---

## How It Works

SSH Monitor continuously monitors SSH authentication events on your server.

Whenever a user connects through SSH:

1. The username or IP address is checked against the configured whitelist.
2. If the connection is authorized, no action is taken.
3. If the connection is not whitelisted, SSH Monitor immediately sends a Discord webhook notification containing the connection details.

This allows administrators to detect and respond to unauthorized access in real time.

---

## Discord Notifications

When an unauthorized connection is detected, SSH Monitor can send information such as:

* Username
* Source IP address
* Date and time
* Hostname
* Server name

The notification format may vary depending on your configuration.

---

## Whitelist

Only users or IP addresses added to the whitelist are considered trusted.

Any connection from an unknown user or IP address will automatically trigger a Discord alert.

---

## Repository

GitHub Repository

https://github.com/Docker-Update/SSH-Monitor

Report Issues

https://github.com/Docker-Update/SSH-Monitor/issues

---

## License

This project is licensed under the MIT License.

https://github.com/Docker-Update/SSH-Monitor/blob/main/LICENSE
