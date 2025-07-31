# KingsPaw Restaurant TCP Server

This is a simple multi-threaded TCP server project written in C for a fictional restaurant called **KingsPaw Restaurant**. The server allows clients to connect over the network, view a menu of meals, salads, and drinks, and place orders that are processed by chef threads.

## Features

- Multi-threaded: Handles up to 36 clients concurrently using POSIX threads and semaphores.
- Chef Simulation: 3 chef threads process orders from a shared queue.
- Menu System: Displays a categorized menu (Heavy Meals, Salads, Drinks) with details for each item.
- Simple Command Interface: Clients can view the menu, place an order, pay, tip, and exit.
- Colorful Terminal Output: Uses ANSI escape codes for colored text in compatible terminals.

## Progress

- **Core functionality is ~80% complete.**
- Order queue, chef threads, and client-server interaction are implemented.
- Payment and tip handling are in place.
- Further improvements and client features are in progress.

## How It Works

1. The server listens on a specified port (default: 9000).
2. Clients connect via TCP (e.g., using `telnet` or the provided client app).
3. The server greets the client and presents options:
   - View Menu
   - Place Order
   - Exit
4. If "View Menu" is selected, the server sends a formatted menu with all available items.
5. If "Place Order" is selected, the client chooses an item, waits for the chef to prepare it, then pays and tips before disconnecting.
6. The client can continue interacting or exit.

## How to Run

### Server

1. **Build the server:**

   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

2. **Start the server:**
   ```sh
   ./restaurant_server
   ```

### Client

If you have a Go client (example):

1. **Build the client:**

   ```sh
   cd client
   go build -o restaurant_client main.go
   ```

2. **Run the client:**
   ```sh
   ./restaurant_client
   ```

Or connect using telnet:

```sh
telnet 127.0.0.1 9000
```

## File Structure

- `main.c` - Main server logic and client handler.
- `functions.c` - Order queue, chef, and client handling logic.
- `data.c` - Menu data (meals, salads, drinks).
- `data.h` / `types.h` - Struct definitions and extern declarations.
- `constants.h` / `constants.c` - Constants and global variables.
- `client/` - (Optional) Go client implementation.
- `readme.md` - This file.

## Notes

- This project is for learning and demonstration purposes only.
- The menu and order system are simulated; no real order persistence is implemented.
- You can expand the project by adding real order tracking, user authentication, or a GUI client.

---

Enjoy exploring
