from collections import namedtuple

Message = namedtuple("Message", ["id", "dst_client_uid", "src_client_uid", "type", "content"])
