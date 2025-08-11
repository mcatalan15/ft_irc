# 🌐 ft_irc: My Own IRC Server! 🌟

Hey there, welcome to **ft_irc**! 🎉 This project is our take on building a fully functional **Internet Relay Chat (IRC) server** in **C++98**, designed to handle real-time messaging with multiple clients over **TCP/IP**. Using the **Irssi** client as our reference, we created a robust server that supports authentication, channel management, operator commands, and even some cool bonuses like a Spanish-phrase-loving bot! 🤖 Whether you're a networking nerd or a curious coder, let’s dive into the world of IRC! 😺

## 🛠️ What’s This Project About?

**ft_irc** is an IRC server that lets users connect, chat, and manage channels in real time. Built from scratch in **C++98**, it handles multiple clients concurrently using non-blocking I/O with `poll()`. It supports core IRC commands like `JOIN`, `PRIVMSG`, and `KICK`, plus channel modes for operators. We also implemented bonus features like file transfers and a fun bot that responds to Spanish phrases (e.g., saying "bingo!" when someone types "ocho"). The server is resilient, handles edge cases, and uses **vectors** and **maps** for efficient data management. It’s a deep dive into **socket programming**, **networking**, and protocol implementation! 🚀

### ✨ Features
- **Multi-Client Support**: Handles multiple clients simultaneously with non-blocking `poll()`.
- **Client Registration**: Authenticate users with `PASS`, `NICK`, and `USER` commands via **Irssi**.
- **Channel Management**: Create and join channels (`JOIN`), send messages (`PRIVMSG`), set topics (`TOPIC`), and manage users (`KICK`, `INVITE`).
- **Operator Commands**: Control channels with modes (`+i`, `+t`, `+k`, `+o`, `+l`) and operator privileges.
- **Robust Networking**: Built with **TCP/IP** (IPv4/IPv6) using socket functions (`socket`, `bind`, `listen`, `accept`).
- **Bonus Features**:
  - **File Transfer**: Send and receive files with `SENDFILE` and `GETFILE` commands.
  - **Smart Bot**: Joins a channel, listens to messages, trims them, and responds to Spanish phrases (e.g., "ocho" → "bingo!").
- **Error Handling**: Gracefully manages partial data, low bandwidth, and invalid inputs.

### 🧠 What I Learned
This project was a fantastic journey into low-level programming and networking! Here’s what I mastered:
- Writing **C++98** code with modern features (e.g., `<cstring>`) and data structures like **vectors** and **maps** for managing clients and channels.
- Implementing **non-blocking I/O** with `poll()` to handle multiple clients efficiently.
- Designing **TCP/IP** communication with socket programming and byte-order conversions (`htons`, `ntohl`).
- Parsing IRC commands and handling **CRLF** (`\r\n`) for complete messages.
- Managing **signal handling** (`sigaction`) for clean shutdowns.
- Building a **bot** that processes messages and responds to Spanish phrases (e.g., "ocho" → "bingo!").
- Testing with **Irssi** to ensure compatibility with a real IRC client.
- Tackling edge cases like partial data and low-bandwidth scenarios.

## 🚀 Getting Started

Ready to run your own IRC server? Follow these steps to set it up locally!

### Prerequisites
- **C++98 Compiler** (e.g., `g++` with `-std=c++98 -Wall -Wextra -Werror`)
- **Irssi** (reference IRC client): [Install Irssi](https://irssi.org/)
- A Unix-like system (Linux or macOS)
- Basic knowledge of networking and terminal commands

### Installation
1. Clone the repo:
   ```bash
   git clone https://github.com/mcatalan15/ft_irc.git
   ```
2. Navigate to the project folder:
   ```bash
   cd ft_irc
   ```
3. Compile the server with the provided Makefile:
   ```bash
   make
   ```
4. Run the server:
   ```bash
   ./ircserv <port> <password>
   ```
  Example: `./ircserv 6667 mypassword`
 
### Usage
1. Start the server: `./ircserv 6667 mypassword`
2. Open Irssi in a terminal:
   ```bash
   irssi
   ```
3. Connect to the server:
   ```bash
   /connect 127.0.0.1 6667 mypassword
   ```
4. Register your client:
   ```bash
   PASS mypassword
   NICK mynick
   USER myuser localhost ft_irc :My Real Name
   ```
5. Join a channel and start chatting:
   ```bash
   JOIN #mychannel
   PRIVMSG #mychannel :Hello, world!
   ```
6. Try the bot: Type "ocho" in the channel and the bot will reply "bingo!"🎉
7. Test file transfer: Use `SENDFILE <user> <filepath>` to share files.

### Example Commands
- **Join a channel**: `JOIN #mychannel`
- **Send a message**: `PRIVMSG #mychannel :Hey, everyone!`
- **Set a channel topic**: `TOPIC #mychannel :Welcome to my IRC!`
- **Kick a user**: `KICK #mychannel baduser :Be nice!`
- **Set channel mode**: `MODE #mychannel +k secretkey`
- **Trigger the bot**: `PRIVMSG #mychannel :ocho (Bot replies: "bingo!")`

## 📧 Contact Us
This project was a team effort, and my awesome mates at 42 Barcelona helped make it happen! Reach out to us for questions, collabs, or just to chat about coding! 😸
- **Jon Paul Kruger**: [jpaul-kr (GitHub)](https://github.com/jpaul-kr) | [jpaul-kr@student.42barcelona.com](mailto:jpaul-kr@student.42barcelona.com)
- **Eva Ferré Mur**: [eferre-m (GitHub)](https://github.com/eferre-m) | [eferre-m@student.42barcelona.com](mailto:eferre-m@student.42barcelona.com)
- **Marc Catalán Sánchez**: [mcatalan15 (GitHub)](https://github.com/mcatalan15) | [mcatalan@student.42barcelona.com](mailto:mcatalan@student.42barcelona.com)
