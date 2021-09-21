import struct
import socket
from threading import Thread, Lock
from request_handler import SignUpRequestHandler
from message_server import ServerError
from consts import *
import logging

client_locks = {}

logger = logging.getLogger(__name__)


class ClientRequest:
    REQUESTS = [SignUpRequestHandler, ]
    request_handlers = dict([(request.code, request) for request in REQUESTS])

    def __init__(self, socket, server):
        # type: (socket.socket) -> ClientHandler
        self.socket = socket
        self.server = server
        self.payload = ""

    def recv_request(self):
        # type: () -> None
        header_data = self.socket.recv(UUID_LENGTH)
        self.uuid, self.version, self.code, self.payload_size = struct.unpack("<8sBHI", header_data)
        if self.payload_size is 0:
            return
        self.get_payload()

    def get_payload(self):
        # type: () -> None
        try:
            while True and len(self.payload) < self.payload_size:
                self.payload += self.socket.recv(2048)
        except socket.timeout:
            if len(self.payload) != self.payload_size:
                raise ServerError

    def send_reply(self, reply):
        self.socket.send(reply.to_hex())

    def parse(self):
        try:
            self.recv_request()
            if self.uuid not in client_locks:
                client_locks[self.uuid] = Lock()
            with client_locks[self.uuid]:
                request_handler = self.request_handlers[self.code](self)
                request_handler.handle()
        except ServerError as e:
            logger.error(e)
            self.socket.send(GeneralServerError().get_bytes())
