from flask import Flask
from flask_login import LoginManager
from config import Config
from flask_wtf.csrf import CSRFProtect

login_manager = LoginManager()
csrf = CSRFProtect()


def create_app():
    app = Flask(__name__, template_folder="../templates", static_folder="../static")
    app.config.from_object(Config)

    from app.models import init_db

    init_db(app)
    login_manager.init_app(app)
    csrf.init_app(app)

    from app.routes import main

    app.register_blueprint(main)

    from app.auth import auth

    app.register_blueprint(auth)

    return app
