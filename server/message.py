from exceptions import ServerError
import secrets
import struct

MESSAGE_HEADER_LEN = 21
MESSAGE_ID_LEN = 4


class Message:
    def __init__(self, message_id: bytes, dst_client_uid: bytearray, src_client_uid: bytearray, message_type: int,
                 content: bytearray):
        self.id = message_id
        self.dst_client_uid = dst_client_uid
        self.src_client_uid = src_client_uid
        self.type = message_type
        self.content = content

    @classmethod
    def from_request(cls, request):
        message = cls()
        message.src_client_uid = request.uid
        message.dst_client_uid, message.type, content_len, message.content = struct.unpack(
            "<16sHI{content_len}%s".format(content_len=len(request.payload) - MESSAGE_HEADER_LEN), request.payload)
        if len(message.content) != content_len:
            raise ServerError("content len not compatible with the header.")
        message.id = secrets.token_bytes(MESSAGE_ID_LEN)
        return message
