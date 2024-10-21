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

const Trip = mongoose.model("trips", tripSchema);

module.exports = Trip;
