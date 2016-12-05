# Meanshift for Node JS #
[![CircleCI](https://circleci.com/gh/cmrigney/meanshift.svg?style=svg)](https://circleci.com/gh/cmrigney/meanshift)

This package is a wrapper of the C++ Meanshift Algorithm implementation by [Halil I. CUCE](http://www.cs.bilkent.edu.tr/~ismaila/MUSCLE/MSTracker.htm) with minor modifications. The Meanshift Algorithm is useful for object tracking in computer vision applications.

## Install ##
`npm install meanshift`

## Usage ##
```
var Meanshift = require('meanshift');

var ImageWidth = 320;
var ImageHeight = 240;
var imageData = new Buffer(320*240*3);

//... put image data in buffer here...

var meanshift = new Meanshift.Meanshift(ImageWidth, ImageHeight, Meanshift.ImageType.MD_RGB);

//Set the object to be tracked current position
meanshift.initObject(objectCenterX, objectCenterY, objectWidth, objectHeight);

var center = meanshift.track(imageData, 5 /* Optionally pass iterations, default is 5 */);

// center = { x, y }
// Go to next frame

var newCenter = meanshift.track(imageData);

// And so on
```

## Documentation ##

See typings file below:
```
declare namespace meanshift {

  /**
   * Image formats 
   * 
   * @enum {number}
   */
  enum ImageType {
    MD_RGB = 0,
    MD_RGBA = 1,
    MD_YUV = 2,
    MD_WAVELET = 3
  }

  /**
   * Position of tracked object 
   * 
   * @interface Point
   */
  interface Point {
    x: number;
    y: number;
  }

  /**
   * Meanshift algorithm class 
   * 
   * @class Meanshift
   */
  class Meanshift {

    /**
     * Creates an instance of Meanshift.
     * 
     * @param {number} imageWidth Image width
     * @param {number} imageHeight Image height
     * @param {ImageType} imageType Format of image
     */
    constructor(imageWidth: number, imageHeight: number, imageType: ImageType);

    /**
     * Tell meanshift the initial position of the object you are trying to track
     * 
     * @param {number} x Center x position of object to be tracked
     * @param {number} y Center y position of object to be tracked
     * @param {number} width Width of object to be tracked
     * @param {number} height Height of object to be tracked
     */
    initObject(x: number, y: number, width: number, height: number): void;

    /**
     * Find the new position of the object
     * 
     * @param {Buffer} imageData Buffer containing image data in format specified by imageType in the constructor
     * @param {number} [iterations] Number of iterations to algorithm should run for. Default is 5.
     * @returns {Point} Found position of object
     */
    track(imageData: Buffer, iterations?: number): Point;

  }
}

declare module 'meanshift' {
  export = meanshift;
}
```
