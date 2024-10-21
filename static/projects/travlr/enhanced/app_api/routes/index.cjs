const express = require("express");
const jwt = require("jsonwebtoken"); // Enable JSON Web Tokens
const router = express.Router();

/**
 * Method to authenticate our JWT
 * @type {import('express').RequestHandler}
 */
function authenticateJWT(req, res, next) {
  // console.log('In Middleware');
  const authHeader = req.headers["authorization"];
  // console.log('Auth Header: ' + authHeader);
  if (authHeader == null) {
    console.log("Auth Header Required but NOT PRESENT!");
    res.sendStatus(401);
    return
  }
  let headers = authHeader.split(" ");
  if (headers.length < 1) {
    console.log("Not enough tokens in Auth Header: " + headers.length);
    res.sendStatus(501);
    return
  }
  const token = authHeader.split(" ")[1];
  // console.log('Token: ' + token);
  if (token == null) {
    console.log("Null Bearer Token");
    res.sendStatus(401);
    return
  }
  // console.log(process.env.JWT_SECRET);
  // console.log(jwt.decode(token));
  const verified = jwt.verify(
    token,
    process.env.JWT_SECRET,
    (err, verified) => {
      if (err) {
        return res.sendStatus(401).json("Token Validation Error!");
      }
      req.auth = verified; // Set the auth paramto the decoded object
    }
  );
  next(); // We need to continue or this will hang forever
}

const authController = require("../controllers/authentication.cjs");
const tripsController = require("../controllers/trips.cjs");

router.route("/register").post(authController.register);
router.route("/login").post(authController.login);

router
  .route("/trips")
  .get(tripsController.tripsList)
  .post(authenticateJWT, tripsController.tripAddTrip);

router
  .route("/trips/:tripCode")
  .get(tripsController.tripsFindByCode)
  .put(authenticateJWT, tripsController.tripUpdateTrip);

module.exports = router;
