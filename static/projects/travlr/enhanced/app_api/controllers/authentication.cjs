const mongoose = require("mongoose");
const passport = require("passport");
const User = require("../models/user.cjs");

/**
 * @type {import('express').RequestHandler}
 */
const register = async (req, res) => {
  if (!req.body.name || !req.body.email || !req.body.password) {
    res.status(400).json({
      message: "All fields required",
    });
    return;
  }

  const user = new User({
    name: req.body.name,
    email: req.body.email,
    salt: "",
  });
  user.setPassword(req.body.password);
  const q = await user.save();

  if (!q) {
    res.status(400).json({
      message: "Registration failed",
    });
    return;
  } else {
    const token = user.generateJWT();
    res.status(200).json(token);
  }
};

/**
 * @type {import('express').RequestHandler}
 */
const login = (req, res) => {
  // Validate message to ensure that email and password are present.
  if (!req.body.email || !req.body.password) {
    res.status(400).json({ message: "All fields required" });
    return;
  }
  // Delegate authentication to passport module
  passport.authenticate(
    "local",
    (
      /** @type {unknown} */ err,
      /** @type {User.UserDocument} */ user,
      /** @type {unknown} */ info
    ) => {
      if (err) {
        // Error in Authentication Process
        res.status(404).json(err);
        return;
      }
      if (user) {
        // Auth succeeded - generate JWT and return to caller
        const token = user.generateJWT();
        res.status(200).json({ token });
      } else {
        // Auth failed return error
        res.status(401).json(info);
      }
    }
  )(req, res);
};

module.exports = {
  register,
  login,
};
