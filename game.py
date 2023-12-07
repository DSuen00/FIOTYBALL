# import the pygame module, so you can use it
import pygame
from file_read import write_motor_data, read_motor_data
import sys

motor_data = [1,1,1,1]

# define a main function
def game_setup():
     
    # initialize the pygame module
    write_motor_data("goals", "0:0")
    pygame.init()
    pygame.font.init()

    # create a surface on screen that has the size of 240 x 180
    res = (1280, 720)
    screen = pygame.display.set_mode(res, pygame.RESIZABLE)

    # blit background
    background = pygame.image.load("Background.jpg").convert()
    screen.blit(background, (0,0))

    # draw boxes
    RED = (200,50,0)
    YELLOW = (255,200,0)
    WHITE = (255,255,255)
    top = 363 
    RED_BOX = pygame.Rect(1047,top,110,150)
    YELLOW_BOX = pygame.Rect(860,top,110,150)
    pygame.draw.rect(screen, RED, RED_BOX)
    pygame.draw.rect(screen, YELLOW, YELLOW_BOX)

    # define a variable to control the main loop

    scoreboard(screen, [0,0])
    pygame.key.set_repeat()
    pygame.display.update()
    return screen

def game_main(screen, motorstring, score_arr):
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
    scoreboard(screen, score_arr)
    try:
        image = pygame.image.load("uploaded_image.jpg").convert()
        image = pygame.Surface.convert(image)
        image = pygame.transform.scale(image,(640,480))
        screen.blit(image, (80,42))
    except:
        pass

    pygame.display.update()

def getscore():
    goals_read = read_motor_data("goals")
    goals_arr = goals_read.split(":")
    return goals_arr


def scoreboard(screen, score_arr):
    SCORE_FONT = pygame.font.SysFont('comicsans',70)

    RED = (200,50,0)
    YELLOW = (255,200,0)
    WHITE = (255,255,255)

    old_score = score_arr
    score_arr = getscore()
    top = 363 
    try:
        if score_arr[0] != old_score[0]:
            YELLOW_BOX = pygame.Rect(860,top,110,150)
            pygame.draw.rect(screen, YELLOW, YELLOW_BOX)
        if score_arr[1] != old_score[1]:
            RED_BOX = pygame.Rect(1047,top,110,150)
            pygame.draw.rect(screen, RED, RED_BOX)
        
        yellow_rawscore = score_arr[0]
        red_rawscore = score_arr[1]

        red_scoreboard = SCORE_FONT.render(str(red_rawscore), 1, WHITE)
        yellow_scoreboard = SCORE_FONT.render(str(yellow_rawscore), 1, WHITE)
     
     
        screen.blit(red_scoreboard, (1080, 385))
        screen.blit(yellow_scoreboard, (890, 385))
    except:
        pass
     



    pygame.display.update()

# screen = game_setup()

# while True:
#     game_main(screen, motor_data)
