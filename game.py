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

       elif event.type == RED_GOAL:
           red_rawscore += 1
       elif event.type == YELLOW_GOAL:
           yellow_rawscore += 1
     
        # convert motorstring to a string
    motor_save = "".join(str(x) for x in motorstring)
    write_motor_data("motor_data",motor_save)
    try:
        image = pygame.image.load("uploaded_image.jpg").convert()
        screen.blit(image, (50,50))
    except:
        pass

    pygame.display.update()

def scoreboard():
     pygame.font.init()

     RED = (255,0,0)
     YELLOW = (255,255,0)
     WHITE = (255,255,255)

     SCORE_FONT = pygame.font.SysFont('comicsans',40)

     RED_GOAL = pygame.USEREVENT + 1
     YELLOW_GOAL = pygame.USEREVENT + 2

     red_rawscore = 0
     yellow_rawscore = 0

     red_scoreboard = SCORE_FONT.render(str(red_rawscore), 1, WHITE)
     yellow_scoreboard = SCORE_FONT.render(str(yellow_rawscore), 1, WHITE)

     RED_BOX = pygame.Rect(800,400,50,50)
     YELLOW_BOX = pygame.Rect(860,400,50,50)
     pygame.draw.rect(screen, RED, RED_BOX)
     pygame.draw.rect(screen, YELLOW, YELLOW_BOX)
     
     screen.blit(red_scoreboard, (800, 400))
     screen.blit(yellow_scoreboard, (860, 400))

     pygame.display.update()
     
