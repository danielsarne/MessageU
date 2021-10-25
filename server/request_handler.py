import struct
import abc
from db_manager import DBManager
from exceptions import UserExists, UserDoesNotExists
from reply import ServerReply, SuccessfullSignupReply, ClientListReply

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
        super().__init__(request)
        cname, self.public_key = struct.unpack("<255s160s", self.request.payload)
        self.name = cname.decode("utf-8").split('\0')[0]

    def handle(self) -> ServerReply:
        if self.server.user_exists(self.name):
            raise UserExists(self.name)
        else:
            uid = self.server.register_user(self.name, self.public_key)
            return SuccessfullSignupReply(uid)


class GetClientListHandler(RequestHandler):
    code = 1001

    def __init__(self, request):
        super().__init__(request)
        self.client_uid = request.uid

    def handle(self) -> ServerReply:
        if self.server.uid_exists(self.client_uid):
            raise UserDoesNotExists(self.client_uid.decode())
        else:
            clients = [client for client in self.server.get_clients() if client.uid != self.client_uid]
            return ClientListReply(clients)
