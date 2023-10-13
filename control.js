import { initializeApp } from "https://www.gstatic.com/firebasejs/10.0.0/firebase-app.js";
import { getDatabase, ref, set, onValue } from "https://www.gstatic.com/firebasejs/10.0.0/firebase-database.js";

const firebaseConfig = {
  apiKey: "AIzaSyCRX5UlbPr4apW7kWPSfNOcX0wShaxfd3o",
  authDomain: "dht11-1ca81.firebaseapp.com",
  databaseURL: "https://dht11-1ca81-default-rtdb.firebaseio.com",
  projectId: "dht11-1ca81",
  storageBucket: "dht11-1ca81.appspot.com",
  messagingSenderId: "1033761407283",
  appId: "1:1033761407283:web:608a4f6f9a6727917f7e46",
  measurementId: "G-8GCZQ48BXQ"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

function toggleLamp(toggleElem, stateElem, path) {
  let parentNode = toggleElem.parentNode;
  parentNode.classList.toggle('active');
  if (parentNode.classList.contains('active')) {
    set(ref(database, path), true);
  } else {
    set(ref(database, path), false); 
  }
}

let lamps = [
  {toggle: document.getElementById('lamp_1_toggle'), state: document.getElementById('lamp_1_state'), path: 'device/lamp1'},
  {toggle: document.getElementById('air_toggle'), state: document.getElementById('air_state'), path: 'device/air-condition'},
  {toggle: document.getElementById('heat_toggle'), state: document.getElementById('heat_state'), path: 'device/heat'},
  {toggle: document.getElementById('tv_toggle'), state: document.getElementById('tv_state'), path: 'device/tv'},
  {toggle: document.getElementById('fan_toggle'), state: document.getElementById('fan_state'), path: 'device/fan'},
  {toggle: document.getElementById('wifi_toggle'), state: document.getElementById('wifi_state'), path: 'device/wifi'}
];
  
lamps.forEach(function(lamp) {
  lamp.toggle.addEventListener('click', function() {
    toggleLamp(lamp.toggle, lamp.state, lamp.path);
  });
});
    
let lamps_fb = [
  {toggle: document.getElementById('lamp_1_toggle'), state: document.getElementById('lamp_1_state'), path: 'device/lamp1'},
  {toggle: document.getElementById('air_toggle'), state: document.getElementById('air_state'), path: 'device/air-condition'},
  {toggle: document.getElementById('heat_toggle'), state: document.getElementById('heat_state'), path: 'device/heat'},
  {toggle: document.getElementById('tv_toggle'), state: document.getElementById('tv_state'), path: 'device/tv'},
  {toggle: document.getElementById('fan_toggle'), state: document.getElementById('fan_state'), path: 'device/fan'},
  {toggle: document.getElementById('wifi_toggle'), state: document.getElementById('wifi_state'), path: 'device/wifi'}
];

lamps_fb.forEach(function(lamp_fb) {
  onValue(ref(database, lamp_fb.path), function(snapshot) {
    let state = snapshot.val();
    if (state) {
      lamp_fb.toggle.parentNode.classList.add('active');
      lamp_fb.state.innerHTML = "ON";
    } else {
      lamp_fb.toggle.parentNode.classList.remove('active');
      lamp_fb.state.innerHTML = "OFF";
    }
  });
});

