const canvas = document.getElementById('visualizationCanvas');
const container = document.getElementById('visualization-container');
const configFileSelector = document.getElementById('configFileSelector');

let config = {
    map_size: 8,
};

class Drawable {
    constructor(scene, type) {
        this.scene = scene;
        this.type = type;
    }

    draw(ctx) { }
    contains(point) { return false; }
    onPointerDown(point) { }
    onPointerMove(point) { }
    onPointerUp(point) { }
    delete() { }
    children() { return []; }
    onChildMove(point, child) { }
}

class Robot extends Drawable {
    width = 0.2;
    height = 0.5;

    constructor(scene) {
        super(scene, "robot");
        this.position = { x: 0.0, y: 0.0 };
        this.angle = 90.0;
        this.rotation_handle = new Point(scene, this, this.handle_position(), 7);
    }

    children() {
        return [this.rotation_handle];
    }

    handle_position() {
        return {
            x: this.position.x + Math.cos(this.angle * Math.PI / 180.0) * this.height / 2,
            y: this.position.y + Math.sin(this.angle * Math.PI / 180.0) * this.height / 2,
        }
    }

    contains(point) {
        let dx = Math.abs(point.x - this.position.x);
        let dy = Math.abs(point.y - this.position.y);

        return (dx < this.width / 2) && (dy < this.height / 2);
    }

    update_position(point) {
        let delta = {
            x: point.x - this.start_position.x - this.offset.x,
            y: point.y - this.start_position.y - this.offset.y,
        };

        this.offset = {
            x: point.x - this.start_position.x,
            y: point.y - this.start_position.y,
        }

        this.position = {
            x: this.position.x + delta.x,
            y: this.position.y + delta.y,
        }

        this.rotation_handle.update_position(this.handle_position());
    }

    onPointerDown(point) {
        if (this.scene.active_object != null) { return; }

        this.scene.set_active(this);
        this.start_position = point;
        this.offset = {
            x: 0, y: 0,
        };
    }

    onPointerMove(point) {
        this.update_position(point);
    }

    onPointerUp(point) {
        let action = new Move(this.scene, this, this.start_position, point, null);
        this.scene.set_active(null);
        this.scene.apply(action);
    }

    onChildMove(point, child) {
        let x = point.x - this.position.x;
        let y = point.y - this.position.y;

        this.angle = Math.atan2(y, x) * 180.0 / Math.PI;
        this.angle = (this.angle + 360.0) % 360.0;
        let margin = 5.0; // degrees
        function nearly_equal(a, b) {
            let x = Math.abs(a - b);
            return x < margin || (360.0 - x) < margin;
        }

        let snap_angles = [0.0, 45.0, 90.0, 135.0, 180.0, 225.0, 270.0, 315.0];
        for (var i = 0; i < snap_angles.length; i++) {
            if (nearly_equal(snap_angles[i], this.angle)) {
                this.angle = snap_angles[i];
                break;
            }
        }

        this.rotation_handle.update_position(this.handle_position());
    }

    draw(ctx) {
        ctx.lineWidth = this.scene.scale(5);
        ctx.strokeStyle = "black";

        ctx.save();
        ctx.translate(this.position.x, this.position.y);
        ctx.rotate(this.angle * Math.PI / 180.0);

        ctx.beginPath();
        ctx.rect(-this.height / 2, -this.width / 2, this.height, this.width);
        ctx.stroke();

        ctx.restore();
    }
}

class Polygon extends Drawable {
    vertex_size = 5;

    constructor(scene, closed) {
        super(scene, "polygon");
        this.vertices = [];
        this.closed = closed;
    }

    children() {
        return this.vertices;
    }

    contains(point) {
        if (!this.closed || this.vertices.length < 3) { return false; }

        function edge_value(p, v1, v2) {
            return (v1.x - p.x) * (v2.y - p.y) - (v2.x - p.x) * (v1.y - p.y);
        }

        function compute_winding(v1, v2) {
            let between = (v1.y <= point.y && point.y <= v2.y) || (v2.y <= point.y && point.y <= v1.y);
            if (!between) { return 0; }

            let dy = v2.y - v1.y;
            if (dy > 0 && edge_value(point, v1, v2) > 0) {
                return 1;
            } else if (dy < 0 && edge_value(point, v1, v2) < 0) {
                return -1;
            } else {
                return 0;
            }
        }

        let winding = 0;

        for (var i = 0; i < this.vertices.length - 1; i++) {
            winding += compute_winding(this.vertices[i].point, this.vertices[i + 1].point);
        }

        winding += compute_winding(this.vertices[this.vertices.length - 1].point, this.vertices[0].point);

        return winding != 0;
    }

