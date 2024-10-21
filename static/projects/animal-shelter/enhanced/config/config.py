import os


class Config:
    SECRET_KEY = os.environ.get("SECRET_KEY") or "your-secret-key"
    SQLALCHEMY_DATABASE_URI = os.environ.get("DATABASE_URL") or "sqlite:///animals.db"
    SQLALCHEMY_TRACK_MODIFICATIONS = False

    def __init__(self) -> None:
        if self.SECRET_KEY == 'your-secret-key':
            print("WARNING: SECRET_KEY is not set. Using 'your-secret-key'")
