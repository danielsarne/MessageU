import socket
import selectors
import threading
from consts import *
from client_request import ClientRequest


class MessageServer:
    version = 2

    def __init__(self):
        with open(SERVER_PORT_PATH, "r") as port_file:
            self.port = int(port_file.read())
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.bind(("0.0.0.0", self.port))
        self.socket.settimeout(1)
        self.clients = []
        self.db_manager = DBManager("clients.db")

    def run(self):
        while True:
            client_conn = self.socket.accept()
            client_handler = ClientRequest(client_conn)
            client_thread = threading.Thread(client_handler.handle)
            client_thread.run()

    def user_exists(self, username):
        pass

    def register_user(self, username):
        pass
