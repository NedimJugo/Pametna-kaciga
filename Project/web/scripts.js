
import { initializeApp } from "https://www.gstatic.com/firebasejs/11.0.1/firebase-app.js";
import { getDatabase, ref, onValue,set } from "https://www.gstatic.com/firebasejs/11.0.1/firebase-database.js";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
export const firebaseConfig = {
    apiKey: "YOUR_API_KEY",
    authDomain: "YOUR_AUTH_DOMAIN",
    databaseURL: "YOUR_DATABASE_URL",
    projectId: "YOUR_PROJECT_ID",
    storageBucket: "YOUR_STORAGE_BUCKET",
    messagingSenderId: "YOUR_MESSAGING_SENDER_ID",
    appId: "YOUR_APP_ID"
  };

const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

// References to database paths
const humidityRef = ref(database, 'DHT_11/Humidity');
const temperatureRef = ref(database, 'DHT_11/Temperature');
const ledColorRef = ref(database, 'DHT_11/LED_Color');
const ledControlRef = ref(database, 'DHT_11/LED_Control');
const airQualityRef = ref(database, 'DHT_11/Air_Quality');

onValue(humidityRef, (snapshot) => {
    const humidity = snapshot.val();
    document.getElementById('humidity').innerHTML = humidity + "%";
});


onValue(temperatureRef, (snapshot) => {
    const temperature = snapshot.val();
    document.getElementById('temperature').innerHTML = temperature + "&#8451;";
    updateChart(temperature);
});
onValue(airQualityRef, (snapshot) => {
    const airQuality = snapshot.val();
    document.getElementById('airQuality').innerHTML = airQuality;
    updateAirQualityChart(airQuality);
});

onValue(ledColorRef, (snapshot) => {
    const ledColor = snapshot.val();
    document.getElementById('ledColor').innerHTML = ledColor; 
});

const ctx = document.getElementById('temperatureChart').getContext('2d');
const ctxAir = document.getElementById('airQualityChart').getContext('2d');

const temperatureChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Temperatura (°C)',
            data: [],
            borderColor: '#6EB7FF',
            backgroundColor: 'rgba(110, 183, 255, 0.1)',
            borderWidth: 2,
            tension: 0.3
        }]
    },
    options: {
        scales: {
            x: {
                type: 'time',
                time: {
                    unit: 'minute'
                },
                title: {
                    display: true,
                    text: 'Vrijeme'
                }
            },
            y: {
                beginAtZero: false,
                title: {
                    display: true,
                    text: 'Temperatura (°C)'
                }
            }
        }
    }
});

const airQualityChart = new Chart(ctxAir, {
    type: 'line',
    data: { 
        labels: [], 
        datasets: [{ 
            label: 'Kvaliteta zraka', 
            data: [], 
            borderColor: '#FF6E6E', 
            backgroundColor: 'rgba(255, 110, 110, 0.1)', 
            borderWidth: 2, 
            tension: 0.3 
        }] 
    },
    options: { 
        scales: { 
            x: { 
                type: 'time', 
                time: { 
                    unit: 'minute' 
                }, 
                title: {
                    display: true,
                    text: 'Vrijeme'
                }
            },
            y: { 
                beginAtZero: false,
                title: {
                    display: true,
                    text: 'Kvaliteta zraka'
                }
            } 
        }
    }
});


function updateChart(temperature) {
    const now = new Date();
    console.log("New data point added:", now, temperature); 
    temperatureChart.data.labels.push(now);
    temperatureChart.data.datasets[0].data.push(temperature);

    if (temperatureChart.data.labels.length > 20) {
        temperatureChart.data.labels.shift();
        temperatureChart.data.datasets[0].data.shift();
    }

    temperatureChart.update(); 
}

function updateAirQualityChart(airQuality) {
    const now = new Date();
    airQualityChart.data.labels.push(now);
    airQualityChart.data.datasets[0].data.push(airQuality);
    if (airQualityChart.data.labels.length > 20) {
        airQualityChart.data.labels.shift();
        airQualityChart.data.datasets[0].data.shift();
    }
    airQualityChart.update();
}

document.getElementById('toggleLed').addEventListener('click', async () => {
    const currentState = document.getElementById('toggleLed').textContent;
    const newState = currentState === 'Pozovi' ? 'ON' : 'OFF';
    await set(ledControlRef, newState);
    document.getElementById('toggleLed').textContent = newState === 'ON' ? 'Prekini poziv' : 'Pozovi';
});

onValue(ledControlRef, (snapshot) => {
const ledState = snapshot.val();
document.getElementById('toggleLed').textContent = ledState === 'ON' ? 'Prekini poziv' : 'Pozovi';
});

document.getElementById('themeToggle').addEventListener('change', function () {
    document.body.classList.toggle('light-mode');
    const themeIcon = document.getElementById('themeIcon');
    const canvas1 = document.getElementById('temperatureChart');
    canvas1.style.backgroundColor='transparent';
    canvas1.style.opacity=1;
    const canvas2 = document.getElementById('airQualityChart');
    canvas2.style.backgroundColor='transparent';
    canvas2.style.opacity=1;
});
