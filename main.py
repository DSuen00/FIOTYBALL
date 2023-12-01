from Comserver import setup_server, run_server
from game import game_main, game_setup

server = setup_server()
screen = game_setup()
motor_data = [1,1,1,1]
score_arr = [0,0]

running = True

while running:
    run_server(server)
    game_main(screen,motor_data, score_arr)