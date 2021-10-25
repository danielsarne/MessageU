import struct
import sqlite3
import time


class DBManager:
    def __init__(self, db_name):
        self.connection = sqlite3.connect(db_name)
        self.cur = self.connection.cursor()
        if not self._db_exists():
            self._create_client_table()

    def _db_exists(self):
        tables = self.cur.execute(
            "SELECT name FROM sqlite_master WHERE type='table'").fetchall()
        return tables != []

    def _create_client_table(self):
        self.cur.execute("""CREATE TABLE `clients` (
    `id`	BLOB NOT NULL UNIQUE,
    `name`	TEXT NOT NULL UNIQUE,
    `public_key`	BLOB NOT NULL,
    `last_seen`	INTEGER NOT NULL,
    PRIMARY KEY(`id`));""")

    def user_exists(self, username):
        return self.cur.execute("SELECT ID FROM clients WHERE name=?", tuple([username])).fetchall() != []

    def register_user(self, uid, username, public_key):
        self.cur.execute("INSERT INTO clients VALUES (?, ?, ?, ?)",
                         (uid, username, public_key, int(time.time())))
        self.connection.commit()

    def __del__(self):
        self.connection.close()
