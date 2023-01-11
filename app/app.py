from server import *
from config import *
from math import *
import pygame
import PIL.Image as Image
import io

CONTROL_BOUNDARY_COLOR = (165, 165, 165, 90)
CONTROL_COLOR          = (255, 255, 255, 200)
BUTTON_COLOR_INACTIVE  = (255, 255, 255, 190)
BUTTON_COLOR_ACTIVE    = (255, 255, 255, 100)
POV_COLOR              = (200, 249, 244, 150)

class App:
	def __init__(self, width, height):
		self.win_width	 = width
		self.win_height  = height
		self.vp_width	 = 600
		self.vp_height	 = 480

		print("[DISPLAY INFO]:")
		print(f"[WINDOW]: {self.win_width}x{self.win_height}")
		print(f"[VIEW PORT]: {self.vp_width}x{self.vp_height}")

		self.__construct_screen()

		# Creating server
		self.server = Server(SERVER_IP, SERVER_PORT)

		self.running = True
		self.image = None
		self.clock = pygame.time.Clock()

		# Movement variables
		self.movement = {
			FORWARD : False,
			BACKWARD: False,
			LEFT	: False,
			RIGHT	: False
		}
		self.angle = 90
		self.angle_speed = 30

		# Controls
		self.control_bound = (self.vp_width / 8, self.vp_height / 1.2)
		self.control_pos   = [self.control_bound[0], self.control_bound[1]]

		# Button color states
		self.left_turn_color  = BUTTON_COLOR_INACTIVE
		self.right_turn_color = BUTTON_COLOR_INACTIVE
		self.flash_color	  = BUTTON_COLOR_INACTIVE

		# Warning
		self.is_warned    = False
		self.warn_start   = 0
		self.warn_timeout = 1

		self.__load_assets()

	def __load_assets(self):
		self.rot_left_img  = pygame.image.load("assets/rotate-left.png")
		self.rot_right_img = pygame.image.load("assets/rotate-right.png")
		self.flash_off_img = pygame.image.load("assets/flash_off.png")
		self.flash_on_img  = pygame.image.load("assets/flash_on.png")
		self.warning_img   = pygame.image.load("assets/warning.png")
		self.car_model     = pygame.image.load("assets/car-top-view.png").convert()
		self.car_model.set_alpha(150)

	def __construct_screen(self):
		self.screen		= pygame.display.set_mode((self.win_width, self.win_height))
		self.viewport	= pygame.Surface((self.vp_width, self.vp_height))
		self.ui_surface = pygame.Surface((self.vp_width, self.vp_height), pygame.SRCALPHA)

	def __load_image(self):
		if len(self.server.image_buffer) == 0: return
		
		image_bytes = self.server.image_buffer.pop(0)
		try:
			# Loading image through PIL and converting to pygame surface
			# NOTE: Might be slow
			image = Image.open(io.BytesIO(image_bytes))
			mode = image.mode
			size = image.size
			data = image.tobytes()
			self.image = pygame.image.fromstring(data, size, mode)
		except Exception as e:
			print(f"[APPLICATION ERROR]: {e}.");

	def __make_triangle(self, pos, scale, internalAngle, rotation):
		#define the points in a uint space
		ia = (radians(internalAngle) * 2) - 1
		p1 = (0, 0)
		p2 = (cos(ia), sin(ia))
		p3 = (cos(ia) * -1, sin(ia))
	
		#rotate the points
		ra = radians(rotation) 
		rp1x = p1[0] * cos(ra) - p1[1] * sin(ra)
		rp1y = p1[0] * sin(ra) + p1[1] * cos(ra)
		rp2x = p2[0] * cos(ra) - p2[1] * sin(ra)
		rp2y = p2[0] * sin(ra) + p2[1] * cos(ra)
		rp3x = p3[0] * cos(ra) - p3[1] * sin(ra)
		rp3y = p3[0] * sin(ra) + p3[1] * cos(ra)
		rp1 = ( rp1x, rp1y )
		rp2 = ( rp2x, rp2y )
		rp3 = ( rp3x, rp3y )
	
		#scale the points 
		sp1 = [rp1[0] * scale + pos[0], rp1[1] * scale + pos[1]]
		sp2 = [rp2[0] * scale + pos[0], rp2[1] * scale + pos[1]]
		sp3 = [rp3[0] * scale + pos[0], rp3[1] * scale + pos[1]]

		return sp1, sp2, sp3

	def __event_handler(self):
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				self.running = False
				self.server.stop()

			elif event.type == pygame.KEYDOWN:

				# WASD movement on keydown
				if event.key == pygame.K_w:
					self.movement[FORWARD]	= True
				elif event.key == pygame.K_s:
					self.movement[BACKWARD] = True
				elif event.key == pygame.K_a:
					self.movement[LEFT]		= True
				elif event.key == pygame.K_d:
					self.movement[RIGHT]	= True

				# Camera angle change
				elif event.key == pygame.K_q:
					if self.angle + self.angle_speed > 180:
						self.angle = 180
					else:
						self.angle += self.angle_speed
					self.server.send(ANGLE, self.angle);

					self.left_turn_color = BUTTON_COLOR_ACTIVE

				elif event.key == pygame.K_e:
					if self.angle - self.angle_speed < 0:
						self.angle = 0
					else:
						self.angle -= self.angle_speed
					self.server.send(ANGLE, self.angle);

					self.right_turn_color = BUTTON_COLOR_ACTIVE

				# Flash control
				elif event.key == pygame.K_f:
					self.server.send(FLASH, 0);
					if self.flash_color == BUTTON_COLOR_INACTIVE:
						self.flash_color = BUTTON_COLOR_ACTIVE
					else:
						self.flash_color = BUTTON_COLOR_INACTIVE

			elif event.type == pygame.KEYUP:

				# WASD movement on keyup
				if event.key == pygame.K_w:
					self.movement[FORWARD]	= False
				elif event.key == pygame.K_s:
					self.movement[BACKWARD] = False
				elif event.key == pygame.K_a:
					self.movement[LEFT]		= False
				elif event.key == pygame.K_d:
					self.movement[RIGHT]	= False

				# Turn movement
				elif event.key == pygame.K_q:
					self.left_turn_color  = BUTTON_COLOR_INACTIVE
				elif event.key == pygame.K_e:
					self.right_turn_color = BUTTON_COLOR_INACTIVE

			elif event.type == pygame.VIDEORESIZE:
				self.win_width = self.screen.get_width()
				self.win_height = self.screen.get_height()
	
	def __update(self):
		if self.movement[FORWARD]:
			self.server.send(MOVE, FORWARD)
			self.control_pos[1] = self.control_bound[1] - 20
		elif self.movement[BACKWARD]:
			self.server.send(MOVE, BACKWARD)
			self.control_pos[1] = self.control_bound[1] + 20
		elif self.movement[LEFT]:
			self.server.send(MOVE, LEFT)
			self.control_pos[0] = self.control_bound[0] - 20
		elif self.movement[RIGHT]:
			self.server.send(MOVE, RIGHT)
			self.control_pos[0] = self.control_bound[0] + 20
		else:
			self.control_pos[0] = self.control_bound[0]
			self.control_pos[1] = self.control_bound[1]


	def __render_ui(self):
		# WASD controls
		pygame.draw.circle(self.ui_surface, CONTROL_BOUNDARY_COLOR, self.control_bound, 50, 2)
		pygame.draw.circle(self.ui_surface, CONTROL_COLOR, self.control_pos, 20, 1)
		pygame.draw.circle(self.ui_surface, CONTROL_COLOR, self.control_pos, 16)

		# Servo angle control
		left_p	= (self.vp_width / 2 - 100, self.vp_height / 1.2)
		right_p = (self.vp_width / 2 + 100, self.vp_height / 1.2)

		# Left turn button
		pygame.draw.circle(self.ui_surface, CONTROL_BOUNDARY_COLOR, left_p, 25, 2)
		pygame.draw.circle(self.ui_surface, self.left_turn_color  , left_p, 20)
		self.ui_surface.blit(
				self.rot_left_img, 
				(left_p[0] - self.rot_left_img.get_width() / 2, left_p[1] - self.rot_left_img.get_height() / 2)
			)

		# Right turn button
		pygame.draw.circle(self.ui_surface, CONTROL_BOUNDARY_COLOR, right_p, 25, 2)
		pygame.draw.circle(self.ui_surface, self.right_turn_color , right_p, 20)
		self.ui_surface.blit(
				self.rot_right_img,
				(right_p[0] - self.rot_right_img.get_width() / 2, right_p[1] - self.rot_right_img.get_height() / 2)
			)

		# Flash button
		flash_p = (self.vp_width / 8, self.vp_height / 1.6)
		pygame.draw.circle(self.ui_surface, CONTROL_BOUNDARY_COLOR, flash_p, 25, 2)
		pygame.draw.circle(self.ui_surface,		  self.flash_color, flash_p, 20)

		if self.flash_color == BUTTON_COLOR_ACTIVE:
			self.ui_surface.blit(
					self.flash_on_img,
					(flash_p[0] - self.flash_on_img.get_width() / 2, flash_p[1] - self.flash_on_img.get_height() / 2)
				)
		elif self.flash_color == BUTTON_COLOR_INACTIVE:
			self.ui_surface.blit(
					self.flash_off_img,
					(flash_p[0] - self.flash_off_img.get_width() / 2, flash_p[1] - self.flash_off_img.get_height() / 2)
				)


		# View model
		model_pos = (self.vp_width / 1.1 - self.car_model.get_width() / 2, self.vp_height / 1.2 - self.car_model.get_height() / 2)
		pov_pos   = (model_pos[0] + self.car_model.get_width() / 2, model_pos[1] + self.car_model.get_height() / 2)
		self.ui_surface.blit(self.car_model, model_pos)

		# POV ui
		pov_points = self.__make_triangle(pov_pos, 50, 60, -(self.angle + 90))
		pygame.draw.polygon(self.ui_surface, POV_COLOR, pov_points)

		# Warning sigh
		if len(self.server.sensor_buffer) > 0:
			if not self.is_warned:
				self.is_warned = True
				self.warn_start = time.time()
				self.server.sensor_buffer.pop(0)

		if self.is_warned:
			if time.time() - self.warn_start >= self.warn_timeout:
				self.is_warned = False
			self.ui_surface.blit(self.warning_img, (self.vp_width / 1.1 - self.warning_img.get_width() / 2, self.vp_height / 9))

	def __render_viewport(self):
		self.__load_image()
		if self.image: self.viewport.blit(self.image, (0, 0))

	def __render(self):
		self.screen.fill((165, 165, 165))
		self.viewport.fill((0,0,0))
		self.ui_surface.fill((0,0,0,100))

		self.__render_viewport()
		self.__render_ui()

		self.screen.blit(self.viewport, (self.win_width / 2 - self.vp_width / 2, self.win_height / 2 - self.vp_height / 2))
		self.screen.blit(self.ui_surface, (self.win_width / 2 - self.vp_width / 2, self.win_height / 2 - self.vp_height / 2))

		pygame.display.update()
		self.clock.tick(60)

	def run(self):
		server_thread = threading.Thread(target=self.server.start)
		server_thread.start()

		while self.running:
			self.__event_handler()
			self.__update()
			self.__render()


if __name__ == "__main__":
	pygame.init()
	disp_info = pygame.display.Info()
	win_width = disp_info.current_w
	win_height = disp_info.current_h
	app = App(600, 480)
	app.run()

