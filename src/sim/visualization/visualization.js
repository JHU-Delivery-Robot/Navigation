const canvas = document.getElementById('visualizationCanvas');
const container = document.getElementById('visualization-container');
const simulationFileSelector = document.getElementById('simulationFileSelector');

// Placeholders/defaults, data will be loaded from user-supplied file
let simRecording = {
    obstacles: [],
    positions: [],
    waypoints: [],
    time_step: 0.1,
    size: 8,
};

let currentTimeIndex = 0;
let timeElapsed = 0;
let lastUpdate = null;
let updateTimer = null;

window.addEventListener('resize', resizeCanvas, false);
simulationFileSelector.addEventListener('input', onFileInput);

// Fit canvas to initial window size
resizeCanvas();

// Triggers file selection dialog
function selectFile() {
    simulationFileSelector.click();
}

// File selection callback
function onFileInput(event) {
    const files = event.target.files;

    if (files.length == 1) {
        let file = files[0];
        event.target.value = null;
        document.getElementById('simulationFileDisplay').innerText = file.name;

        // Load/parse sim recording and display first frame
        file.text().then(text => {
            simRecording = JSON.parse(text);
            visualize();
        });
    }
}

// Updates which replay control icon is 'active'
function updateActiveControl(activeControl) {
    document.getElementById('play').classList.remove('active');
    document.getElementById('pause').classList.remove('active');
    document.getElementById('restart').classList.remove('active');
    document.getElementById('stepBack').classList.remove('active');
    document.getElementById('step').classList.remove('active');

    document.getElementById(activeControl).classList.add('active');
}

// Play at normal speed from current place in recording
function play() {
    updateActiveControl('play');

    if (updateTimer) {
        clearInterval(updateTimer);
        updateTimer = null;
    }

    // Move replay forward one tick
    function tick() {
        if (lastUpdate == null) {
            lastUpdate = Date.now();
        }
        timeElapsed += Date.now() - lastUpdate;
        currentTimeIndex = Math.floor(timeElapsed / (1000.0 * simRecording.time_step));

        if (currentTimeIndex >= simRecording.positions.length) {
            currentTimeIndex = 0;
            timeElapsed = 0.0;
            pause();
        } else {
            update();
        }
    }

    lastUpdate = null;
    updateTimer = setInterval(tick, 10);
}

// Pause replay
function pause() {
    updateActiveControl('pause');

    if (updateTimer) {
        clearInterval(updateTimer);
        updateTimer = null;
    }
}

// Set replay to beginning and pause if playing
function restart() {
    updateActiveControl('restart');

    if (updateTimer) {
        clearInterval(updateTimer);
        updateTimer = null;
    }

    currentTimeIndex = 0;
    timeElapsed = 0.0;
    update();
}

// Step forward one tick and pause if playing and 
function step() {
    updateActiveControl('step');

    if (updateTimer) {
        clearInterval(updateTimer);
        updateTimer = null;
    }

    if (currentTimeIndex < simRecording.positions.length) {
        currentTimeIndex += 1;
        timeElapsed = currentTimeIndex * simRecording.time_step;
    }

    update();
}

// Step back one tick and pause if playing
function stepBack() {
    updateActiveControl('stepBack');

    if (updateTimer) {
        clearInterval(updateTimer);
        updateTimer = null;
    }

    if (currentTimeIndex > 0) {
        currentTimeIndex -= 1;
        timeElapsed = currentTimeIndex * simRecording.time_step;
    }

    update();
}

// Update visualization and time step display
function update() {
    visualize();
    document.querySelector('.timestep-counter').innerText = currentTimeIndex;
}

// Resize canvas to fit current window
function resizeCanvas() {
    let height = window.innerHeight;
    let width = window.innerWidth - document.querySelector(".sidebar-container").getBoundingClientRect().width;

    canvas.width = width;
    canvas.height = height;

    visualize();
}

// Get canvas x coordinate from point in sim-coordinate space
function pointX(point) {
    let canvasSize = Math.min(canvas.width, canvas.height);
    let size = simRecording.size;
    return point[0] * (canvasSize / size) + 0.5 * canvas.width;
}

// Get canvas y coordinate from point in sim-coordinate space
function pointY(point) {
    let canvasSize = Math.min(canvas.width, canvas.height);
    let size = simRecording.size;
    return 0.5 * canvas.height - point[1] * (canvasSize / size);
}

// Visualize current tick of replay
function visualize() {
    drawObstacles();
    drawWaypoints();

    if (simRecording.positions[currentTimeIndex]) {
        drawRobot(simRecording.positions[currentTimeIndex]);
    }
}

function drawObstacles() {
    let ctx = canvas.getContext('2d');
    ctx.fillStyle = '#ddd';
    ctx.fillRect(0, 0, canvas.width, canvas.height);
    ctx.fillStyle = '#000';

    simRecording.obstacles.forEach(obstacle => {
        if (obstacle.length == 0) {
            return;
        }

        ctx.beginPath();
        ctx.moveTo(pointX(obstacle[0]), pointY(obstacle[0]));

        obstacle.forEach(point => {
            ctx.lineTo(pointX(point), pointY(point));
        });

        ctx.closePath();
        ctx.fill();
    });
}

function drawWaypoints() {
    let ctx = canvas.getContext('2d');
    ctx.fillStyle = '#aa0099';

    simRecording.waypoints.forEach(waypoint => {
        ctx.beginPath();
        ctx.arc(pointX(waypoint), pointY(waypoint), 5, 0, 2 * Math.PI, false);
        ctx.fill();
    });
}

function drawRobot(position) {
    let ctx = canvas.getContext('2d');
    let canvasSize = Math.min(canvas.width, canvas.height);
    let size = simRecording.size;
    let scale = canvasSize / size;

    ctx.save();

    ctx.translate(pointX(position), pointY(position));

    ctx.save();

    ctx.rotate(-position[2]);

    // Body of robot
    ctx.rect(-0.25 * scale, -0.1 * scale, 0.5 * scale, 0.2 * scale);
    ctx.stroke();

    ctx.restore();

    ctx.translate(Math.cos(position[2]) * 0.25 * scale, -Math.sin(position[2]) * 0.25 * scale);

    ctx.restore();
}
