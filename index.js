/// <reference path="./index.d.ts" />
'use strict';

exports.ImageType = {
  MD_RGB: 0,
  MD_RGBA: 1,
  MD_YUV: 2,
  MD_WAVELET: 3,
};

exports.Meanshift = require('./build/Release/meanshift.node').Meanshift;

