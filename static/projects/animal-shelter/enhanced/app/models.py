from app import login_manager
from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import MetaData, String
from sqlalchemy.orm import DeclarativeBase, Mapped, mapped_column
from flask_bcrypt import Bcrypt
from flask_login import UserMixin


class BaseModel(DeclarativeBase):
    metadata = MetaData(
        naming_convention={
            "ix": "ix_%(column_0_label)s",
            "uq": "uq_%(table_name)s_%(column_0_name)s",
            "ck": "ck_%(table_name)s_%(constraint_name)s",
            "fk": "fk_%(table_name)s_%(column_0_name)s_%(referred_table_name)s",
            "pk": "pk_%(table_name)s",
        }
    )


db = SQLAlchemy(model_class=BaseModel)
bcrypt = Bcrypt()


def init_db(app: Flask):
    db.init_app(app)
    bcrypt.init_app(app)
    with app.app_context():
        db.create_all()


class User(UserMixin, db.Model):
    id: Mapped[int] = mapped_column(primary_key=True)
    username: Mapped[str] = mapped_column(String(64), unique=True, nullable=False)
    password_hash: Mapped[str] = mapped_column(String(128))

    def set_password(self, password):
        self.password_hash = bcrypt.generate_password_hash(password).decode("utf-8")

    def check_password(self, password):
        return bcrypt.check_password_hash(self.password_hash, password)

    def __repr__(self):
        return f"<User {self.username}>"


@login_manager.user_loader
def load_user(id):
    return User.query.get(int(id))


class Animal(db.Model):
    id: Mapped[int] = mapped_column(db.Integer, primary_key=True)
    name: Mapped[str] = mapped_column(String(64), nullable=False)
    breed: Mapped[str] = mapped_column(String(64))
    age: Mapped[int]

    def __repr__(self):
        return f"<Animal {self.name} <{self.id}>>"
