from http.server import SimpleHTTPRequestHandler, HTTPServer
from file_read import read_motor_data
import cgi
import socket
import os



class webserverHandler(SimpleHTTPRequestHandler):
    """docstring for webserverHandler"""
    buf = True
    def __init__(self, *args, directory=None, **kwargs):
        SimpleHTTPRequestHandler.__init__(self, *args, directory=None, **kwargs)
        self.buf = True

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
            # Save the image data to a file
            with open("uploaded_image.jpg", "wb") as image_file:
                image_file.write(image_data)
        # Respond with a success message
        self.send_response(200)

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
        server = HTTPServer((host, port), webserverHandler)
        server.timeout = 0.1
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

server = setup_server()


# server.handle_request_noblock()