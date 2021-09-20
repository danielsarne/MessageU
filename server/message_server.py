import socket
import selectors
import threading
from consts import *
from db_manager import DBManger
from client_request import ClientHandler


class ServerError(Exception):
    pass

class MessageServer:
    version = 2

    def __init__(self):
        with open(SERVER_PORT_PATH, "r") as port_file:
            self.port = int(port_file.read())
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.bind(("0.0.0.0", self.port))
        self.socket.settimeout(1)
        self.clients = []
        self.db_manager = DBManger("clients.db")

    def run(self):
        while True:
            client_conn = self.socket.accept()
            client_handler = ClientHandler(client_conn)
            client_thread = threading.Thread(client_handler.handle)
            client_thread.run()
