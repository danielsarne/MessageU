import struct
import sqlite3
import time

from client import Client
from message import Message


class DBManager:
    def __init__(self, db_name):
        self.connection = sqlite3.connect(db_name)
        self.cur = self.connection.cursor()
        if not self._db_exists():
            self._create_clients_table()
            self._create_messages_table()

    def _db_exists(self):
        tables = self.cur.execute(
            "SELECT name FROM sqlite_master WHERE type='table'").fetchall()
        return tables != []

    def _create_clients_table(self):
        self.cur.execute("""CREATE TABLE `clients` (
    `id`	BLOB NOT NULL UNIQUE,
    `name`	TEXT NOT NULL UNIQUE,
    `public_key`	BLOB NOT NULL,
    `last_seen`	INTEGER NOT NULL,
    PRIMARY KEY(`id`));""")

    def _create_messages_table(self):
        self.cur.execute("""CREATE TABLE `messages` (
            `id`	INTEGER NOT NULL UNIQUE,
            `to_client`	BLOB NOT NULL,
            `from_client`	BLOB NOT NULL,
            `type`	INTEGER NOT NULL,
            `content` BLOB NOT NULL,
            PRIMARY KEY(`id`));""")

    def user_exists(self, username):
        print(self.cur.execute("SELECT ID FROM clients WHERE name=?", tuple([username])).fetchall())
        return self.cur.execute("SELECT ID FROM clients WHERE name=?", tuple([username])).fetchall() != []

    def get_client_by_uid(self, uid):
        return Client(*self.cur.execute("SELECT * FROM clients WHERE id=?", tuple([uid])).fetchall()[0])

    def uid_exists(self, uid: bytearray):
        return self.cur.execute("SELECT ID FROM clients WHERE id=?", tuple([uid])).fetchall() != []

    def get_clients(self):
        return [Client(*client_entry) for client_entry in self.cur.execute("SELECT * FROM clients").fetchall()]

    def register_user(self, uid, username, public_key):
        self.cur.execute("INSERT INTO clients VALUES (?, ?, ?, ?)",
                         (uid, username, public_key, int(time.time())))
        self.connection.commit()

    def pull_client_messages(self, uid):
        messages = [Message(*message_entry) for message_entry in
                self.cur.execute("SELECT * FROM messages WHERE to_client=?", tuple([uid])).fetchall()]
        self.cur.execute("DELETE * FROM messages WHERE to_client=?", tuple([uid]))
        return messages

    def __del__(self):
        self.connection.close()