    update_position(point) {
        let delta = {
            x: point.x - this.start_position.x - this.offset.x,
            y: point.y - this.start_position.y - this.offset.y,
        };

        this.offset = {
            x: point.x - this.start_position.x,
            y: point.y - this.start_position.y,
        }

        for (var i = 0; i < this.vertices.length; i++) {
            this.vertices[i].point.x += delta.x;
            this.vertices[i].point.y += delta.y;
        }
    }

    onPointerDown(point) {
        if (this.scene.active_object != null) { return; }

        this.scene.set_active(this);
        this.start_position = point;
        this.offset = {
            x: 0, y: 0,
        };
    }

    onPointerMove(point) {
        if (!this.closed) { return; }

        this.update_position(point);
    }

    onPointerUp(point) {
        if (!this.closed) { return; }

        let action = new Move(this.scene, this, this.start_position, point, null);
        this.scene.set_active(null);
        this.scene.apply(action);
    }

    delete() {
        for (var i = 0; i < this.scene.objects.length; i++) {
            if (this.scene.objects[i] == this) {
                let remove = new Remove(this.scene, i, this);
                scene.apply(remove);
                return
            }
        }
    }

    draw(ctx) {
        ctx.lineWidth = this.scene.scale(3);
        ctx.fillStyle = "black";
        ctx.strokeStyle = this.closed && this.scene.active_object != this ? "green" : "blue";

        ctx.beginPath();
        ctx.moveTo(this.vertices[0].point.x, this.vertices[0].point.y);
        for (var i = 1; i < this.vertices.length; i++) {
            ctx.lineTo(this.vertices[i].point.x, this.vertices[i].point.y);
        }

        if (this.closed) {
            ctx.closePath();
        }

        ctx.fill();
        ctx.stroke();
    }
}

class Point extends Drawable {
    constructor(scene, parent, point, size) {
        super(scene, "point");
        this.parent = parent;
        this.point = point;
        this.size = size;
    }

    update_position(point) {
        this.point = point;
    }

    position() {
        return this.point;
    }

    contains(point) {
        if (this.scene.active_object != null) { return false; }
        let distance = Math.sqrt((point.x - this.point.x) ** 2 + (point.y - this.point.y) ** 2);

        return distance <= 3 * this.scene.scale(this.size);
    }

    onPointerDown(point) {
        if (this.scene.active_object == null) {
            this.scene.set_active(this);
            this.start_position = this.point;
        }
    }

    onPointerMove(point) {
        this.update_position(point);
        this.parent.onChildMove(point, this);
    }

    onPointerUp(point) {
        let action = new Move(this.scene, this, this.start_position, point, this.parent);
        this.scene.set_active(null);
        this.scene.apply(action);
    }

    draw(ctx) {
        ctx.fillStyle = "blue";

        ctx.beginPath();
        ctx.arc(this.point.x, this.point.y, this.scene.scale(this.size), 0, 2 * Math.PI, false);
        ctx.closePath();
        ctx.fill();
    }
}

class Waypoints extends Drawable {
    waypoint_size = 5;

    constructor(scene) {
        super(scene, "waypoints");
        this.waypoints = [
            new Point(scene, this, { x: 1.0, y: -1.0 }, this.waypoint_size),
            new Point(scene, this, { x: 1.0, y: 1.0 }, this.waypoint_size),
        ];
    }

    children() {
        return this.waypoints;
    }

    closest_segment(point) {
        let closest_index = 0;
        let closest_distance = Infinity;

        for (var i = 0; i < this.waypoints.length - 1; i++) {
            let start = this.waypoints[i].point;
            let end = this.waypoints[i + 1].point;

            let delta = [end.x - start.x, end.y - start.y];
            let p = [point.x - start.x, point.y - start.y];
            let t = (delta[0] * p[0] + delta[1] * p[1]) / (delta[0] * delta[0] + delta[1] * delta[1]);
            t = Math.max(0, Math.min(1, t));

            let projected = [start.x + t * delta[0], start.y + t * delta[1]];
            let projection_delta = [projected[0] - point.x, projected[1] - point.y];
            let distance = Math.sqrt(projection_delta[0] ** 2 + projection_delta[1] ** 2);
            if (distance < closest_distance) {
                closest_distance = distance;
                closest_index = i;
            }
        }

        return [closest_index, closest_distance];
    }

