from exceptions import ServerError
import secrets
import struct
from typing import Optional

MESSAGE_HEADER_LEN = 21
MESSAGE_ID_LEN = 4


class Message:
    def __init__(self, message_id: Optional[bytes] = None, dst_client_uid: Optional[bytearray] = None,
                 src_client_uid: Optional[bytearray] = None,
                 message_type: Optional[int] = None,
                 content: Optional[bytearray] = None):
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
            "<16sBI{content_len}s".format(content_len=len(request.payload) - MESSAGE_HEADER_LEN), request.payload)
        if len(message.content) != content_len:
            raise ServerError(
                "content len not compatible with the header. {content_len} != {content_len_header}".format(
                    content_len=content_len, content_len_header=len(message.content)))
        message.id = secrets.token_bytes(MESSAGE_ID_LEN)
        return message
