from http.server import SimpleHTTPRequestHandler, HTTPServer
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

                output = ""
                output += '<html><body>Hello!'
                output += '<form method="POST" enctype="multipart/form-data" action="/hello"><h2> What would you like me to say?</h2><input name="message" type="text" /><input type="submit" value="Submit" /></form>'
                output += '</body></html>'
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
            # Split the form data into individual parts using the boundary
            # if "image/jpeg" in form_data:
                # Extract the image data
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
 
def main():
    try:
        
        host = get_local_ip()
        port = 8000
        server = HTTPServer((host, port), webserverHandler)
        print("Web server running on {}:{}".format(host, port))
        server.serve_forever()
    except KeyboardInterrupt:
        print(" ^C entered stopping web server...")
        server.socket.close()

if __name__ == '__main__':
        main()