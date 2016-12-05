var Meanshift = require('../index.js');

var image = new Buffer(160*120*3);

function clear() {
  image.fill(0);
}

function sq(val) {
  return val*val;
}

function dist(a, b) {
  return Math.sqrt(sq(a.x - b.x) + sq(a.y - b.y));
}

function drawSquare(x, y, size) {
  for  (var i = x - size/2; i < x + size/2; i++) {
    for (var j = y - size/2; j < y + size/2; j++) {
      var gradient = 255 - 255*(sq(i - x) + sq(j - y))/sq(size);
      var loc = (j*160 + i)*3;
      image[loc] = gradient;
      image[loc+1] = gradient;
      image[loc+2] = 255 - gradient;
    }
  }
}

var meanshift = new Meanshift.Meanshift(160, 120, Meanshift.ImageType.MD_RGB);

var paths = [
  [30, 30],
  [35, 32],
  [42, 30],
  [36, 45],
  [48, 59],
  [55, 60],
  [50, 65],
  [52, 70],
];

meanshift.initObject(paths[0][0], paths[0][1], 20, 20);

paths.forEach((p, i) => {
  var path = { x: p[0], y: p[1] };
  clear();
  drawSquare(path.x, path.y, 20);
  var pos = meanshift.track(image);
  console.log(`Position ${i}: `, path);
  console.log(`Tracked  ${i}: `, pos);

  if (dist(path, pos) > 3) {
    console.log('Too far! Regression...');
    process.exit(1);
  }
});

console.log('Tests finished');
process.exit(0);

