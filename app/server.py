import socket
import threading
import os
from config import *

class Server:
	def __init__(self, ip, port):
		self.ip   = ip
		self.port = port
		self.__construct_server(ip, port)

		self.running = True
		self.image_buffer = []
		self.send_buffer = []

	def __construct_server(self, ip, port):
		self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
		self.server.bind((ip, port))

	def send(self, data):
		self.send_buffer.append((f"{data}\r".encode("utf-8")))

	def start(self):
		self.server.listen()
		print(f"Server listening on {self.ip}:{self.port}")

		self.conn, addr = self.server.accept()
		print(self.conn, " connected.")

		while self.running:
			stream = b""
			data = self.conn.recv(BUFFER_SIZE)

			# If the data is image
			if data == HEADER:
				data = self.conn.recv(BUFFER_SIZE)
				while (data != FOOTER):
					stream += data
					data = self.conn.recv(BUFFER_SIZE)

				# Appending the image
				self.image_buffer.append(stream)
				print("Image buffer:", len(self.image_buffer))

			if (len(self.send_buffer) > 0): self.conn.send(self.send_buffer.pop(0))

	def stop(self):
		self.running = False
		self.server.shutdown(socket.SHUT_RDWR)
		print("Closing server.")
