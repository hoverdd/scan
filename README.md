# ctools — TCP Port Scanner

A simple and fast TCP port scanner written in C.

## Features

- Scan a range of TCP ports on a specified IP address  
- Configurable connection timeout  
- Lightweight and efficient implementation  
- Cross-platform support for Unix-like systems (Linux/macOS)

## Build

```bash
make
```

To clean build artifacts:

```bash
make clean
```

## Usage

```bash
./scan <ip> <start_port> <end_port> [-t <timeout_ms>]
```

Example:

```bash
./scan 127.0.0.1 1 1024 -t 100
```

Parameters:

- `<ip>` — target IPv4 address  
- `<start_port>` — starting port number  
- `<end_port>` — ending port number  
- `-t <timeout_ms>` — optional timeout in milliseconds (default: 50)

## License

MIT License
