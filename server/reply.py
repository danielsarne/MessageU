import struct
from message import Message
from consts import SERVER_VERSION


class ServerReply:
    version = SERVER_VERSION

    def get_bytes(self):
        payload_len = len(self.payload)
        return struct.pack("<BHI{payload_len}s".format(payload_len=payload_len),
                           self.version, self.code, payload_len, self.payload)

    @property
    def code(self):
        raise NotImplementedError("should use one of derived replies.")


class GeneralServerErrorReply(ServerReply):
    code = 9000

    def __init__(self):
        self.payload = b''


class SuccessfullSignupReply(ServerReply):
    code = 2000

    def __init__(self, uid):
        self.payload = uid


class ClientListReply(ServerReply):
    code = 2001

    def __init__(self, clients):
        self.payload = b''
        for client in clients:
            self.payload += client.uid
            self.payload += client.name.encode()


class ClientPublicKeyReply(ServerReply):
    code = 2002

    def __init__(self, client):
        self.payload = b''
        self.payload += client.uid + client.public_key


class MessageSentReply(ServerReply):
    code = 2003

    def __init__(self, message: Message):
        self.payload = b''
        self.payload += message.dst_client_uid + message.id


class MessagesListReply(ServerReply):
    code = 2004

    def __init__(self, messages):
        self.payload = b''
        print(messages)
        for message in messages:
            print("message content:", message.content)
            self.payload += struct.pack("<16s4sBI{content_size}s".format(content_size=len(message.content)),
                                        message.src_client_uid, message.id, message.type, len(message.content),
                                        message.content)
