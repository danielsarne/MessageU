import struct
import secrets
from consts import SERVER_VERSION, UUID_LENGTH


class ServerReply:
    version = SERVER_VERSION

    def __init__(self, payload):
        self.payload = payload

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


class ClientListReply(ServerReply):
    code = 2001

    def __init__(self, clients):
        self.payload = b''
        for client in clients:
            padded_name = client.name.ljust(255, '\0')
            self.payload += client.uid
            self.payload += padded_name.encode()

