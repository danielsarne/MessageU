import struct
import socket
import threading
import abc
from client_request import ClientRequest
from db_manager import DBManager
from exceptions import UserExists, UserDoesNotExists

db_manager = DBManager("client.db")


class RequestHandler:
    def __init__(self, request):
        self.request = request
        self.server = self.request.server

    @abc.abstractmethod
    def handle(self):
        pass


class SignUpRequestHandler(RequestHandler):
    code = 1000

    def __init__(self, request):
        # type: (ClientRequest) -> SignUpRequestHandler
        super().__init__(request)
        cname, self.public_key = struct.unpack("<255s160s", self.request.payload)
        self.name = cname.split('\0')[0]

    def handle(self):
        if self.server.user_exists(self.name):
            raise UserExists(self.name)
        else:
            self.server.register(self.name, self.public_key)