    contains(point) {
        let closest = this.closest_segment(point);
        return closest[1] < 3*this.scene.scale(this.waypoint_size);
    }

    onPointerDown(point) {
        if (this.scene.active_object == null && drawing_mode === Modes.Waypoint) {
            let closest = this.closest_segment(point);
            let action = new ExtendWaypoints(this.scene, this, closest[0]+1, point);
            this.scene.apply(action);
        }
    }

    draw(ctx) {
        ctx.lineWidth = this.scene.scale(5);
        ctx.strokeStyle = "purple";

        ctx.beginPath();
        ctx.moveTo(this.waypoints[0].point.x, this.waypoints[0].point.y);
        for (var i = 1; i < this.waypoints.length; i++) {
            ctx.lineTo(this.waypoints[i].point.x, this.waypoints[i].point.y);
        }

        ctx.stroke();
    }
}

class Action {
    constructor(scene) {
        this.scene = scene;
    }

    do() { }
    undo() { }
}

class Add extends Action {
    constructor(scene, object) {
        super(scene);
        this.object = object;
    }

    do() {
        super.do();
        this.scene.add(this.object);
        this.scene.set_active(this.object);
    }

    undo() {
        super.undo();
        this.scene.remove();
        this.scene.set_active(null);
    }
}

class Remove extends Action {
    constructor(scene, index, object) {
        super(scene);
        this.index = index;
        this.object = object;
    }

    do() {
        super.do();
        this.scene.set_active(null);
        this.scene.objects.splice(this.index, 1);
    }

    undo() {
        super.undo();
        this.scene.objects.splice(this.index, 0, this.object);
    }
}

class ExtendPolygon extends Action {
    constructor(scene, polygon, vertex) {
        super(scene);
        this.polygon = polygon;
        this.vertex = new Point(this.scene, this.polygon, vertex, this.polygon.vertex_size);
    }

    do() {
        super.do();

        let start = this.polygon.vertices[0].point;
        let distance_to_start = Math.sqrt((start.x - this.vertex.point.x) ** 2 + (start.y - this.vertex.point.y) ** 2);
        if (distance_to_start < 3 * this.scene.scale(this.polygon.vertex_size)) {
            this.polygon.closed = true;
            this.scene.set_active(null);
        } else {
            this.polygon.vertices.push(this.vertex);
        }
    }

    undo() {
        super.undo();

        if (this.polygon.closed) {
            this.polygon.closed = false;
        } else {
            this.polygon.vertices.pop();
        }
    }
}

class ExtendWaypoints extends Action {
    constructor(scene, waypoints, index, point) {
        super(scene);
        this.waypoints = waypoints;
        this.index = index;
        this.point = new Point(this.scene, this.waypoints, point, this.waypoints.waypoint_size);
    }

    do() {
        super.do();
        this.waypoints.waypoints.splice(this.index, 0, this.point);
    }

    undo() {
        super.undo();
        this.waypoints.waypoints.splice(this.index, 1);
    }
}

class Move extends Action {
    constructor(scene, target, start, end, parent) {
        super(scene);
        this.target = target;
        this.start = start;
        this.end = end;
        this.parent = parent;
    }

    do() {
        super.do();
        this.target.update_position(this.end);

        if (this.parent) {
            this.parent.onChildMove(this.end, this.target);
        }
    }

    undo() {
        super.undo();
        this.target.update_position(this.start);

        if (this.parent) {
            this.parent.onChildMove(this.start, this.target);
        }
    }
}

class Scene {
    constructor() {
        this.objects = [];
        this.active_object = null;

        this.actions = [];
        this.undone_actions = [];
    }

    draw_object(ctx, object) {
        object.draw(ctx);
        let children = object.children();

        for (var i = 0; i < children.length; i++) {
            this.draw_object(ctx, children[i]);
        }
    }

