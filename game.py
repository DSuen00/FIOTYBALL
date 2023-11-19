# import the pygame module, so you can use it
import pygame
from file_read import write_motor_data
import sys
# define a main function
def game_setup():
     
    # initialize the pygame module
    write_motor_data("goals", "0:0")
    pygame.init()
    # create a surface on screen that has the size of 240 x 180
    res = (1280, 720)
    screen = pygame.display.set_mode(res, pygame.RESIZABLE)
     
    # define a variable to control the main loop
    background = pygame.image.load("Background.jpg").convert()
    screen.blit(background, (0,0))
    pygame.key.set_repeat()
    pygame.display.update()
    return screen

def game_main(screen, motorstring):
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        # Check for KEYDOWN event
        elif event.type == pygame.KEYDOWN:
            # You can also check for specific keys
            if event.key == pygame.K_LEFT:
                motorstring[2] = 0
            elif event.key == pygame.K_RIGHT:
                motorstring[2] = 2
            elif event.key == pygame.K_UP:
                motorstring[3] = 0
            elif event.key == pygame.K_DOWN:
                motorstring[3] = 2
            elif event.key == pygame.K_a:
                motorstring[0] = 0
            elif event.key == pygame.K_d:
                motorstring[0] = 2
            elif event.key == pygame.K_w:
                motorstring[1] = 0
            elif event.key == pygame.K_s:
                motorstring[1] = 2

        # Check for KEYUP event
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_LEFT:
                motorstring[2] = 1
            elif event.key == pygame.K_RIGHT:
                motorstring[2] = 1
            elif event.key == pygame.K_UP:
                motorstring[3] = 1
            elif event.key == pygame.K_DOWN:
                motorstring[3] = 1
            elif event.key == pygame.K_a:
                motorstring[0] = 1
            elif event.key == pygame.K_d:
                motorstring[0] = 1
            elif event.key == pygame.K_w:
                motorstring[1] = 1
            elif event.key == pygame.K_s:
                motorstring[1] = 1
        
        # convert motorstring to a string
    motor_save = "".join(str(x) for x in motorstring)
    write_motor_data("motor_data",motor_save)
    try:
        image = pygame.image.load("uploaded_image.jpg").convert()
        screen.blit(image, (50,50))
    except:
        pass

    pygame.display.update()
