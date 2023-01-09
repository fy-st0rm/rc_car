from server import *
from config import *
import pygame
import PIL.Image as Image
import io

class App:
	def __init__(self, width, height):
		self.width = width
		self.height = height
		self.__construct_screen()
		self.running = True
		self.server = Server(SERVER_IP, SERVER_PORT)
		self.image = None

	def __construct_screen(self):
		self.screen = pygame.display.set_mode((self.width, self.height))

	def __load_image(self):
		if len(self.server.image_buffer) == 0: return
		image_bytes = self.server.image_buffer[0]
		with open("test.jpg", "wb") as f:
			f.write(image_bytes)
		self.image = pygame.image.load("test.jpg")
		self.server.image_buffer.pop(0)

	def run(self):
		server_thread = threading.Thread(target=self.server.start)
		server_thread.start()

		while self.running:
			for event in pygame.event.get():
				if event.type == pygame.QUIT:
					self.running = False
					self.server.stop()

				elif event.type == pygame.KEYDOWN:
					if event.key == pygame.K_w:
						self.server.send(FORWARD)
					elif event.key == pygame.K_s:
						self.server.send(BACKWARD)
					elif event.key == pygame.K_a:
						self.server.send(LEFT)
					elif event.key == pygame.K_d:
						self.server.send(RIGHT)

			# Rendering
			self.screen.fill((165, 165, 165))

			self.__load_image()
			if self.image: self.screen.blit(self.image, (100, 100))

			pygame.display.update()

if __name__ == "__main__":
	app = App(800, 600)
	app.run()

