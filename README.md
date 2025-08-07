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
./scan <ip> <start_port> <end_port> [options]

Options:
  -h, --help           Show this help message and exit
  -t, --timeout <ms>   Set the timeout for each port scan in milliseconds (default: 100)
  -n, --threads <num>  Set the number of concurrent threads (default: 100)
```

## Testing

This scanner has been thoroughly tested on the following Linux distributions:

- Debian Bullseye  
- Ubuntu 22.04 LTS  
- Arch Linux  
- Kali Linux  
- Alpine Linux  

Testing included compiling, running with various parameters, and verifying stability.  
Docker containers with clean base images were used to ensure portability and environment independence.

## License

MIT License
