import socket
from hashlib import md5
from binascii import a2b_hex
import threading
from collections import namedtuple
from consts import *
import logging
from client_request import ClientRequest
from db_manager import DBManager

logger = logging.getLogger(__name__)


class MessageServer:
    version = SERVER_VERSION

    def __init__(self):
        with open(SERVER_PORT_PATH, "r") as port_file:
            self.port = int(port_file.read())
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.bind(("0.0.0.0", self.port))
        self.socket.listen(5)
        self.db_manager = DBManager("server.db")
        logging.info("server starting running")

    def run(self):
        while True:
            client_conn, addr = self.socket.accept()
            logger.info("recieved another client request")
            client_handler = ClientRequest(client_conn, self)
            client_thread = threading.Thread(target=client_handler.parse)
            client_thread.run()

    def register_user(self, username, public_key):
        uid = a2b_hex(md5(username.encode("utf-8")).hexdigest().encode("utf-8"))
        username = username.ljust(255, '\0')
        self.db_manager.register_user(uid, username, public_key)
        print("DEBUG:", uid)
        return uid
