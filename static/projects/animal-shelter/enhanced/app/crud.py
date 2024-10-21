from app.models import db, Animal


class AnimalShelter:
    @staticmethod
    def create(data):
        animal = Animal(**data)
        db.session.add(animal)
        db.session.commit()
        return True

    @staticmethod
    def read(query):
        return Animal.query.filter_by(**query).all()

    @staticmethod
    def update(query, data):
        animals = Animal.query.filter_by(**query).all()
        for animal in animals:
            for key, value in data.items():
                setattr(animal, key, value)
        db.session.commit()
        return len(animals)

    @staticmethod
    def delete(query):
        animals = Animal.query.filter_by(**query).all()
        for animal in animals:
            db.session.delete(animal)
        db.session.commit()
        return len(animals)
