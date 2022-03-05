const canvas = document.getElementById('drawCanvas');
var ctx = canvas.getContext('2d');
var drawing = false;
const offsetX = canvas.offsetLeft;
const offsetY = canvas.offsetTop;
var cw =document.getElementById('draw-container').offsetWidth;
ctx.canvas.width  = cw;
var ch = document.getElementById('draw-container').offsetHeight;
ctx.canvas.height = ch;
var drawing = "p";
var allObjects = [];
var coordinates = [];
var redoElements = [];
var coordsRedo = [];
//DEFINE DRAWING OBJECT-------------------
class DrawObject{
  constructor(ctx, type, coords){
    this.ctx = ctx;
    this.type= type;
    this.coords = coords;
  }
  draw(){
    switch(this.type){
      case "p":
        ctx.beginPath();
        ctx.moveTo(this.coords[0].x, this.coords[0].y);
        for(var index=1; index<this.coords.length;index++) {
          ctx.lineTo(this.coords[index].x, this.coords[index].y);
        }   
        ctx.closePath();
        ctx.lineWidth=2;
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
          if(this.coords.length!=0){
            ctx.arc(this.coords[0].x, this.coords[0].y, 10, 0, 2 * Math.PI, false);
            ctx.fillStyle = 'green';
            ctx.fill();
            ctx.closePath();
          }
          break;
    }
    
  }
}
var startPoint = new DrawObject(ctx, "s", []);

//---------------------------------

//CLEAR AND REDRAW ALL OBJECTS
function redrawAll(){
  ctx.clearRect(0,0,cw,ch);
  startPoint.draw();
  for(var i = 0; i<allObjects.length; i++){
    allObjects[i].draw();
  }
}

//UNDO
$(document).keydown(function(e){
  if( e.which === 90 && e.ctrlKey ){
    if(coordinates.length!=0){
      coordsRedo.push(coordinates.pop());
      redrawAll();
      drawPolygon(coordinates);
    }
    else{
      redoElements.push(allObjects.pop());
      redrawAll();
    }
  }          
}); 
//REDO
$(document).keydown(function(e){
  if( e.which === 89 && e.ctrlKey ){
    if(coordinates.length!=0 || coordsRedo.length!=0){
      if(coordsRedo.length!=0){
      coordinates.push(coordsRedo.pop());
      redrawAll();
      drawPolygon(coordinates);
      }
    }
    else{
      if(redoElements.length>0){
        allObjects.push(redoElements.pop());
      }
      redrawAll();
    }
  }          
}); 
//Set drawing modes for each button click
$("#poly_button").click(function(){
    //polygon
    drawing = "p";
});
$("#way_button").click(function(){
  //waypoint
  drawing = "w";
});
$("#start_button").click(function(){
  //start point
  drawing = "s";
});
$("#fin_button").click(function(){
    //end draw
    if(coordinates.length!=0){
      pol = new DrawObject(ctx,"p", [...coordinates]);
      allObjects.push(pol);
      redrawAll();
      coordinates = [];
      redoElements = [];
      coordsRedo = [];
    }
});
//-------------------------------------

//Handle canvas click operations-----------------------------
$("#drawCanvas").click(function(e){handleMouseDown(e);});
function handleMouseDown(e){       
    // tell the browser we're handling this event
    e.preventDefault();
    e.stopPropagation();
    mouseX=parseInt(e.clientX-offsetX);
    mouseY=parseInt(e.clientY-offsetY);
    if(drawing == "p"){
      coordinates.push({x:mouseX,y:mouseY});
      drawPolygon(coordinates);
    }
    else if(drawing == "w"){
      coordinates.push({x:mouseX,y:mouseY});
      waypoint = new DrawObject(ctx,"w", [...coordinates]);
      allObjects.push(waypoint);
      waypoint.draw();
      coordinates = [];
    }
    else if(drawing == "s"){
      coordinates.push({x:mouseX,y:mouseY});
      startPoint.coords = [...coordinates];
      redrawAll();
      coordinates = [];
    }
}
//---------------------

function drawPolygon(coords){
  redrawAll();
  ctx.beginPath();
  ctx.strokeStyle='black';
  ctx.fillStyle = 'black';
  ctx.moveTo(coords[0].x, coords[0].y);
  for(index=1; index<coords.length;index++) {
    ctx.lineTo(coords[index].x, coords[index].y);
  }   
  ctx.closePath();
  ctx.stroke();
  ctx.lineWidth=2;

  ctx.fill();
  ctx.fillStyle = 'red';

  for(var i = 0; i<coords.length;i++){
    ctx.fillRect(coords[i].x-5,coords[i].y-5,10,10);
  }
}