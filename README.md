
# WebServer

A web server project involves developing software that responds to requests made over the Hypertext Transfer Protocol (HTTP). This software acts as the engine behind a website, delivering content and functionality to users who access it through web browsers.


## Introduction

The 42 webserver project is all about building your own web server from scratch, specifically in C++98. It's a chance to dive deep and understand the inner workings of what powers websites. Here's some of what we should accomplish:

+ Core Functionality: Develop a functional HTTP server that can handle incoming requests and deliver responses. This includes parsing HTTP methods (GET, POST, DELETE), processing headers, and sending back appropriate response.
+ Static Content Serving: Configure your server to locate and serve static files like HTML, CSS, videos, and images, forming the basic building blocks of webpages.
+ Non-Blocking Operations: Implement the server using a non-blocking approach, allowing it to handle multiple client connections concurrently without getting bogged down by a single request. This is typically achieved using poll() or similar techniques.
+ Configuration: The server should be able to read configuration files that define settings like port number, root directory for static files, and additional functionalities.
## Lessons Learned

The project provided valuable hands-on experience in crafting basic web server configuration files, offering insights into how popular web servers typically parse their configuration directives. Subsequently, the development process involved parsing and handling HTTP requests, followed by crafting appropriate responses. Additionally, the project delved into the concept of I/O multiplexing, a crucial technique for handling concurrent client connections efficiently. Furthermore, it explored how certain requests can be delegated for processing by CGI scripts.
## Features

- HTTP Request Parsing: Parse methods, headers, and potentially content.
- Static Content Serving: Locate and deliver HTML, CSS, and images.
- Non-Blocking Operations: Handle multiple connections concurrently.
- Configuration File Parsing: Read settings like port and root directory.
- Error Handling: Return appropriate error codes for issues.
- CGI Script Support: (Project-dependent) Handle requests for external processing.
- And more ...
## Installation

```bash
git clone https://github.com/hex01e/webserv.git
cd webserv
make
./webserv "path_of_config_file"
```
## Authors

- [@Hamza Oumansour](https://github.com/hex01e)

- [@ABDELLATYF EN-NEIYMY](https://github.com/Abdlatif-20)

- [@Mohamed El Youssfi](https://github.com/SimoRedDevil)
