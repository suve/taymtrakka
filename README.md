# Taymtrakka - a basic time tracking tool

Taymtrakka is a program that enables you to track time spent using various programs,
which in turn allows you to measure the time spent doing various tasks (or slacking off),
estimate your productivity, and so on.

## Usage

When run, the program starts polling the OS to learn which window is currently focused
and how it's titled. The window title is then stored in the program's database.
The program continues to do this every 5 seconds until turned off.

To see the statistics, open your web browser and navigate to http://localhost:36912.


## Building from source

You will need:
- gcc (or some other compiler supporting C99)
- libmicrohttpd
- make
- Python 3.4 or newer
- "rcssmin" Python3 library
- SQLite3

Once you have everything installed, just run `make`.
