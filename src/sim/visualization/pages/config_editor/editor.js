const canvas = document.getElementById('visualizationCanvas');
const container = document.getElementById('visualization-container');
const configFileSelector = document.getElementById('configFileSelector');

let config = {
    map_size: 8,
};

let scene = new Scene(canvas, handle_pointer);
scene.objects.push(new Robot(scene));
let waypoints = new Waypoints(scene);
scene.objects.push(waypoints);

document.addEventListener('keydown', function (e) {
    if (e.key === 'z' && e.ctrlKey) {
        scene.undo();
        scene.draw();
    } else if (e.key === 'y' && e.ctrlKey) {
        scene.redo();
        scene.draw();
    } else if (event.key === 's' && event.ctrlKey) {
        event.preventDefault();
        event.stopPropagation();
        save_config();
    }
});

const EditModes = {
    None: "none",
    Obstacle: "obstacle",
    Waypoint: "waypoint",
};

let edit_mode = EditModes.None;

function polygon_mode() {
    edit_mode = EditModes.Obstacle;
    document.getElementById('polygon_mode').classList.add('active');
    document.getElementById('waypoint_mode').classList.remove('active');
}

function waypoint_mode() {
    edit_mode = EditModes.Waypoint;
    document.getElementById('waypoint_mode').classList.add('active');
    document.getElementById('polygon_mode').classList.remove('active');
}

function handle_pointer(pointer) {
    if (edit_mode == EditModes.Obstacle) {
        if (scene.active_object != null) {
            let action = new ExtendObstacle(scene, scene.active_object, pointer);
            scene.apply(action);
        } else {
            let obstacle = new Obstacle(scene, false);
            obstacle.vertices.push(new Point(scene, obstacle, pointer, obstacle.vertex_size));
            let action = new Add(scene, obstacle);
            scene.apply(action);
        }
    } else if (edit_mode == EditModes.Waypoint) {
        let action = new ExtendWaypoints(scene, waypoints, waypoints.waypoints.length, pointer);
        scene.apply(action);
    }
}

function save_config() {
    output_config = config;
    output_config["obstacles"] = [];

    for (var i = 0; i < scene.objects.length; i++) {
        let object = scene.objects[i];
        if (object.type == "obstacle") {
            let vertices = [];

            for (var j = 0; j < object.vertices.length; j++) {
                let point = [object.vertices[j].point.x, object.vertices[j].point.y];
                vertices.push(point);
            }

            output_config["obstacles"].push(vertices)
        } else if (object.type == "waypoints") {
            output_config["waypoints"] = [];
            for (var j = 0; j < object.waypoints.length; j++) {
                let point = [object.waypoints[j].point.x, object.waypoints[j].point.y];
                output_config["waypoints"].push(point);
            }
        } else if (object.type == "robot") {
            output_config["robot"] = {
                "position": [object.position.x, object.position.y],
                "angle": object.angle,
            }
        }
    }

    let json = JSON.stringify(output_config, null, 4);
    let json_data = new Blob([json], { type: "text/json" });

    let downloader = document.getElementById('configFileDownload');
    let file_name = document.getElementById('configFileDisplay').innerText || "sim_config.json";
    downloader.download = file_name;
    downloader.href = URL.createObjectURL(json_data);
    downloader.dispatchEvent(new MouseEvent("click"));
    URL.revokeObjectURL(downloader.href);
}

function load_config(config) {
    scene = new Scene(canvas, handle_pointer);
    scene.set_size(config.map_size);

    let robot = new Robot(scene);
    robot.load(config["robot"]);
    scene.objects.push(robot);

    let waypoints = new Waypoints(scene);
    waypoints.load(config["waypoints"]);
    scene.objects.push(waypoints);

    for (var i = 0; i < config.obstacles.length; i++) {
        let obstacle = new Obstacle(scene, true);
        obstacle.load(config["obstacles"][i]);
        scene.objects.push(obstacle);
    }
}

function canvasKeyDown(event) {
    if (event.key == "Delete") {
        event.stopPropagation();
        if (scene.active_object != null) {
            scene.active_object.delete();
            scene.draw();
        }
    }
}

canvas.addEventListener("pointerdown", (event) => scene.on_pointer_down(event));
canvas.addEventListener("pointerup", (event) => scene.on_pointer_up(event));
canvas.addEventListener("pointermove", (event) => scene.on_pointer_move(event));
window.addEventListener("resize", resize_canvas, false);
window.addEventListener("keydown", canvasKeyDown);

// Fit canvas to initial window size
resize_canvas();

// Resize canvas to fit current window
function resize_canvas() {
    let height = window.innerHeight;
    let width = window.innerWidth - document.querySelector(".sidebar-container").getBoundingClientRect().width;

    canvas.width = width;
    canvas.height = height;

    scene.draw();
}

// Triggers file selection dialog
function selectFile() {
    configFileSelector.click();
}

configFileSelector.addEventListener('input', onFileInput);

// File selection callback
function onFileInput(event) {
    const files = event.target.files;

    if (files.length == 1) {
        let file = files[0];
        event.target.value = null;
        document.getElementById('configFileDisplay').innerText = file.name;

        // Load/parse sim recording and display first frame
        file.text().then(text => {
            config = JSON.parse(text);
            load_config(config);
            scene.draw();
        });
    }
}
