#!/bin/bash

# Exit on any error
set -e

# Check if Python3 is installed, install if not
if ! command -v python3 &> /dev/null; then
    echo "Python3 not found. Installing..."
    sudo apt-get update
    sudo apt-get install -y python3 python3-pip
else
    echo "Python3 already installed."
fi

# Upgrade pip to the latest version
python3 -m pip install --upgrade pip

# Install required Python packages if not already installed
echo "Installing Python dependencies..."
pip3 install --user picamera io logging socketserver server serial websockets asyncio argparse

# Create Programs directory if it doesn't exist
mkdir -p ~/Programs
cd ~/Programs

# Download the Python scripts
echo "Downloading camera.py and server.py..."
wget -O camera.py https://raw.githubusercontent.com/NotPunchnox/hexa/refs/heads/main/software/Raspberry-pi-zero/camera.py
wget -O server.py https://raw.githubusercontent.com/NotPunchnox/hexa/refs/heads/main/software/Raspberry-pi-zero/server.py

# Create systemd service file for aash_vision
echo "Creating aash_vision.service..."
sudo bash -c 'cat > /etc/systemd/system/aash_vision.service << EOF
[Unit]
Description=Aash Vision Camera Stream
After=network.target

[Service]
ExecStart=/usr/bin/python3 /home/admin/Programs/camera.py
WorkingDirectory=/home/admin/Programs
User=admin
Environment="PATH=/home/admin/.local/bin:/usr/bin:/bin"
Restart=on-failure

[Install]
WantedBy=multi-user.target
EOF'

# Create systemd service file for aash_server
echo "Creating aash_server.service..."
sudo bash -c 'cat > /etc/systemd/system/aash_server.service << EOF
[Unit]
Description=Aash Command Websocket
After=network.target

[Service]
ExecStart=/usr/bin/python3 /home/admin/Programs/server.py
WorkingDirectory=/home/admin/Programs
User=admin
Environment="PATH=/home/admin/.local/bin:/usr/bin:/bin"
Restart=on-failure

[Install]
WantedBy=multi-user.target
EOF'

# Reload systemd daemon
sudo systemctl daemon-reload

# Enable and start aash_vision service
echo "Enabling and starting aash_vision.service..."
sudo systemctl enable aash_vision.service
sudo systemctl start aash_vision.service
sudo systemctl status aash_vision.service

# Enable and start aash_server service
echo "Enabling and starting aash_server.service..."
sudo systemctl enable aash_server.service
sudo systemctl start aash_server.service
sudo systemctl status aash_server.service

echo "Setup completed successfully!"
