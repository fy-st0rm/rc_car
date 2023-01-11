import socket

SERVER_IP   = socket.gethostbyname(socket.gethostname())
SERVER_PORT = 6969

BUFFER_SIZE = 70_000

SLAVE_ADDR  =  9

# Headers
HEADER = b"__IMAGE_BEGIN__"
FOOTER = b"__IMAGE_DONE__"

MOVE  = 1
ANGLE = 2
FLASH = 3

# Commands
FORWARD  = 1
BACKWARD = 2
LEFT     = 3
RIGHT    = 4

