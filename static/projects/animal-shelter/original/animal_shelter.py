from pymongo import MongoClient


class AnimalShelter:
    """CRUD operations for Animal collection in MongoDB"""

    # A proper package outside of the class would not use defaults.
    # Defaults are just for assignment purposes
    def __init__(
        self,
        USER="aacuser",
        PASS="super_secret_password_123",
        HOST="nv-desktop-services.apporto.com",
        PORT=32431,
        DB="AAC",
        COL="animals",
    ):
        # Initializing the MongoClient. This helps to
        # access the MongoDB databases and collections.
        # This is hard-wired to use the aac database, the
        # animals collection, and the aac user.
        #
        # Initialize Connection
        #
        self.client = MongoClient("mongodb://%s:%s@%s:%d" % (USER, PASS, HOST, PORT))
        self.database = self.client["%s" % (DB)]
        self.collection = self.database["%s" % (COL)]

    def _precheckParam(self, **kwargs):
        for key, data in kwargs.items():
            if data is None:
                raise Exception(f"{key} parameter must be defined")
            elif not isinstance(data, dict):
                raise Exception(f"{key} parameter must be a key/value map")
            elif len(data) == 0:
                raise Exception(f"{key} parameter must not be empty")

    def create(self, data: dict) -> bool:
        """
        Document Creation
        """
        self._precheckParam(data=data)

        result = self.database.animals.insert_one(data)

        # Return the insert result
        return result is not None and result.acknowledged

    def read(self, data: dict) -> list[dict]:
        """
        Document Reading
        """
        self._precheckParam(data=data)

        # Return all the documents found as a list
        return list(self.database.animals.find(data))

    def update(self, idData: dict, updateData: dict) -> int:
        """
        Document Updating
        """
        self._precheckParam(idData=idData, updateData=updateData)

        result = self.database.animals.update_many(idData, updateData)

        if result is None:
            # Update failed for some reason
            return 0
        # Return the number of updated documents
        return result.modified_count

    def delete(self, idData: dict) -> int:
        """
        Document Deletion
        """
        self._precheckParam(idData=idData)

        result = self.database.animals.delete_many(idData)

        if result is None:
            # Delete failed for some reason
            return 0
        # Return the number of deleted documents
        return result.deleted_count
