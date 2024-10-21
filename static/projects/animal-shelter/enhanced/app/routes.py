from flask import Blueprint, render_template, jsonify, request
from flask_login import login_required, current_user
from app.models import Animal
from app.crud import AnimalShelter

from flask import current_app

main = Blueprint("main", __name__)


@main.route("/")
def index():
    import os

    # return render_template("index.html")
    with current_app.app_context():
        print("Current working directory:", os.getcwd())

        return render_template("index.html")


@main.route("/api/animals", methods=["GET"])
def get_animals():
    shelter = AnimalShelter()
    animals = shelter.read({})
    return jsonify(animals)


@main.route("/admin")
@login_required
def admin():
    return render_template("admin.html")
