# Security Overview

This project demonstrates a secure IoT firmware and device authentication system,
focusing on **cryptographic trust, device identity, and firmware integrity**.

It is designed as an educational and demonstrative implementation of
real-world IoT security principles.

---

## Threat Model

The system considers the following threat scenarios:

- Unauthorized devices attempting to connect to the backend
- Man-in-the-middle (MITM) attacks on device–server communication
- Tampered or modified firmware images
- Compromised or revoked device credentials

The system does **not** attempt to defend against:
- Physical attacks on the device
- Side-channel attacks
- Hardware fault injection
- Supply-chain attacks

---

## Device Identity & Authentication

### Mutual TLS (mTLS)

Each device authenticates to the backend server using **mutual TLS**:

- The server presents a certificate signed by a trusted CA
- The device presents a **unique device certificate**
- The server verifies the device certificate against the CA

Device identity is derived from the **X.509 Common Name (CN)** field.

> Note: CN-based identity is used for demonstration purposes.
> Production systems should prefer SAN extensions, SPIFFE IDs,
> or certificate fingerprints.

---

## Certificate Authority (CA)

- A private Certificate Authority (CA) is used to issue device certificates
- The CA certificate is embedded in firmware as a **trust anchor**
- The CA private key is **never stored on the device**

All certificates used in this repository are **demo-only** and generated locally.

---

## Private Key Handling

- Device private keys are required for mTLS authentication
- Private keys are **never committed to version control**
- Private keys are embedded only in local build artifacts for testing

The repository intentionally excludes:
- Private keys
- Production certificates
- Signing keys

---

## Firmware Integrity & OTA Verification

Firmware authenticity is verified using **digital signatures**:

- Firmware images are signed offline using a private signing key
- The device stores **only the public verification key**
- Firmware integrity is verified using SHA-256 and signature validation

If firmware verification fails:
- The firmware is rejected
- The device halts or reboots

Private signing keys are **never present on the device**.

---

## Device Revocation

The backend server supports **device revocation**:

- Revoked device identities are stored in a denylist
- Devices on the denylist are rejected during TLS authentication
- Revocation decisions are enforced at connection time

This demonstrates certificate-based access control and lifecycle management.

---

## Secure Boot (Conceptual)

The system is compatible with ESP32 Secure Boot and Flash Encryption.
These features are **not enabled by default** in this repository but are
considered part of the intended deployment model.

---

## Security Limitations

This project is a **demonstration**, not a production-ready system.

Known limitations:
- CN-based identity validation
- File-based revocation list
- No hardware-backed key storage (eFuse / HSM)
- No automatic key rotation

---

## Responsible Use

This repository is intended for:
- Learning
- Demonstration
- Academic and portfolio purposes

Do **not** deploy this system in production without
proper security audits and hardware protections.
