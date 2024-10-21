/**
 * Developer: Dennis James Stelmach
 * Date: 2024-10-06
 * 
 * The intent of the enhancement is to prevent malformed data
 * from being stored in the database, which is then displayed
 * to the user in the browser.
 */

const mongoose = require("mongoose");
const crypto = require("crypto");
const jwt = require("jsonwebtoken");

// Mongoose doesn't play well with types, so manual intervention is needed

/**
 * @typedef {Object} IUser
 * @property {string} email
 * @property {string} name
 * @property {string} hash
 * @property {string} salt
 */

/**
 * @typedef {Object} IUserMethods
 * @property {(password: crypto.BinaryLike) => void} setPassword
 * @property {(password: crypto.BinaryLike) => boolean} validPassword
 * @property {() => string} generateJWT
 */

/**
 * @typedef {IUser & IUserMethods} UserDocument
 */

/**
 * @typedef {mongoose.Model<UserDocument> & {
 *   new(doc: IUser): mongoose.Document<unknown, {}, UserDocument> & UserDocument
 * }} UserModel
 */

/** @type {mongoose.Schema<UserDocument>} */
const userSchema = new mongoose.Schema({
  email: {
    type: String,
    unique: true,
    required: true,
  },
  name: {
    type: String,
    required: true,
  },
  hash: String,
  salt: String,
});

// Method to set the password on this record.
userSchema.methods.setPassword = function (
  /** @type {crypto.BinaryLike} */ password
) {
  this.salt = crypto.randomBytes(16).toString("hex");
  this.hash = crypto
    .pbkdf2Sync(password, this.salt, 1000, 64, "sha512")
    .toString("hex");
};

// Method to compare entered password against stored hash
userSchema.methods.validPassword = function (
  /** @type {crypto.BinaryLike} */ password
) {
  const hash = crypto
    .pbkdf2Sync(password, this.salt, 1000, 64, "sha512")
    .toString("hex");
  return this.hash === hash;
};

// Method to generate a JSON Web Token for the current record
userSchema.methods.generateJWT = function () {
  return jwt.sign(
    {
      // Payload for our JSON Web Token
      _id: this._id,
      email: this.email,
      name: this.name,
    },
    process.env.JWT_SECRET, // SECRET stored in .env file
    { expiresIn: "1h" }
  ); // Token expires an hour from creation
};

// Middleware to validate email format
userSchema.pre("save", function(next) {
  const emailRegex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
  if (!emailRegex.test(this.email)) {
    const err = new Error("Invalid email format");
    err.name = "ValidationError";
    return next(err);
  }
  next();
});

// Middleware to ensure password is set on create
userSchema.pre("save", function(next) {
  if (this.isNew && !this.hash) {
    const err = new Error("Password is required on create");
    err.name = "ValidationError";
    return next(err);
  }
  next();
});

// Middleware to sanitize email and name fields
userSchema.pre("save", function(next) {
  this.email = this.email.trim().toLowerCase();
  this.name = this.name.trim();
  next();
});

/** @type {UserModel} */
const User = mongoose.model("users", userSchema);
module.exports = User;
