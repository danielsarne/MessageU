import struct
import abc
from db_manager import DBManager
from message import Message
from exceptions import UserExists, UserDoesNotExists
from reply import ServerReply, SuccessfullSignupReply, ClientListReply, ClientPublicKeyReply, MessagesListReply, \
    MessageSentReply

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
        name, self.public_key = struct.unpack("<255s160s", self.request.payload)
        self.name = name.decode("utf-8")

    def handle(self) -> ServerReply:
        if self.server.db_manager.user_exists(self.name):
            raise UserExists(self.name)
        else:
            uid = self.server.register_user(self.name, self.public_key)
            print(SuccessfullSignupReply(uid).get_bytes())
            return SuccessfullSignupReply(uid)


class GetClientListHandler(RequestHandler):
    code = 1001

    def __init__(self, request):
        super().__init__(request)
        self.client_uid = request.uid

    def handle(self) -> ServerReply:
        if not self.server.db_manager.uid_exists(self.client_uid):
            raise UserDoesNotExists(self.client_uid)
        else:
            clients = [client for client in self.server.db_manager.get_clients() if client.uid != self.client_uid]
            print(clients)
            return ClientListReply(clients)


class GetClientPublicKeyHandler(RequestHandler):
    code = 1002

    def __init__(self, request):
        super().__init__(request)
        self.other_uid = request.payload

    def handle(self) -> ServerReply:
        if not self.server.db_manager.uid_exists(self.other_uid):
            raise UserDoesNotExists(self.other_uid)
        else:
            client = self.server.db_manager.get_client_by_uid(self.other_uid)
            return ClientPublicKeyReply(client)


class SendMessageHandler(RequestHandler):
    code = 1003

    def __init__(self, request):
        super().__init__(request)

    def handle(self) -> ServerReply:
        if not self.server.db_manager.uid_exists(self.request.uid):
            raise UserDoesNotExists(self.request.uid)
        else:
            m = Message.from_request(self.request)
            self.server.db_manager.register_message(m)
            return MessageSentReply(m)


class PullMessagesHandler(RequestHandler):
    code = 1004

    def __init__(self, request):
        super().__init__(request)
        self.uid = request.uid

    def handle(self) -> ServerReply:
        if not self.server.db_manager.uid_exists(self.uid):
            raise UserDoesNotExists(self.uid)
        else:
            messages = self.server.db_manager.pull_client_messages(self.uid)
            return MessagesListReply(messages)
