document.addEventListener('DOMContentLoaded', function() {
    const status = document.getElementById('status');

    async function loadSettings() {
        try {
            const response = await fetch('/settings');
            if (!response.ok) {
                throw new Error('Failed to load settings');
            }
            const data = await response.json();

            document.getElementById('ssid').value = data.wifi_ssid || '';
            document.getElementById('passphrase').value = data.wifi_passphrase || '';
            document.getElementById('keyboardLayout').value = data.keyboard_layout || 'FR';
        } catch (error) {
            console.error('Error loading settings:', error);
            status.textContent = 'Failed to load current settings';
            status.className = 'error';
        }
    }

    document.getElementById('rebootButton').addEventListener('click', async function() {
        const button = this;

        status.textContent = 'Rebooting...';
        status.className = '';
        button.disabled = true;

        try {
            const response = await fetch('/reboot', { method: 'POST' });
            
            if (response.ok) {
                status.textContent = 'Reboot command sent. Device will restart.';
                status.className = 'success';
            } else {
                const error = await response.text();
                status.textContent = 'Error: ' + error;
                status.className = 'error';
                button.disabled = false;
            }
        } catch (error) {
            status.textContent = 'Reboot command sent (device may restart).';
            status.className = 'success';
        }
    });

    document.getElementById('settingsForm').addEventListener('submit', async function(e) {
        e.preventDefault();

        const button = this.querySelector('button');
        status.textContent = 'Saving settings...';
        status.className = '';
        button.disabled = true;

        const payload = {
            wifi_ssid: document.getElementById('ssid').value,
            wifi_passphrase: document.getElementById('passphrase').value,
            keyboard_layout: document.getElementById('keyboardLayout').value
        };

        try {
            const response = await fetch('/settings', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(payload)
            });

            const result = await response.text();

            if (response.ok) {
                status.textContent = 'Settings saved. Reboot the device to apply changes.';
                status.className = 'success';
            } else {
                status.textContent = 'Error: ' + result;
                status.className = 'error';
            }
        } catch (error) {
            status.textContent = 'Error: ' + error.message;
            status.className = 'error';
        } finally {
            button.disabled = false;
        }
    });

    document.getElementById('resetButton').addEventListener('click', async function() {
        const status = document.getElementById('status');
        const button = this;

        if (!confirm('Reset all settings to default and reboot?')) {
            return;
        }

        status.textContent = 'Resetting settings...';
        status.className = '';
        button.disabled = true;

        try {
            const response = await fetch('/reset', { method: 'POST' });
            
            if (response.ok) {
                status.textContent = 'Settings reset. Device will restart.';
                status.className = 'success';
            } else {
                const error = await response.text();
                status.textContent = 'Error: ' + error;
                status.className = 'error';
                button.disabled = false;
            }
        } catch (error) {
            status.textContent = 'Error: ' + error.message;
            status.className = 'error';
            button.disabled = false;
        }
    });

    loadSettings();
});