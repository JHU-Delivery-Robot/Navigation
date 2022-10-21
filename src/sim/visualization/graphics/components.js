class Robot extends Object {
    width = 0.2;
    height = 0.5;

    constructor(scene) {
        super(scene, "robot");
        this.position = { x: 0.0, y: 0.0 };
        this.angle = 90.0;
        this.rotation_handle = new Point(scene, this, this.rotation_handle_position(), 7);
    }

    load(json) {
        this.position = { x: json.position[0], y: json.position[1] };
        this.angle = json.angle;
        this.rotation_handle.update_position(this.rotation_handle_position());
    }

    save() {
        return {
            position: [this.position.x, this.position.y],
            angle: this.angle,
        };
    }

    children() {
        return [this.rotation_handle];
    }

    rotation_handle_position() {
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

    set_position(position, angle) {
        this.position = position;
        this.angle = angle;
        this.rotation_handle.update_position(this.rotation_handle_position());
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

        this.rotation_handle.update_position(this.rotation_handle_position());
    }

    on_pointer_down(point) {
        if (this.scene.active_object != null) { return; }

        this.scene.set_active(this);
        this.start_position = point;
        this.offset = {
            x: 0, y: 0,
        };
    }

    on_pointer_move(point) {
        this.update_position(point);
    }

    on_pointer_up(point) {
        let action = new Move(this.scene, this, this.start_position, point, null);
        this.scene.set_active(null);
        this.scene.apply(action);
    }

    on_child_move(point, child) {
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

        this.rotation_handle.update_position(this.rotation_handle_position());
    }
}

class Obstacle extends Object {
    vertex_size = 5;

    constructor(scene, closed, editable = true) {
        super(scene, "obstacle");
        this.vertices = [];
        this.closed = closed;
        this.editable = editable;

        if (!this.editable) {
            this.vertex_size = 0;
        }
    }

    load(json) {
        this.vertices = [];
        for (var i = 0; i < json.length; i++) {
            let vertex_point = { x: json[i][0], y: json[i][1] };
            let vertex = new Point(this.scene, this, vertex_point, this.vertex_size);
            this.vertices.push(vertex);
        }
    }

    save() {
        let vertices = [];
        for (var i = 0; i < this.vertices.length; i++) {
            let vertex = [this.vertices[i].x, this.vertices[i].y ];
            vertices.push(vertex);
        }

        return vertices;
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

        if (this.editable) {
            ctx.stroke();
        }
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

    delete() {
        for (var i = 0; i < this.scene.objects.length; i++) {
            if (this.scene.objects[i] == this) {
                let remove = new Remove(this.scene, i, this);
                scene.apply(remove);
                return
            }
        }
    }

    on_pointer_down(point) {
        if (this.scene.active_object != null) { return; }

        this.scene.set_active(this);
        this.start_position = point;
        this.offset = {
            x: 0, y: 0,
        };
    }

    on_pointer_move(point) {
        if (!this.closed) { return; }

        this.update_position(point);
    }

    on_pointer_up(point) {
        if (!this.closed) { return; }

        let action = new Move(this.scene, this, this.start_position, point, null);
        this.scene.set_active(null);
        this.scene.apply(action);
    }
}

class Waypoints extends Object {
    waypoint_size = 5;

    constructor(scene) {
        super(scene, "waypoints");
        this.waypoints = [
            new Point(scene, this, { x: 1.0, y: -1.0 }, this.waypoint_size),
            new Point(scene, this, { x: 1.0, y: 1.0 }, this.waypoint_size),
        ];
    }

    load(json) {
        this.waypoints = [];
        for (var i = 0; i < json.length; i++) {
            let point = { x: json[i][0], y: json[i][1] };
            let waypoint = new Point(this.scene, this, point, this.waypoint_size);
            this.waypoints.push(waypoint);
        }
    }

    save() {
        let waypoints = [];
        for (var i = 0; i < this.waypoints.length; i++) {
            let point = [this.waypoints[i].x, this.waypoints[i].y];
            waypoints.push(point);
        }

        return waypoints;
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
        return closest[1] < 3 * this.scene.scale(this.waypoint_size);
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

    on_pointer_down(point) {
        if (this.scene.active_object == null && edit_mode === EditModes.Waypoint) {
            let closest = this.closest_segment(point);
            let action = new ExtendWaypoints(this.scene, this, closest[0] + 1, point);
            this.scene.apply(action);
        }
    }
}

class ExtendObstacle extends Action {
    constructor(scene, obstacle, vertex) {
        super(scene);
        this.obstacle = obstacle;
        this.vertex = new Point(this.scene, this.obstacle, vertex, this.obstacle.vertex_size);
    }

    do() {
        super.do();

        let start = this.obstacle.vertices[0].point;
        let distance_to_start = Math.sqrt((start.x - this.vertex.point.x) ** 2 + (start.y - this.vertex.point.y) ** 2);
        if (distance_to_start < 3 * this.scene.scale(this.obstacle.vertex_size)) {
            this.obstacle.closed = true;
            this.scene.set_active(null);
        } else {
            this.obstacle.vertices.push(this.vertex);
        }
    }

    undo() {
        super.undo();

        if (this.obstacle.closed) {
            this.obstacle.closed = false;
        } else {
            this.obstacle.vertices.pop();
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
