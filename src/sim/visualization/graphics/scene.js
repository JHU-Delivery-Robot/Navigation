class Object {
    constructor(scene, type) {
        this.scene = scene;
        this.type = type;
    }

    load(json) { }
    save() { return {}; }

    children() { return []; }
    contains(point) { return false; }

    draw(ctx) { }

    update_position() { }
    delete() { }

    on_pointer_down(point) { }
    on_pointer_move(point) { }
    on_pointer_up(point) { }
    on_child_move(point, child) { }
}

class Point extends Object {
    constructor(scene, parent, point, size) {
        super(scene, "point");
        this.parent = parent;
        this.point = point;
        this.size = size;
        this.color = "blue";
    }

    load(json) { this.point = { x: json[0], y: json[1], }; }
    save() { return [ this.point.x, this.point.y ]; }

    contains(point) {
        if (this.scene.active_object != null) { return false; }
        let distance = Math.sqrt((point.x - this.point.x) ** 2 + (point.y - this.point.y) ** 2);

        return distance <= 3 * this.scene.scale(this.size);
    }

    set_color(color) {
        this.color = color;
    }

    draw(ctx) {
        ctx.fillStyle = this.color;

        ctx.beginPath();
        ctx.arc(this.point.x, this.point.y, this.scene.scale(this.size), 0, 2 * Math.PI, false);
        ctx.closePath();
        ctx.fill();
    }

    position() {
        return this.point;
    }

    update_position(point) {
        this.point = point;
    }

    on_pointer_down(point) {
        if (this.scene.active_object == null) {
            this.scene.set_active(this);
            this.start_position = this.point;
        }
    }

    on_pointer_move(point) {
        this.update_position(point);
        this.parent.on_child_move(point, this);
    }

    on_pointer_up(point) {
        let action = new Move(this.scene, this, this.start_position, point, this.parent);
        this.scene.set_active(null);
        this.scene.apply(action);
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
            this.parent.on_child_move(this.end, this.target);
        }
    }

    undo() {
        super.undo();
        this.target.update_position(this.start);

        if (this.parent) {
            this.parent.on_child_move(this.start, this.target);
        }
    }
}

class Scene {
    constructor(canvas, pointer_handler) {
        this.canvas = canvas;

        this.objects = [];
        this.active_object = null;

        this.actions = [];
        this.undone_actions = [];

        this.pointer_handler = pointer_handler;

        this.size = 8;
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

    _canvas_scale() {
        let canvas_size = Math.min(this.canvas.width, this.canvas.height);
        let map_size = this.size;
        return map_size / canvas_size;
    }

    // transform canvas to have origin in center, X & Y like standard Cartesian coordinates,
    // and scaled to fit map to canvas
    transform_canvas(ctx) {
        let scale = this._canvas_scale();

        ctx.resetTransform();
        ctx.translate(ctx.canvas.width / 2.0, ctx.canvas.height / 2.0);
        ctx.scale(1.0/scale, -1.0/scale);
    }

    // transform mouse/touch pointer coordinates to match transformed canvas
    get_pointer(event) {
        let x = parseInt(event.clientX - this.canvas.offsetLeft);
        let y = parseInt(event.clientY - this.canvas.offsetTop);

        let scale = this._canvas_scale();

        x = (x - (this.canvas.width / 2.0)) * scale;
        y = (y - (this.canvas.height / 2.0)) * (-scale);

        return { x, y };
    }

    scale(size) {
        return size * this._canvas_scale();
    }

    draw() {
        let ctx = this.canvas.getContext('2d');
        this.clear_background(ctx);

        this.transform_canvas(ctx);

        for (var i = 0; i < this.objects.length; i++) {
            this.draw_object(ctx, this.objects[i]);
        }
    }

    set_size(size) {
        this.size = size;
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

    on_pointer_down(event) {
        let pointer = this.get_pointer(event);

        let target = this.find_collision(pointer);
        if (target != null) {
            target.on_pointer_down(pointer);
        } else {
            this.pointer_handler(pointer);
        }

        this.draw();
    }

    on_pointer_move(event) {
        let pointer = this.get_pointer(event);

        if (this.active_object != null) {
            this.active_object.on_pointer_move(pointer);
        }

        this.draw();
    }

    on_pointer_up(event) {
        let pointer = this.get_pointer(event);

        if (this.active_object != null) {
            this.active_object.on_pointer_up(pointer);
        }

        this.draw();
    }
}