    clear_background(ctx) {
        ctx.save();
        ctx.setTransform(1, 0, 0, 1, 0, 0);
        ctx.fillStyle = '#ddd';
        ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);
        ctx.restore();
    }

    // transform canvas to have origin in center, X & Y like standard Cartesian coordinates,
    // and scaled to fit map to canvas
    transform_canvas(ctx) {
        let canvas_size = Math.min(canvas.width, canvas.height);
        let map_size = config.map_size;
        let scale = canvas_size / map_size;

        ctx.resetTransform();
        ctx.translate(canvas.width / 2.0, canvas.height / 2.0);
        ctx.scale(scale, -scale);
    }

    // transform mouse/touch pointer coordinates to match transformed canvas
    get_pointer(event) {
        let x = parseInt(event.clientX - canvas.offsetLeft);
        let y = parseInt(event.clientY - canvas.offsetTop);

        let canvas_size = Math.min(canvas.width, canvas.height);
        let map_size = config.map_size;
        let scale = map_size / canvas_size;

        x = (x - (canvas.width / 2.0)) * scale;
        y = (y - (canvas.height / 2.0)) * (-scale);

        return { x, y };
    }

    scale(size) {
        let canvas_size = Math.min(canvas.width, canvas.height);
        let map_size = config.map_size;
        let scale = map_size / canvas_size;

        return size * scale;
    }

    draw() {
        let ctx = canvas.getContext('2d');
        this.clear_background(ctx);

        this.transform_canvas(ctx);

        for (var i = 0; i < this.objects.length; i++) {
            this.draw_object(ctx, this.objects[i]);
        }
    }

    add(object) {
        this.objects.push(object);
    }

    remove() {
        this.objects.pop();
    }

    _find_collision(object, point) {
        let children = object.children();
        for (var i = 0; i < children.length; i++) {
            let collision = this._find_collision(children[i], point);
            if (collision != null) { return collision; }
        }

        if (object.contains(point)) {
            return object;
        }

        return null;
    }

    find_collision(point) {
        for (var i = this.objects.length - 1; i >= 0; i--) {
            let collision = this._find_collision(this.objects[i], point);
            if (collision != null) { return collision; }
        }

        return null;
    }

    set_active(object) {
        this.active_object = object;
    }

    apply(action) {
        action.do();
        this.actions.push(action);
        this.undone_actions = [];
    }

    undo() {
        if (this.actions.length > 0) {
            let action = this.actions.pop();
            action.undo();
            this.undone_actions.push(action);
        }
    }

    redo() {
        if (this.undone_actions.length > 0) {
            let action = this.undone_actions.pop();
            action.do();
            this.actions.push(action);
        }
    }
}

let scene = new Scene();
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

const Modes = {
    None: "none",
    Polygon: "polygon",
    Waypoint: "waypoint",
};

let drawing_mode = Modes.None;

function polygon_mode() {
    drawing_mode = Modes.Polygon;
    document.getElementById('polygon_mode').classList.add('active');
    document.getElementById('waypoint_mode').classList.remove('active');
}

function waypoint_mode() {
    drawing_mode = Modes.Waypoint;
    document.getElementById('waypoint_mode').classList.add('active');
    document.getElementById('polygon_mode').classList.remove('active');
}

function onPointerDown(event) {
    let pointer = scene.get_pointer(event);

    let target = scene.find_collision(pointer);
    if (target != null) {
        target.onPointerDown(pointer);
        return;
    } else {
        if (drawing_mode == Modes.Polygon) {
            if (scene.active_object != null) {
                let action = new ExtendPolygon(scene, scene.active_object, pointer);
                scene.apply(action);
            } else {
                let polygon = new Polygon(scene, false);
                polygon.vertices.push(new Point(scene, polygon, pointer, polygon.vertex_size));
                let action = new Add(scene, polygon);
                scene.apply(action);
            }
        } else if (drawing_mode == Modes.Waypoint) {
            let action = new ExtendWaypoints(scene, waypoints, waypoints.waypoints.length, pointer);
            scene.apply(action);
        }
    }

    scene.draw();
}

function onPointerMove(event) {
    let pointer = scene.get_pointer(event);

    if (scene.active_object != null) {
        scene.active_object.onPointerMove(pointer);
    }

    scene.draw();
}

function onPointerUp(event) {
    let pointer = scene.get_pointer(event);

    if (scene.active_object != null) {
        scene.active_object.onPointerUp(pointer);
    }

    scene.draw();
}

function save_config() {
    output_config = config;
    output_config["obstacles"] = [];

    for (var i = 0; i < scene.objects.length; i++) {
        let object = scene.objects[i];
        if (object.type == "polygon") {
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
            output_config["start_position"] = [object.position.x, object.position.y ];
            output_config["start_angle"] = object.angle;
        }
    }

    let json = JSON.stringify(output_config);
    console.log(json);
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

canvas.addEventListener("pointerdown", onPointerDown);
canvas.addEventListener("pointerup", onPointerUp);
canvas.addEventListener("pointermove", onPointerMove);
window.addEventListener("resize", resizeCanvas, false);
window.addEventListener("keydown", canvasKeyDown);

// Fit canvas to initial window size
resizeCanvas();

// Resize canvas to fit current window
function resizeCanvas() {
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
            scene.draw();
        });
    }
}
