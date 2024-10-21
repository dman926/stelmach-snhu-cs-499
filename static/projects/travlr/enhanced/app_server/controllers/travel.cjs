const tripsEndpoint = "http://localhost:3000/api/trips";
const options = {
  method: "GET",
  headers: {
    "Content-Type": "application/json",
  },
};

/**
 * GET travel view
 *
 * @type {import('express').RequestHandler}
 */
const travel = async (req, res) => {
  await fetch(tripsEndpoint, options)
    .then((res) => res.json())
    .then((trips) => {
      let message = null;
      if (!Array.isArray(trips)) {
        message = "API lookup error";
        trips = [];
      } else {
        if (!trips.length) {
          message = "No trips exist in our database!";
        }
      }
      res.render("travel", { title: "Travlr Gateways", trips, message });
    })
    .catch((err) => {
      res.status(500).send(err.message);
    });
};

module.exports = {
  travel,
};
