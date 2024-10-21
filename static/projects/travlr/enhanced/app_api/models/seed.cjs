const Mongoose = require("./db.cjs");
const Trip = require("./travlr.cjs");
const fs = require("fs");

const trips = JSON.parse(fs.readFileSync("./data/trips.json", "utf8"));

const seedDb = async () => {
  await Trip.deleteMany();
  await Trip.insertMany(trips);
};

seedDb().then(async () => {
  await Mongoose.connection.close();
  process.exit(0);
});
