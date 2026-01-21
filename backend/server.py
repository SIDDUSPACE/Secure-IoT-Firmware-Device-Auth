import ssl
import socket

HOST = "0.0.0.0"
PORT = 8443

CA_CERT = "ca.pem"
SERVER_CERT = "server.crt"
SERVER_KEY = "server.key"

REVOKE_FILE = "revoked_devices.txt"


def load_revoked_devices():
    """
    Load revoked device CNs from file.
    One CN per line.
    """
    try:
        with open(REVOKE_FILE, "r") as f:
            return set(line.strip() for line in f if line.strip())
    except FileNotFoundError:
        return set()


def main():
    # ---- Load denylist once at startup ----
    revoked_devices = load_revoked_devices()
    print(f"[SERVER] Revoked devices loaded: {revoked_devices}")

    # ---- TLS context (client auth REQUIRED) ----
    context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
    context.verify_mode = ssl.CERT_REQUIRED

    # Trust ONLY your CA
    context.load_verify_locations(cafile=CA_CERT)

    # Server identity
    context.load_cert_chain(
        certfile=SERVER_CERT,
        keyfile=SERVER_KEY
    )

    # ---- TCP socket ----
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((HOST, PORT))
    sock.listen(5)

    print(f"[SERVER] Listening on {HOST}:{PORT}")

    while True:
        conn, addr = sock.accept()
        try:
            tls_conn = context.wrap_socket(conn, server_side=True)

            # ---- Extract device identity ----
            cert = tls_conn.getpeercert()
            subject = dict(x[0] for x in cert["subject"])
            device_cn = subject.get("commonName", "UNKNOWN")

            print(f"[SERVER] Connection from {addr}, CN={device_cn}")

            # ---- Basic identity check ----
            if not device_cn.startswith("esp32-device-"):
                print("[SERVER] ❌ Unauthorized device (invalid CN)")
                tls_conn.close()
                continue

            # ---- Revocation check (THIS IS THE KEY FEATURE) ----
            if device_cn in revoked_devices:
                print(f"[SERVER] ❌ REVOKED DEVICE BLOCKED: {device_cn}")
                tls_conn.close()
                continue

            print("[SERVER] ✅ Device authenticated & authorized")

            tls_conn.send(b"AUTH OK\n")
            tls_conn.close()

        except ssl.SSLError as e:
            print("[SERVER] ❌ TLS error:", e)

        except Exception as e:
            print("[SERVER] ❌ General error:", e)


if __name__ == "__main__":
    main()
