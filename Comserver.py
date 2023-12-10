from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from file_read import read_motor_data, write_motor_data
import cgi
import socket
import os


# This object defines how the http server deals with requests
class webserverHandler(SimpleHTTPRequestHandler):
    """docstring for webserverHandler"""

    # Defining how the server responds to HTTP get requests
    def do_GET(self):
        try:
            # When http//(server address)/motor is called
            # This address is only called by the ESP32 controlling the motors, and returns the keyboard input stored in the motor_data file
            if self.path.endswith("/motor"):
                self.send_response(200)
                self.send_header('Content-Type', 'data')
                self.end_headers()

                # Reads data from the motor_data file
                data = read_motor_data("motor_data")
                output = ""
                # Sends data to the client
                output += data
                self.wfile.write(output.encode())
                print(output)
                return

            # When http//(server address)/pla is called
            # This address is requested by the ESP32 with ultrasonic sensors, and is called when a goal is scored by the player
            if self.path.endswith("/pla"):
                self.send_response(200)
                # Tells the ESP32 the goal has been registered
                self.send_header('Content-Type', 'response')
                self.end_headers()
                output = ""
                self.wfile.write(output.encode())

                # Reads the current record of goals scored
                goal_data = read_motor_data("goals")
                # Stores current goals into an array
                goal_data = goal_data.rsplit(":")
                for x in range(len(goal_data)):
                    goal_data[x] = int(goal_data[x])

                # Increment the corresponding goal
                goal_data[0] +=1
                goal_data = str(goal_data[0]) + ":" + str(goal_data[1])

                # write the goal data back into the file
                write_motor_data("goals",goal_data)
                return
            
            # When http//(server address)/opp is called
            # This address is requested by the ESP32 with ultrasonic sensors, and is called when a goal is scored by the player
            if self.path.endswith("/opp"):
                # Tells the ESP32 the goal has been registered
                self.send_response(200)
                self.send_header('Content-Type', 'response')
                self.end_headers()
                output = ""
                self.wfile.write(output.encode())

                 # Reads the current record of goals scored
                goal_data = read_motor_data("goals")

                # Stores current goals into an array
                goal_data = goal_data.rsplit(":")
                for x in range(len(goal_data)):
                    goal_data[x] = int(goal_data[x])

                # Increment the corresponding goal
                goal_data[1] +=1
                goal_data = str(goal_data[0]) + ":" + str(goal_data[1])
                # write the goal data back into the file
                write_motor_data("goals",goal_data)
                return

        except IOError:
            self.send_error(404, "File not found %s" % self.path)

     # Defining how the server responds to HTTP POST requests
    #  These requests are only sent by the ESP-CAM
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        content_type = self.headers['Content-Type']

        if content_type.startswith('multipart/form-data'):
            # Process the multipart data
            form_data = self.rfile.read(content_length)
            image_data = form_data
            self.send_response(200)
            # Save the image data to a file
            try:
                with open("uploaded_image.jpg", "wb") as image_file:
                    image_file.write(image_data)
            except:
                pass

        elif content_type.startswith('trainingdata'):
            # Process the multipart data
            form_data = self.rfile.read(content_length)
            image_data = form_data
            self.send_response(200)
            # Save the image data to a file
            file_name = os.path.join("trainingData", read_motor_data("datacount") +".jpg")
            with open(file_name, "wb") as image_file:
                image_file.write(image_data)
            file_name = int(read_motor_data("datacount")) + 1
            write_motor_data("datacount", file_name)
        
# This function prints the ip address the server is being hosted on
def get_local_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # doesn't even have to be reachable
        s.connect(('192.255.255.255', 1))
        IP = s.getsockname()[0]
    except:
        IP = '127.0.0.1'
    finally:
        s.close()
    return IP


# this function runs once to set up the server
def setup_server():

    try:
        host = get_local_ip()
        port = 8000
        server = ThreadingHTTPServer((host, port), webserverHandler)
        server.timeout = 0.1
        print("Web server running on {}:{}".format(host, port))
        return server
    except:
        pass

# This function runs continously
def run_server(server):
    try:
        # server.serve_forever()
        server.handle_request()
        return True
    except KeyboardInterrupt:
        print(" ^C entered stopping web server...")
        server.socket.close()
        return False

# server = setup_server()

# while True:
#     run_server(server)
#     print("run")


# server.handle_request_noblock()