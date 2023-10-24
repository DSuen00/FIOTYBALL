# import the pygame module, so you can use it
import pygame
 
# define a main function
def main():
     
    # initialize the pygame module
    pygame.init()

     
    # create a surface on screen that has the size of 240 x 180
    screen = pygame.display.set_mode((1000,700))
     
    # define a variable to control the main loop
    running = True
     
    # main loop
    while running:
        # event handling, gets all event from the event queue
        for event in pygame.event.get():
            # only do something if the event is of type QUIT
            if event.type == pygame.QUIT:
                # change the value to False, to exit the main loop
                running = False
        try:
            image = pygame.image.load("uploaded_image.jpg")
            screen.blit(image, (50,50))
        except:
            pass
        pygame.display.update()
     
     
# run the main function only if this module is executed as the main script
# (if you import this as a module then nothing is executed)
if __name__=="__main__":
    # call the main function
    main()