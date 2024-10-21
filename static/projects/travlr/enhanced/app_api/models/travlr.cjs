/**
 * Developer: Dennis James Stelmach
 * Date: 2024-10-06
 * 
 * The intent of the enhancement is to prevent malformed data
 * from being stored in the database, which is then displayed
 * to the user in the browser.
 */

const mongoose = require("mongoose");

const tripSchema = new mongoose.Schema({
  code: { type: String, required: true, index: true },
  name: { type: String, required: true, index: true },
  length: { type: String, required: true },
  start: { type: Date, required: true },
  resort: { type: String, required: true },
  perPerson: { type: Number, required: true },
  image: { type: String, required: true },
  description: { type: String, required: true },
});

// Middleware to validate price format
tripSchema.pre("save", function(next) {
  if (typeof this.perPerson !== "number" || this.perPerson <= 0) {
    const err = new Error("Invalid price format. Should be a positive number");
    err.name = "ValidationError";
    return next(err);
  }
  next();
});

// Middleware to sanitize description field
tripSchema.pre("save", function(next) {
  this.description = this.description.trim();
  next();
});

const Trip = mongoose.model("trips", tripSchema);

module.exports = Trip;
