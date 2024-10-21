const passport = require("passport");
const LocalStrategy = require("passport-local").Strategy;
const mongoose = require("mongoose");
const User = mongoose.model("users");

passport.use(
  new LocalStrategy(
    { usernameField: "email" },
    async (username, password, done) => {
      const q = await User.findOne({ email: username }).exec();

      /**
       * "Full Stack Guide" describes individual error messages,
       * but this leaks sensitive information about the attempt,
       * and is insecure, as it allows an attacker to guess usernames
       * and know when a valid username is given, which they can then target
       * to breach the password.
       * 
       * The solution is to combine them into a single non-descript message
       * and add a small randomized delay to help prevent timing attacks.
       */

       // Random delay to help mitigate timing attacks
      const randomDelay = Math.random() * 100; // Delay between 0 and 100 milliseconds
      await new Promise(resolve => setTimeout(resolve, randomDelay));

      // Validate user
      if (!q || !q.validPassword(password)) {
        return done(null, false, { message: "Incorrect Username or Password" });
      }

      return done(null, q);
    }
  )
);
