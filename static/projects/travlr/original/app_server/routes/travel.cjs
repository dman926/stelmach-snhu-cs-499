const express = require("express");
const router = express.Router();
const controller = require("../controllers/travel.cjs");

/* GET users listing. */
router.get("/", controller.travel);

module.exports = router;
