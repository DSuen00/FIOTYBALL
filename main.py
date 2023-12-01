from Comserver import setup_server, run_server
from game import game_main, game_setup

server = setup_server()
screen = game_setup()

running = True

while running:
    run_server(server)
    game_main(screen)
