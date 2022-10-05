# 42-Minitalk

<p align="center">
	<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/ailopez-o/42Barcelona-minitalk?color=lightblue" />
	<img alt="Number of lines of code" src="https://img.shields.io/tokei/lines/github/ailopez-o/42Barcelona-minitalk?color=critical" />
	<img alt="Code language count" src="https://img.shields.io/github/languages/count/ailopez-o/42Barcelona-minitalk?color=yellow" />
	<img alt="GitHub top language" src="https://img.shields.io/github/languages/top/ailopez-o/42Barcelona-minitalk?color=blue" />
	<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/ailopez-o/42Barcelona-minitalk?color=green" />
</p>

## Server - Client

The purpose of this project is to code a small data exchange program using UNIX signals.


Cloning the repo
```bash
git clone --recursive https://github.com/ailopez-o/42Barcelona-Minitalk.git
```
To compile client & server

```bash
$ make
```
To compile client

```bash
$ make client
```
To compile server

```bash
$ make client
```

## How to use

Execute Server

```bash
$ ./server
```


Execute client with the Server PID and a string to send

```bash
$ ./client 12345 "String to send"
```
```bash
$ ./client 12345 "$(cat file.txt)"
```
```bash
$ ./client 12345 -g file.txt
```

<p align="center">
	<img src="https://github.com/ailopez-o/42Barcelona-Minitalk/blob/main/img/client.png?raw=true" />
	<img src="https://github.com/ailopez-o/42Barcelona-Minitalk/blob/main/img/server.png?raw=true" />
</p>



