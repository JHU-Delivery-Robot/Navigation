let canvas = document.getElementById('visualizationCanvas');

let container = document.getElementById('visualizationContainer');
window.addEventListener('resize', resizeCanvas, false);

const simulationFileSelector = document.getElementById('simulationFileSelector');
simulationFileSelector.addEventListener('change', (event) => {
    const files = event.target.files;

    if (files.length == 1) {
        loadSimRecording(files[0]);
    }
});

let simRecording = {
    obstacles: [],
    positions: [],
}

function resizeCanvas() {
    let rect = canvas.parentNode.getBoundingClientRect();
    let height = window.innerHeight - document.querySelector(".header").getBoundingClientRect().height;

    let size = Math.min(rect.width, height);
    console.log(size);
    canvas.width = size * 0.9;
    canvas.height = size * 0.9;

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
    let ctx = canvas.getContext('2d');
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
