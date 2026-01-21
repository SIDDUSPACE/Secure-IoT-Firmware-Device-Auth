# Backend Authentication Server

This directory contains the backend server responsible for authenticating
IoT devices using **mutual TLS (mTLS)**.

The server validates device identity using X.509 certificates and enforces
certificate-based access control.

---

## Overview

The backend server performs the following functions:

- Listens for incoming TLS connections from devices
- Requires client certificate authentication (mTLS)
- Verifies device certificates against a trusted Certificate Authority (CA)
- Extracts device identity from the certificate
- Rejects revoked or unauthorized devices

The server is intentionally minimal and implemented using the Python
standard library.

---

## Authentication Model

### Mutual TLS (mTLS)

- Devices present a client certificate during TLS handshake
- The server validates the certificate against a trusted CA
- The server presents its own certificate to the device

Both sides authenticate each other before any application data is exchanged.

---

## Device Identity

Device identity is derived from the **X.509 Common Name (CN)** field
of the client certificate.

Accepted devices must follow the naming pattern:

esp32-device-<unique-id>


> Note: CN-based identity is used for demonstration purposes.
> Production systems should prefer SAN extensions or certificate fingerprints.

---

## Device Revocation

The server supports device revocation using a denylist mechanism:

- Revoked device identities are stored in a text file
- One device identity per line
- Connections from revoked devices are rejected immediately

Revocation checks are enforced at connection time.

---

## Files

|       File           |                   Description               |
|----------------------|---------------------------------------------|
| `server.py`          | mTLS authentication server                  |
| `requirements.txt`   | Python dependencies (standard library only) |
| `revoked_devices.txt`| Device denylist (local, not committed)      |

---

## Running the Server

1. Generate server certificates and CA locally
2. Place certificates in the working directory (not committed)
3. Start the server:

```bash
python3 server.py

The server listens on:

0.0.0.0:8443