from http.server import SimpleHTTPRequestHandler, HTTPServer
import cgi
import socket

class webserverHandler(SimpleHTTPRequestHandler):
    """docstring for webserverHandler"""

    def do_GET(self):
        try:
            if self.path.endswith("/hello"):
                self.send_response(200)
                self.send_header('Content-Type', 'text/html')
                self.end_headers()

                output = ""
                output += '<html><body>Hello!'
                output += '<form method="POST" enctype="multipart/form-data" action="/hello"><h2> What would you like me to say?</h2><input name="message" type="text" /><input type="submit" value="Submit" /></form>'
                output += '</body></html>'
                self.wfile.write(output.encode())
                print(output)
                return

            if self.path.endswith("/hola"):
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
        try:
            self.send_response(301)
            self.send_header('Content-Type', 'text/html')
            self.end_headers()
            ctype, pdict = cgi.parse_header(self.headers.get('Content-Type'))
            pdict['boundary'] = bytes(pdict['boundary'], "utf-8")
            if ctype == 'multipart/form-data':
                fields = cgi.parse_multipart(self.rfile, pdict)
                messagecontent = fields.get('message')
            output = ''
            output += '<html><body>'
            output += '<h2> Okay, how about this: </h2>'
            output += '<h1> %s </h1>' % messagecontent[0].decode("utf-8")
            output += '<form method="POST" enctype="multipart/form-data" action="/hello"><h2> What would you like me to say?</h2><input name="message" type="text" /><input type="submit" value="Submit" /></form>'
            output += '</body></html>'
            self.wfile.write(output.encode())
            print(output)
        except:
            self.send_error(404, "{}".format(sys.exc_info()[0]))
            print(sys.exc_info())

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