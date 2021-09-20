import struct
import socket
import threading
import abc


class RequestHandler:
    def __init__(self, request):
        self.request = request

    @abc.abstractmethod
    def handle(self):
        pass
