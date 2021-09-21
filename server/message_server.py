import socket
import selectors
import threading
from consts import *
import logging
from client_request import ClientRequest

logger = logging.getLogger(__name__)


class MessageServer:
    version = 2

    def __init__(self):
        with open(SERVER_PORT_PATH, "r") as port_file:
            self.port = int(port_file.read())
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.bind(("0.0.0.0", self.port))
        self.socket.settimeout(1)
        self.db_manager = DBManager("server.db")
        logging.info("server starting running")

    def run(self):
        while True:
            client_conn, addr = self.socket.accept()
            logger.info("recieved another client request")
            client_handler = ClientRequest(client_conn, self)
            client_thread = threading.Thread(target=client_handler.parse)
            client_thread.run()

    def user_exists(self, username):
        pass

    def register_user(self, username):
        pass
