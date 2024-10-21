const mongoose = require("mongoose");
const Trip = require("../models/travlr.cjs");
const Model = mongoose.model("trips");

/**
 * GET: /trips - list all the trips
 * Regardless of the outcome, reesponse must include HTML status code
 * and JSON message to the requesting client
 *
 * @type {import('express').RequestHandler}
 */
const tripsList = async (req, res) => {
  const q = await Model.find({})
    .exec()
    .catch((err) => err);

  if (!q || q instanceof Error) {
    res.status(404).json(q);
  } else {
    res.status(200).json(q);
  }

  return;
};

/**
 * GET: /trips/:tripCode - list a single trip
 * Regardless of the outcome, reesponse must include HTML status code
 * and JSON message to the requesting client
 *
 * @type {import('express').RequestHandler}
 */
const tripsFindByCode = async (req, res) => {
  const q = await Model.find({ code: req.params.tripCode })
    .exec()
    .catch((err) => err);

  if (!q || q instanceof Error) {
    res.status(404).json(q);
  } else {
    res.status(200).json(q);
  }

  return;
};

/**
 * POST: /trips - add a new trip
 * Regardless of the outcome, reesponse must include HTML status code
 * and JSON message to the requesting client
 *
 * @type {import('express').RequestHandler}
 */
const tripAddTrip = async (req, res) => {
  const newTrip = new Trip({
    ...req.body,
  });
  const q = await newTrip.save().catch((err) => err);
  if (!q || q instanceof Error) {
    res.status(404).json(q);
  } else {
    res.status(201).json(q);
  }

  return;
};

/**
 * PUT: /trips/:tripCode - edit a trip
 * Regardless of the outcome, reesponse must include HTML status code
 * and JSON message to the requesting client
 *
 * @type {import('express').RequestHandler}
 */
const tripUpdateTrip = async (req, res) => {
  const q = await Model.findOneAndUpdate(
    { code: req.params.tripCode },
    { ...req.body }
  )
    .exec()
    .catch((err) => err);

  if (!q || q instanceof Error) {
    res.status(404).json(q);
  } else {
    res.status(201).json(q);
  }

  return;
};

module.exports = {
  tripsList,
  tripsFindByCode,
  tripAddTrip,
  tripUpdateTrip,
};
