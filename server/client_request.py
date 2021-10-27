import struct
import socket
from threading import Lock
from request_handler import SignUpRequestHandler, GetClientListHandler, GetClientPublicKeyHandler, PullMessagesHandler, \
    SendMessageHandler
from exceptions import ServerError
from reply import GeneralServerErrorReply
from consts import *
import logging

client_locks = {}

logger = logging.getLogger(__name__)


class ClientRequest:
    REQUESTS = [SignUpRequestHandler, GetClientListHandler, GetClientPublicKeyHandler, PullMessagesHandler,
                SendMessageHandler]
    request_handlers = dict([(request.code, request) for request in REQUESTS])

    def __init__(self, socket: socket.socket, server):
        self.socket = socket
        self.server = server
        self.payload = b''

    def recv_request(self):
        # type: () -> None
        header_data = self.socket.recv(HEADER_LENGTH)
        print(len(header_data), header_data)
        self.uid, self.version, self.code, self.payload_size = struct.unpack("<16sBHI", header_data)
        print(self.uid, self.version, self.code, self.payload_size)
        if self.payload_size == 0:
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
            if self.uid not in client_locks:
                client_locks[self.uid] = Lock()
            with client_locks[self.uid]:
                request_handler = self.request_handlers[self.code](self)
                reply = request_handler.handle()
                self.socket.send(reply.get_bytes())
        except ServerError as e:
            logger.error(e)
            self.socket.send(GeneralServerErrorReply().get_bytes())
