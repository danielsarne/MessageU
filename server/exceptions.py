class ServerError(Exception):
    pass


class UserExists(ServerError):
    def __init__(self, username, message="User {} already exists"):
        formatted_message = message.format(username)
        super().__init__(formatted_message)


class UserDoesNotExists(ServerError):
    def __init__(self, username, message="User {} Doesn't exist"):
        formatted_message = message.format(username)
        super().__init__(formatted_message)
