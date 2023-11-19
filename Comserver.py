from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from file_read import read_motor_data, write_motor_data
import cgi
import socket
import os



class webserverHandler(SimpleHTTPRequestHandler):
    """docstring for webserverHandler"""

    def do_GET(self):
        try:
            if self.path.endswith("/motor"):
                self.send_response(200)
                self.send_header('Content-Type', 'data')
                self.end_headers()
                data = read_motor_data("motor_data")
                output = ""
                output += data
                self.wfile.write(output.encode())
                print(output)
                return

            if self.path.endswith("/check"):
                self.send_response(200)
                self.send_header('Content-Type', 'text/html')
                self.end_headers()

                output = ""
                output += '<html><body>&#161Hola <a href="/hello">Back to Hello</a>'
                output += '<form method="POST" enctype="multipart/form-data" action="/hello"><h2> What would you like me to say?</h2><input name="message" type="text" /><input type="submit" value="Submit" /></form>'
                output += '</body></html>'
                self.wfile.write(output.encode())
                print(output)
                return

        except IOError:
            self.send_error(404, "File not found %s" % self.path)

    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        content_type = self.headers['Content-Type']

        if content_type.startswith('multipart/form-data'):
            # Process the multipart data
            form_data = self.rfile.read(content_length)
            image_data = form_data
            self.send_response(200)
            # Save the image data to a file
            with open("uploaded_image.jpg", "wb") as image_file:
                image_file.write(image_data)

        elif content_type.startswith('goal'):
            form_data = self.rfile.read(content_length)
            goal_data = read_motor_data("goals")
            goal_data = goal_data.rsplit(":")
            for x in range(len(goal_data)):
                goal_data[x] = int(goal_data[x])
            if form_data == "player":
                goal_data[0] +=1
            elif form_data == "opponent":
                goal_data[1]+=1
            else:
                self.send_response(100)
                return
            goal_data = str(goal_data[0]) + ":" + str(goal_data[1])
            write_motor_data(goal_data)
            self.send_response(200)


        # Respond with a success message
        

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
 
def setup_server():

    try:
        host = get_local_ip()
        port = 8000
        server = ThreadingHTTPServer((host, port), webserverHandler)
        server.timeout = 0.3
        print("Web server running on {}:{}".format(host, port))
        return server
    except:
        pass

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


# server.handle_request_noblock()