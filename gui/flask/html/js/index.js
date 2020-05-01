var mymap = L.map('mapid', {
  zoomControl: true
}).setView([51.505, -0.09], 13);

L.tileLayer('https://{s}.tile.osm.org/{z}/{x}/{y}.png', {
  attribution: '&copy; <a href="https://osm.org/copyright">OpenStreetMap</a> contributors'
}).addTo(mymap);

function draw(locations) {
  mymap.eachLayer(function(layer) {
    if (!layer._url) {
      mymap.removeLayer(layer);
    }
  });

  //Add markers
  markers = locations.map(function(e, i) {
      return L.marker([e.lat, e.lon])
        .bindPopup('Airport Found' + [e.lon, e.lat])
    })
    //Center the view on the markers
  var group = L.featureGroup(markers).addTo(mymap);
  mymap.fitBounds(group.getBounds());

  //Create the routes
  coors = locations.map(function(e, i) {
      return new L.LatLng(parseFloat(e.lat), parseFloat(e.lon));
    })
    //Adding the routes
  var Geodesic1 = L.geodesic([coors], {
    weight: 3,
    opacity: 1,
    steps: 50
  }).addTo(mymap);
}

$("#search").click(function(evt) {
  var query = $("#query").val()
  $.get("https://www2.transport-search.org/api/v1/geo?query=" + query)
    .then(function(data) {
      draw(data.result.locations);
    })
})

$("#omnibox").submit(function(evt) {
  var query = $("#query").val()
  evt.preventDefault()
  $.get("https://www2.transport-search.org/api/v1/geo?query=" + query)
    .then(function(data) {
      draw(data.result.locations);
    })
})


$("#random-airport").click(function(evt) {
  $.get("https://www2.transport-search.org/api/v1/randomairport")
  .then(function(data) {
    draw(data.result.locations);
  })
})

$("#random-route").click(function(evt) {
  $.get("https://www2.transport-search.org/api/v1/randomroute")
  .then(function(data) {
    draw(data.result.locations);
  })
})

