let canvas = document.getElementById('visualizationCanvas');

let container = document.getElementById('visualizationContainer');
window.addEventListener('resize', resizeCanvas, false);

function selectFile() {
    document.getElementById('simulationFileSelector').click();
}

const simulationFileSelector = document.getElementById('simulationFileSelector');
simulationFileSelector.addEventListener('input', (event) => {
    const files = event.target.files;

    if (files.length == 1) {
        let file = files[0];
        event.target.value = null;
        document.getElementById('simulationFileDisplay').innerText = file.name;

        loadSimRecording(file);
    }
});

let simRecording = {
    obstacles: [],
    positions: [],
    goal: [0.0, 0.0],
    time_step: 0.1,
};

current_time_index = 0;
updateTimer = null;

function updateActiveControl(activeControl) {
    document.getElementById('play').classList.remove('active');
    document.getElementById('pause').classList.remove('active');
    document.getElementById('restart').classList.remove('active');
    document.getElementById('stepBack').classList.remove('active');
    document.getElementById('step').classList.remove('active');

    document.getElementById(activeControl).classList.add('active');
}

function play() {
    updateActiveControl('play');

    if (updateTimer) {
        clearInterval(updateTimer);
        updateTimer = null;
    }

    updateTimer = setInterval(tick, simRecording.time_step * 1000);
}

function pause() {
    updateActiveControl('pause');

    if (updateTimer) {
        clearInterval(updateTimer);
        updateTimer = null;
    }
}

function restart() {
    updateActiveControl('restart');

    if (updateTimer) {
        clearInterval(updateTimer);
        updateTimer = null;
    }

    current_time_index = 0;
    update();
}

function step() {
    updateActiveControl('step');

    if (updateTimer) {
        clearInterval(updateTimer);
        updateTimer = null;
    }

    if (current_time_index < simRecording.positions.length) {
        current_time_index += 1;
    }

    update();
}

function stepBack() {
    updateActiveControl('stepBack');

    if (updateTimer) {
        clearInterval(updateTimer);
        updateTimer = null;
    }

    if (current_time_index > 0) {
        current_time_index -= 1;
    }

    update();
}

function tick() {
    current_time_index += 1;

    if (current_time_index >= simRecording.positions.length) {
        current_time_index = 0;
        pause();
    } else {
        update();
    }
}

function update() {
    visualize();
    document.querySelector('.timestep-counter').innerText = current_time_index;
}

function resizeCanvas() {
    let height = window.innerHeight;
    let width = window.innerWidth - document.querySelector(".sidebar-container").getBoundingClientRect().width;

    canvas.width = width;
    canvas.height = height;

    visualize();
}

resizeCanvas();

function loadSimRecording(file) {
    file.text().then(text => {
        simRecording = JSON.parse(text);
        visualize();
    });
}

function pointX(point) {
    let canvasSize = Math.min(canvas.width, canvas.height);
    let size = simRecording.size;
    return point[0] * (canvasSize / size) + 0.5 * canvas.width;
}

function pointY(point) {
    let canvasSize = Math.min(canvas.width, canvas.height);
    let size = simRecording.size;
    return 0.5 * canvas.height - point[1] * (canvasSize / size);
}

function visualize() {
    draw_obstacles();
    draw_goal();

    if (simRecording.positions[current_time_index]) {
        draw_robot(simRecording.positions[current_time_index]);
    }
}

function draw_obstacles() {
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

function draw_goal() {
    let ctx = canvas.getContext('2d');
    ctx.fillStyle = '#aa0099';
    ctx.beginPath();
    ctx.arc(pointX(simRecording.goal), pointY(simRecording.goal), 5, 0, 2 * Math.PI, false);
    ctx.fill();
}

function draw_robot(position) {
    let ctx = canvas.getContext('2d');
    let canvasSize = Math.min(canvas.width, canvas.height);
    let size = simRecording.size;
    let scale = canvasSize / size;

    ctx.save();

    ctx.translate(pointX(position), pointY(position));

    ctx.save();

    ctx.rotate(-position[2]);

    ctx.rect(-25 * scale, -10 * scale, 50 * scale, 20 * scale);
    ctx.stroke();

    ctx.restore();

    ctx.translate(Math.cos(position[2]) * 25 * scale, -Math.sin(position[2]) * 25 * scale);

    oldLineWidth = ctx.lineWidth;
    ctx.lineWidth = 2;
    ctx.strokeStyle = '#00dd44';
    ctx.beginPath();
    ctx.moveTo(0, 0);
    ctx.lineTo(position[5] * scale, -position[6] * scale);
    ctx.stroke();

    ctx.strokeStyle = '#dd4400';
    ctx.beginPath();
    ctx.moveTo(0, 0);
    ctx.lineTo(position[7] * scale, -position[8] * scale);
    ctx.stroke();
    ctx.lineWidth = oldLineWidth;

    ctx.restore();
}
