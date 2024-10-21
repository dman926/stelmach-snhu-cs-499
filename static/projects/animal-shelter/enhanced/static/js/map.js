let map;

function initMap() {
  map = L.map("map").setView([30.75, -97.48], 10);
  L.tileLayer("https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png", {
    attribution:
      '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors',
  }).addTo(map);
}

function updateMap(animals) {
  if (map) {
    map.eachLayer((layer) => {
      if (layer instanceof L.Marker) {
        map.removeLayer(layer);
      }
    });

    animals.forEach((animal) => {
      if (animal.latitude && animal.longitude) {
        L.marker([animal.latitude, animal.longitude])
          .addTo(map)
          .bindPopup(`<b>${animal.name}</b><br>${animal.breed}`);
      }
    });
  }
}

document.addEventListener("DOMContentLoaded", () => {
  initMap();

  fetch("/api/animals")
    .then((response) => response.json())
    .then((animals) => {
      updateMap(animals);
    });
});
