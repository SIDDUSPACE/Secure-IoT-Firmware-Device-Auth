# 🔐 Secure IoT Firmware & Device Authentication System  
### Secure Boot · Firmware Integrity · mTLS · Device Revocation  
**ESP32 + QEMU + Backend TLS Server**

---

## 🧠 End-to-End System Architecture

<p align="center">
  <img src="diagrams/architecture/end_to_end_system_architecture.png" width="85%" />
</p>

> **Figure:** Overall system architecture showing ESP32 secure boot, firmware verification, mutual TLS authentication, certificate authority, revocation control, and backend authorization.

---

## 👤 Author

**Siddarth S**

<p align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=18&duration=2500&pause=800&color=00C2FF&center=true&vCenter=true&width=700&lines=Embedded+Systems+%7C+IoT+Security;Secure+Boot+%26+Firmware+Verification;Mutual+TLS+Authentication;Enterprise-Grade+Device+Revocation" />
</p>

---

## 📌 Project Overview

This project implements an **end-to-end secure IoT architecture** that protects an ESP32-class device from:

- Firmware tampering  
- Rogue or cloned devices  
- Stolen certificates  
- Man-in-the-Middle (MITM) attacks  
- Unauthorized backend access  

The system combines:

- **Secure Boot & Root of Trust**
- **Firmware Manifest Signature Verification**
- **Mutual TLS (mTLS) Authentication**
- **Certificate Revocation & Authorization Logic**

All security mechanisms are validated using **QEMU-based ESP32 simulation** and a **real TLS backend server**.

---

## 🔒 Secure Boot & Trust Chain

<p align="center">
  <img src="diagrams/architecture/secure_boot_trust_chain_flow.png" width="70%" />
</p>

### Boot Sequence
1. ESP32 Power-On Reset  
2. ROM Secure Boot (Root of Trust)  
3. Second-Stage Bootloader  
4. Firmware Image Loaded  
5. Manifest Signature Verification  

- ✅ **Valid firmware** → Application executes  
- ❌ **Invalid firmware** → Boot halted  

This ensures **only cryptographically signed firmware can execute**, even if flash memory is modified.

---

## 🔑 Firmware Integrity Protection (QEMU Proof)

<p align="center">
  <img src="diagrams/screenshots/firmware_tampered_image_rejected.png" width="45%" />
  <img src="diagrams/screenshots/manifest_signature_verification_failed.png" width="45%" />
</p>

### What was tested
- A valid firmware image  
- A deliberately tampered firmware image  

### Result
- ✔ Valid firmware → Boots normally  
- ❌ Tampered firmware → Rejected during boot  

This proves **real firmware integrity enforcement**, not checksum validation.

---

## 🔐 Mutual TLS (mTLS) Authentication

<p align="center">
  <img src="diagrams/screenshots/mtls_handshake_sequence_device_server.png" width="75%" />
</p>

### mTLS Handshake Flow
1. ClientHello  
2. ServerHello  
3. Server Certificate (X.509)  
4. Client Certificate (X.509)  
5. Client proves possession of private key  
6. Server verifies certificate (CA + CRL)  
7. Authorization check (CN / Device ID)  
8. Secure TLS channel established  
9. Application data exchange (`AUTH OK`)  

This guarantees **mutual authentication**, not just encrypted transport.

---

## 🪪 Certificate & Revocation Control

<p align="center">
  <img src="diagrams/architecture/certificate_and_revocation_flow.png" width="70%" />
</p>

### Identity Model
- Device certificates issued by **Device Root CA**
- Backend trusts **only this CA**
- Revoked devices stored in **CRL / denylist**

### Decision Logic
- ✔ Certificate valid & not revoked → **Access granted**
- ❌ Revoked or unauthorized → **Connection rejected**

### Proof Screenshots
<p align="center">
  <img src="diagrams/screenshots/server_startup_revocation_enabled.png" width="45%" />
  <img src="diagrams/screenshots/authorized_device_authenticated.png" width="45%" />
</p>

<p align="center">
  <img src="diagrams/screenshots/client_without_cert_rejected.png" width="45%" />
</p>

---

## 🛡️ Threat Mitigation Mapping

<p align="center">
  <img src="diagrams/architecture/threat_mitigation_mapping.png" width="70%" />
</p>
  
|         Threat         |            Mitigation            |
|------------------------|----------------------------------|
| Firmware tampering     | Manifest signature verification  |
| Rogue device           | mTLS + Device CA validation      |
| Stolen certificate     | Certificate revocation list      |
| MITM attack            | TLS encrypted channel            |
| Unauthorized access    | Backend authorization logic      |

This section explains **why each security control exists**.

---

## 🧪 Test Environment

- ESP-IDF v5.x  
- QEMU (ESP32 simulation)  
- OpenSSL (TLS server & client)  
- Python TLS backend  
- Linux / WSL environment  

> **Note:** This design can be directly extended to real ESP32 hardware.

---

## 🏁 Final conculsion 

This project demonstrates a **production-grade IoT security architecture**, covering:

- Root-of-Trust boot
- Firmware integrity
- Strong device identity
- Certificate lifecycle management
- Backend authorization control

It reflects **real-world practices used in industrial, automotive, and aerospace**