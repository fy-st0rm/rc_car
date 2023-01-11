import socket
import threading
import os
import time
from config import *

class Server:
	def __init__(self, ip, port):
		self.ip   = ip
		self.port = port
		self.__construct_server(ip, port)

		self.running = True
		self.image_buffer  = []
		self.send_buffer   = []
		self.sensor_buffer = []

	def __construct_server(self, ip, port):
		self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
		self.server.bind((ip, port))

	def __sender(self, conn):
		try:
			while True:
				# If send message in queue
				if self.send_buffer:
					conn.send(self.send_buffer.pop(0))
		except Exception as e:
			print(f"[SERVER ERROR]: {e}");

	def __handle_conn(self, conn):
		send_thread = threading.Thread(target=self.__sender, args=(conn,))
		send_thread.start()

		start_time = time.time_ns() / 10_00_000
		while True:
			try:
				stream = b""
				data = conn.recv(BUFFER_SIZE)
	
				# If the data is image
				if data == HEADER:
					data = conn.recv(BUFFER_SIZE)
					packet = 1
					while (data != FOOTER):
						stream += data
						data = conn.recv(BUFFER_SIZE)
						packet += 1
	
					# Appending the image
					self.image_buffer.append(stream)

					# Calculating the time difference in miliseconds.
					end_time = time.time_ns() / 10_00_000
					print(f"Frame received after: {end_time - start_time} ms | Packet size: {packet}");
					start_time = end_time

				elif data.decode("utf-8").split(" ")[0] == "__SENSOR__":
					value = int(data.decode("utf-8").split(" ")[1])
					self.sensor_buffer.append(value)

			except Exception as e: 
				print(f"[SERVER ERROR]: {e}");

	def send(self, header, data):
		self.send_buffer.append((f"{header} {data}\n".encode("utf-8")))

	def start(self):
		self.server.listen()
		print(f"Server listening on {self.ip}:{self.port}")

		while self.running:
			conn, addr = self.server.accept()
			print(addr, " connected.")

			thread = threading.Thread(target=self.__handle_conn, args=(conn,))
			thread.start()

	def stop(self):
		self.running = False
		self.server.shutdown(socket.SHUT_RDWR)
		print("Closing server.")
		os._exit(0)
