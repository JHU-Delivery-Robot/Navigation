const canvas = document.getElementById('visualizationCanvas');
const container = document.getElementById('visualization-container');

var drawing = false;
const offsetX = canvas.offsetLeft;
const offsetY = canvas.offsetTop;
var drawing = "p";
var allObjects = [];
var coordinates = [];
var redoElements = [];
var coordsRedo = [];

//DEFINE DRAWING OBJECT-------------------
class DrawObject {
    constructor(type, coords) {
        this.type = type;
        this.coords = coords;
    }

    draw() {
        let ctx = canvas.getContext('2d');

        switch (this.type) {
            case "p":
                ctx.beginPath();
                ctx.moveTo(this.coords[0].x, this.coords[0].y);
                for (var index = 1; index < this.coords.length; index++) {
                    ctx.lineTo(this.coords[index].x, this.coords[index].y);
                }
                ctx.closePath();
                ctx.lineWidth = 2;
                ctx.fillStyle = 'black';
                ctx.fill();
                break;
            case "w":
                ctx.beginPath();
                ctx.arc(this.coords[0].x, this.coords[0].y, 10, 0, 2 * Math.PI, false);
                ctx.fillStyle = 'blue';
                ctx.fill();
                ctx.closePath();
                break;
            case "s":
                ctx.beginPath();
                if (this.coords.length != 0) {
                    ctx.arc(this.coords[0].x, this.coords[0].y, 10, 0, 2 * Math.PI, false);
                    ctx.fillStyle = 'green';
                    ctx.fill();
                    ctx.closePath();
                }
                break;
        }
    }
}

var startPoint = new DrawObject("s", []);

//---------------------------------

//CLEAR AND REDRAW ALL OBJECTS
function redrawAll() {
    let ctx = canvas.getContext('2d');
    ctx.fillStyle = '#ddd';
    ctx.fillRect(0, 0, canvas.width, canvas.height);
    ctx.fillStyle = '#000';

    startPoint.draw();
    for (var i = 0; i < allObjects.length; i++) {
        allObjects[i].draw();
    }
}

document.addEventListener('keydown', function (e) {
    //UNDO
    if (e.key === 'z' && e.ctrlKey) {
        if (coordinates.length != 0) {
            coordsRedo.push(coordinates.pop());
            redrawAll();
            drawPolygon(coordinates);
        } else {
            redoElements.push(allObjects.pop());
            redrawAll();
        }
        //REDO
    } else if (e.key === 'y' && e.ctrlKey) {
        if (coordinates.length != 0 || coordsRedo.length != 0) {
            if (coordsRedo.length != 0) {
                coordinates.push(coordsRedo.pop());
                redrawAll();
                drawPolygon(coordinates);
            }
        } else {
            if (redoElements.length > 0) {
                allObjects.push(redoElements.pop());
            }
            redrawAll();
        }
    }
});

function start_polygon() {
    drawing = "p";
}

function add_waypoint() {
    drawing = "w";
}

function set_startpoint() {
    drawing = "s"
}

function close_polygon() {
    if (coordinates.length != 0) {
        poly = new DrawObject("p", [...coordinates]);
        allObjects.push(poly);
        redrawAll();
        coordinates = [];
        redoElements = [];
        coordsRedo = [];
    }
}

//-------------------------------------

//Handle canvas click operations-----------------------------
function onCanvasClick(event) {
    // tell the browser we're handling this event
    event.preventDefault();
    event.stopPropagation();
    mouseX = parseInt(event.clientX - offsetX);
    mouseY = parseInt(event.clientY - offsetY);

    if (drawing == "p") {
        coordinates.push({ x: mouseX, y: mouseY });
        drawPolygon(coordinates);
    } else if (drawing == "w") {
        coordinates.push({ x: mouseX, y: mouseY });
        waypoint = new DrawObject("w", [...coordinates]);
        allObjects.push(waypoint);
        waypoint.draw();
        coordinates = [];
    } else if (drawing == "s") {
        coordinates.push({ x: mouseX, y: mouseY });
        startPoint.coords = [...coordinates];
        redrawAll();
        coordinates = [];
    }
}
canvas.addEventListener("click", onCanvasClick);
window.addEventListener("resize", resizeCanvas, false);

// Fit canvas to initial window size
resizeCanvas();

// Resize canvas to fit current window
function resizeCanvas() {
    let height = window.innerHeight;
    let width = window.innerWidth - document.querySelector(".sidebar-container").getBoundingClientRect().width;

    canvas.width = width;
    canvas.height = height;

    redrawAll();
}

//---------------------

function drawPolygon(coords) {
    let ctx = canvas.getContext('2d');

    redrawAll();
    ctx.beginPath();
    ctx.strokeStyle = 'black';
    ctx.fillStyle = 'black';
    ctx.moveTo(coords[0].x, coords[0].y);

    for (index = 1; index < coords.length; index++) {
        ctx.lineTo(coords[index].x, coords[index].y);
    }

    ctx.closePath();
    ctx.stroke();
    ctx.lineWidth = 2;

    ctx.fill();
    ctx.fillStyle = 'red';

    for (var i = 0; i < coords.length; i++) {
        ctx.fillRect(coords[i].x - 5, coords[i].y - 5, 10, 10);
    }
}
