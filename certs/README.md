# Certificates & Keys

This directory documents the certificate and key material used by the
Secure IoT Firmware & Device Authentication System.

**No real cryptographic secrets are stored in this repository.**

---

## Certificate Authority (CA)

A private Certificate Authority (CA) is used to issue certificates for:

- Backend server authentication
- Device authentication (mTLS)

The CA private key is generated **locally** and is **never committed**
to version control.

Only the CA **public certificate** is embedded in firmware as a trust anchor.

---

## Device Certificates

Each device is provisioned with a unique X.509 certificate:

- Issued by the project CA
- Used for mutual TLS authentication
- Identity derived from the certificate Common Name (CN)

Device private keys:
- Exist only in local build environments
- Are embedded into firmware binaries **only for testing**
- Are excluded from this repository

---

## Firmware Signing Keys

Firmware images are signed offline using a private signing key.

- The signing private key is **never present on the device**
- The device stores only the **public verification key**
- Signature verification is performed during firmware validation

---

## Key & Certificate Generation

Certificates and keys are generated locally using scripts and tooling
outside this repository.

Typical artifacts include:
- `ca.pem`
- `device.crt`
- `device.key`
- firmware signing keys

These files are intentionally excluded via `.gitignore`.

---

## Security Note

This repository is designed for:
- Demonstration
- Learning
- Portfolio presentation

It is **not** a production key management system.

Do not reuse demo keys or certificates in real deployments.
