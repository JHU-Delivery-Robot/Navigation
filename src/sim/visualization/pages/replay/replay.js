const canvas = document.getElementById('visualizationCanvas');
const container = document.getElementById('visualization-container');
const simulationFileSelector = document.getElementById('simulationFileSelector');

// Placeholders/defaults, data will be loaded from user-supplied file
let simRecording = {
    config: {
        obstacles: [],
        waypoints: [],
        time_step: 0.1,
        map_size: 8,
    },
    replay: [],
};

let currentTimeIndex = 0;
let timeElapsed = 0;
let lastUpdate = null;
let updateTimer = null;

let scene = new Scene(canvas, () => { });
let robot = new Robot(scene);

window.addEventListener('resize', resize_canvas, false);
simulationFileSelector.addEventListener('input', onFileInput);

// Fit canvas to initial window size
resize_canvas();

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
            load_scene(simRecording.config);
            document.getElementById('replayPosition').max = simRecording.replay.length - 1;
            visualize();
        });
    }
}

// Updates which replay control icon is 'active'
function updateActiveControl(activeControl) {
    document.getElementById('play').classList.remove('active');
    document.getElementById('pause').classList.remove('active');
    document.getElementById('restart').classList.remove('active');
    document.getElementById('replayPosition').classList.remove('active');

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
        currentTime = Date.now();
        if (lastUpdate == null) {
            lastUpdate = currentTime;
        }
        timeElapsed += currentTime - lastUpdate;
        lastUpdate = currentTime;
        currentTimeIndex = Math.floor(timeElapsed / (1000.0 * simRecording.config.time_step));

        if (currentTimeIndex >= simRecording.replay.length) {
            pause();
            currentTimeIndex = simRecording.replay.length - 1;
        }

        update();
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

// Scrub replay position
function scrub(position) {
    updateActiveControl('replayPosition');

    // pause if playing
    if (updateTimer) {
        clearInterval(updateTimer);
        updateTimer = null;
    }

    currentTimeIndex = position;
    timeElapsed = currentTimeIndex * 1000.0 * simRecording.config.time_step;
    update();
}

// Update visualization and time step display
function update() {
    visualize();
    document.querySelector('.timestep-counter').innerText = currentTimeIndex;
    document.getElementById('replayPosition').value = currentTimeIndex;
}

// Resize canvas to fit current window
function resize_canvas() {
    let height = window.innerHeight;
    let width = window.innerWidth - document.querySelector(".sidebar-container").getBoundingClientRect().width;

    canvas.width = width;
    canvas.height = height;

    scene.draw();
}

// Visualize current tick of replay
function visualize() {
    if (simRecording.replay[currentTimeIndex]) {
        let robot_state = simRecording.replay[currentTimeIndex];
        let angle = robot_state["angle"] * 180.0 / Math.PI;
        let position = {
            x: robot_state["position"][0],
            y: robot_state["position"][1],
        };

        robot.set_position(position, angle);
    }

    scene.draw();
}

function load_scene(config) {
    scene = new Scene(canvas, () => { });
    scene.set_size(config.map_size);

    robot = new Robot(scene);
    robot.load(config["robot"]);
    scene.objects.push(robot);

    let waypoints = new Waypoints(scene);
    waypoints.load(config["waypoints"]);
    scene.objects.push(waypoints);

    for (var i = 0; i < config.obstacles.length; i++) {
        let obstacle = new Obstacle(scene, true, false);
        obstacle.load(config["obstacles"][i]);
        scene.objects.push(obstacle);
    }
}
